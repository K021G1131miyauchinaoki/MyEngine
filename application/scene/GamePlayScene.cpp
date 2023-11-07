/**
 * @file GamePlayScene.cpp
 * @brief ベースシーンを継承したプレイシーン
 */
#include "GamePlayScene.h"
#include"SceneTransition.h"
#include<SpriteCommon.h>
#include"WinApp.h"
#include"Easing.h"
#include<SceneManager.h>
#include<Vector3.h>

int8_t GamePlayScene::startCount = 0;
bool GamePlayScene::isStart = true;

void GamePlayScene::Initialize() {
	input.reset(Input::GetInstance());
	/*変数*/
	mapStratY = -400;
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	had.reset(Model::LoadFromOBJ("TankHad"));
	body.reset(Model::LoadFromOBJ("TankBody"));
	modelMap.reset(Model::LoadFromOBJ("map"));
	parachute.reset(Model::LoadFromOBJ("parachute"));
	//モデルのセット
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());	
	objSkydome->SetScale({ 200.0f,200.0f,200.0f });


	//プレイヤー
	player = std::make_unique<Player>();
	player->PlayInitialeze(had.get(),body.get(),parachute.get(),input.get());
	//エネミー
	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//マップ
	map = std::make_unique<Map>(mapStratY);
	map->Initialize(true);
	map->LoadCSV("1");

	//パーティクル
	particle = std::make_unique <ParticleManager>();
	particle->Initialize(cube.get());

	startCount = 0;
	isStart = true;

	spriteEaseTime = 0.0f;
	spriteWaitTime = 0.0f;
	rPosStartY=WinApp::height+150.0f;
	sPosStartY=-100.0f;
	rPosEndY = (static_cast<float>( WinApp::height) / 2.0f ) + 50.0f;
	sPosEndY= ( static_cast< float >( WinApp::height ) / 2.0f ) - 50.0f;

	ready = std::make_unique<Sprite>();
	ready->Initialize(SpriteCommon::GetInstance(),7);
	ready->SetAnchorPoint({ 0.5f,0.5f });
	ready->SetPosition({ WinApp::width / 2.0f,rPosStartY });

	stage = std::make_unique<Sprite>();
	stage->Initialize(SpriteCommon::GetInstance(),8);
	stage->SetAnchorPoint({ 0.5f,0.5f });
	stage->SetPosition({ 640.0f,sPosStartY });
}

void GamePlayScene::Update(){
	const XMFLOAT3 cameraPos = { player->GetPos().x, 100, player->GetPos().z - 30.0f };
	if ( !player->IsDead() && !enemy->IsDead() )
	{
		CheckAllCollision();
		camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
		camera->SetEye(cameraPos);
		camera->Update();
		player->Update();
		enemy->Update();
		map->Update();
		objSkydome->Update();
		particle->Update();
	}
	//シーン遷移のフラグを立てる
	else if ( !SceneTransition::GetInstance()->GetIsFadeOut() &&
		!SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		SceneTransition::GetInstance()->IsFadeOutTrue();
	}
	//画面真っ暗になったら
	if (!SceneTransition::GetInstance()->GetIsFadeOut()&&
		SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		//シーンの切り替え
		if (player->IsDead())
		{
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
		if (enemy->IsDead())
		{
			SceneManager::playerHP = player->GetHp();
			SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		}
	}
	StartStaging();
}

void GamePlayScene::SpriteDraw() {
	player->SpriteDraw();
	stage->Draw();
	ready->Draw();
}

void GamePlayScene::ObjDraw(){
	enemy->Draw();
	objSkydome->Draw();
	player->ObjDraw();
	map->Draw();
	particle->Draw();
}

void GamePlayScene::CheckAllCollision() {
		//判定対象AとBの座標
		Vector3 posA, posB;
	
		//自弾リストを取得
		const std::list<std::unique_ptr<Bullet>>& playerBullets = player->GetBullets();
		//敵弾リストを取得
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
	#pragma	region	自キャラと敵弾の当たり判定
		//自キャラの座標
		posA = player->GetPos();
		//自キャラと敵弾全ての当たり判定
		for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
			//敵弾の座標
			posB = e_bullet->GetPos();
			//A,Bの距離
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float	radius = player->GetRadius() + e_bullet->GetRadius();
			//判定
			if (dis <= radius) {
				//自キャラのコールバックを呼び出し
				//player->OnCollision();
				//敵弾のコールバックを呼び出し
				e_bullet->OnCollision();
				particle->Add("1", 30, 15, player->GetPos(), 1.0f, 0.0f);
			}
		}
	#pragma	endregion
	
	#pragma region 自弾と敵キャラの当たり判定
		//敵弾の座標
		posA = enemy->GetPos();
		//敵キャラと自弾全ての当たり判定
		for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
			//自弾の座標
			posB = p_bullet->GetPos();
			// A,Bの距離
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float radius = enemy->GetRadius() + p_bullet->GetRadius();
			//判定
			if ( dis <= radius )
			{
//敵キャラのコールバックを呼び出し
				enemy->OnCollision();
				//自弾のコールバックを呼び出し
				p_bullet->OnCollision();

				particle->Add("1",30,15,enemy->GetPos(),1.0f,0.0f);
			}
		}
	#pragma endregion
	
	#pragma region 自弾と敵弾の当たり判定
		//自弾の座標
		for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
			posA = p_bullet->GetPos();
			//自弾と敵弾全ての当たり判定
			for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
				//敵弾の座標
				posB = e_bullet->GetPos();
				// A,Bの距離
				Vector3 vecPos = MyMath::lens(posA, posB);
				float dis = MyMath::length(vecPos);
				//
				float radius = e_bullet->GetRadius() + p_bullet->GetRadius();
				//判定
				if (dis <= radius) {
					//自弾のコールバックを呼び出し
					p_bullet->OnCollision();
					//敵弾のコールバックを呼び出し
					e_bullet->OnCollision();
					particle->Add("1", 5, 10, p_bullet->GetPos(), 1.0f, 0.0f);
					particle->Add("1", 5, 10, e_bullet->GetPos(), 1.0f, 0.0f);
				}
			}
		}
	#pragma endregion
}

void GamePlayScene::StartStaging() {
	if ( startCount >= start::Go)
	{
		isStart = false;
		spriteWaitTime = 0;
		spriteEaseTime = 0;
	}
	if ( startCount== start::Redy )
	{
		XMFLOAT2 readyPos=ready->GetPosition();
		XMFLOAT2 stagePos=stage->GetPosition();
		
		if ( spriteEaseTime== spriteEaseTimer )
		{
			spriteWaitTime++;
		}
		if ( spriteWaitTime>= spriteWaitTimer|| spriteWaitTime==0.0f )
		{
			spriteEaseTime++;
			if ( spriteEaseTime == (spriteEaseTimer*2.0f) )
			{
				startCount++;
			}
		}

		readyPos.y = rPosStartY + ( rPosEndY - rPosStartY ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
		stagePos.y = sPosStartY + ( sPosEndY - sPosStartY ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
		ready->SetPosition(readyPos);
		stage->SetPosition(stagePos);
	}
	stage->Update();
	ready->Update();
}

void GamePlayScene::Finalize(){}

GamePlayScene::GamePlayScene() {}
GamePlayScene::~GamePlayScene() {
	Finalize();
}