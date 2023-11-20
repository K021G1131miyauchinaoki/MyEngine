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
/// <summary>
/// 矩形の判定
/// </summary>
bool CheckBoxXZ( Vector3 pos1,Vector3 scale1,Vector3 pos2,Vector3 scale2)
{
	float cl1 = pos1.x - scale1.x;
	float cr1 = cl1 + ( scale1.x * 2 );
	float cl2 = pos2.x - scale2.x;
	float cr2 = cl2 + ( scale2.x * 2 );

	if ( cr1 <= cl2 )
	{
		return false;
	}

	if ( cr2 <= cl1 )
	{
		return false;
	}

	float cf1 = pos1.z - scale1.z;
	float cb1 = cf1 + ( scale1.z * 2 );
	float cf2 = pos2.z - scale2.z;
	float cb2 = cf2 + ( scale2.z * 2 );

	if ( cb1 <= cf2 )
	{
		return false;
	}

	if ( cb2 <= cf1 )
	{
		return false;
	}

	return true;
}
/// <summary>
/// 線分の判定
/// </summary>
/// <param name="startA">直線のスタート</param>
/// <param name="startB">線分のスタート</param>
/// <param name="endA">直線のエンド</param>
/// <param name="endB">線分のエンド</param>
/// <returns></returns>
int	HitLine(/*始点*/Vector3 startA,Vector3 startB,
	/*終点*/ Vector3 endA,Vector3 endB)//aが直線、bが線分,_sはstartの略,_lはlastの略
{

	{
		const	float	baseX = endB.x - startB.x;	//目標までの距離x
		const	float	baseY = endB.z - startB.z;	//		〃		y
		const	float	sub1X = startA.x - startB.x;	//		〃		x
		const	float	sub1Y = startA.z - startB.z;	//		〃		y
		const	float	sub2X = endA.x - startB.x;	//		〃		x
		const	float	sub2Y = endA.z - startB.z;	//		〃		y

		const	float	bs1 = baseX * sub1Y - baseY * sub1X;
		const	float	bs2 = baseX * sub2Y - baseY * sub2X;
		const	float	re = bs1 * bs2;

		if ( re > 0 )
		{
			return	false;
		}
	}
	{
		const	float	baseX = endA.x - startA.x;	//目標までの距離x
		const	float	baseY = endA.z - startA.z;	//		〃		y
		const	float	sub1X = startB.x - startA.x;	//		〃		x
		const	float	sub1Y = startB.z - startA.z;	//		〃		y
		const	float	sub2X = endB.x - startA.x;	//		〃		x
		const	float	sub2Y = endB.z - startA.z;	//		〃		y

		const	float	bs1 = baseX * sub1Y - baseY * sub1X;
		const	float	bs2 = baseX * sub2Y - baseY * sub2X;
		const	float	re = bs1 * bs2;

		if ( re > 0 )
		{
			return	false;
		}
	}
	return	true;
}

void GamePlayScene::Initialize() {
	input.reset(Input::GetInstance());
	/*変数*/
	mapStratY = -400;
	
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());

	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	light->SetLightDir(lightDir);
	light->Updata();
	Object3d::SetLight(light.get());

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome",true));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	had.reset(Model::LoadFromOBJ("TankHad"));
	body.reset(Model::LoadFromOBJ("TankBody"));
	modelMap.reset(Model::LoadFromOBJ("map"));
	parachute.reset(Model::LoadFromOBJ("parachute"));
	//プレイヤー
	player = std::make_unique<Player>();
	player->PlayInitialeze(had.get(),body.get(),parachute.get(),input.get());
	//json読み込み
	jsonLoader = std::make_unique<LevelData>();
	jsonLoader.reset(LevelLoader::LoadJson("1"));
	models.insert(std::make_pair("enemy",tank.get()));
	models.insert(std::make_pair("block",modelMap.get()));

	// レベルデータからオブジェクトを生成、配置
	for ( auto& objectData : jsonLoader->objects )
	{
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype( models )::iterator it = models.find(objectData.fileName);
		if ( it != models.end() )
		{
			model = it->second;
		}

		//エネミー
		if ( objectData.fileName=="enemy" )
		{
			Enemy* newEnemy = new Enemy();
			newEnemy->Initialeze(model,player.get(),objectData.translation,objectData.rotation);
			enemys.emplace_back(std::move(newEnemy));

		}
		//ブロック
		if ( objectData.fileName == "block" )
		{
			BaseBlock* newBlock = new BaseBlock();
			newBlock->obj = std::make_unique<Object3d>();
			newBlock->obj->Initialize();
			newBlock->obj->SetModel(model);
			newBlock->obj->SetColor({0.5f,0.5f,0.5f,1.0f});
			newBlock->obj->SetPosition(objectData.translation);
			newBlock->obj->SetScale(objectData.scaling);
			blocks.emplace_back(std::move(newBlock));

		}
	}
	//モデルのセット
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());	
	objSkydome->SetScale({ 200.0f,200.0f,200.0f });



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
	enemys.remove_if([ ] (std::unique_ptr<Enemy>& enemy)
	{
		return enemy->IsDead();
	});
	const XMFLOAT3 cameraPos = { player->GetPos().x, 100, player->GetPos().z - 30.0f };
	if ( !player->IsDead() && enemys.size()!=0 )
	{
		camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
		camera->SetEye(cameraPos);
		camera->Update();
		player->Update();
		for ( std::unique_ptr<Enemy>& enemy : enemys )
		{
			enemy->Update();
		}
		for ( std::unique_ptr<BaseBlock>& block : blocks )
		{
			block->obj->Update();
		}
		map->Update();
		objSkydome->SetPosition(player->GetPos());
		objSkydome->Update();
		particle->Update();
		CheckAllCollision();
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
		if (enemys.size()==0)
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
	for ( std::unique_ptr<Enemy>& enemy : enemys )
	{
		enemy->Draw();
	}
	for ( std::unique_ptr<BaseBlock>& block : blocks )
	{
		block->obj->Draw();
	}
	objSkydome->Draw();
	player->ObjDraw();
	map->Draw();
	particle->Draw();
}

void GamePlayScene::CheckAllCollision() {
		
	
		//自弾リストを取得
		const std::list<std::unique_ptr<Bullet>>& playerBullets = player->GetBullets();
		//敵弾リストを取得
		//自キャラの座標
		for ( std::unique_ptr<Enemy>& enemy : enemys )
		{
			const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
			#pragma	region	自キャラと敵弾の当たり判定
			//自キャラと敵弾全ての当たり判定
			for ( const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//自キャラの座標
				posA = player->GetPos();
				//敵弾の座標
				posB = e_bullet->GetPos();
				//A,Bの距離
				Vector3 vecPos = MyMath::lens(posA,posB);
				float dis = MyMath::Length(vecPos);
				//
				float	radius = player->GetRadius() + e_bullet->GetRadius();
				//判定
				if ( dis <= radius )
				{
					//自キャラのコールバックを呼び出し
					//player->OnCollision();
					//敵弾のコールバックを呼び出し
					e_bullet->OnCollision();
					particle->Add("1",30,15,player->GetPos(),1.0f,0.0f);
				}
			}
			#pragma	endregion

			#pragma region 自弾と敵キャラの当たり判定
				
			//敵キャラと自弾全ての当たり判定
			for ( const std::unique_ptr<Bullet>& p_bullet : playerBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//敵弾の座標
				posA = enemy->GetPos();
				//自弾の座標
				posB = p_bullet->GetPos();
				// A,Bの距離
				Vector3 vecPos = MyMath::lens(posA,posB);
				float dis = MyMath::Length(vecPos);
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
			for ( const std::unique_ptr<Bullet>& p_bullet : playerBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//自弾の座標
				posA = p_bullet->GetPos();
				//自弾と敵弾全ての当たり判定
				for ( const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets )
				{
					//敵弾の座標
					posB = e_bullet->GetPos();
					// A,Bの距離
					Vector3 vecPos = MyMath::lens(posA,posB);
					float dis = MyMath::Length(vecPos);
					//
					float radius = e_bullet->GetRadius() + p_bullet->GetRadius();
					//判定
					if ( dis <= radius )
					{
						//自弾のコールバックを呼び出し
						p_bullet->OnCollision();
						//敵弾のコールバックを呼び出し
						e_bullet->OnCollision();
						particle->Add("1",5,10,p_bullet->GetPos(),1.0f,0.0f);
						particle->Add("1",5,10,e_bullet->GetPos(),1.0f,0.0f);
					}
				}
			}
			#pragma endregion

			#pragma region 敵弾とブロックの当たり判定
			for ( const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//自弾の座標
				posA = e_bullet->GetPos();
				//自弾と敵弾全ての当たり判定
				for ( const std::unique_ptr<BaseBlock>& block : blocks )
				{
					//敵弾の座標
					posB = block->obj->GetPosition();

					//判定
					if ( CheckBoxXZ(posA,e_bullet->GetScale(),posB,block->obj->GetScale()) )
					{
						//自弾のコールバックを呼び出し
						e_bullet->OnCollision();
						//パーティクルの呼び出し
						particle->Add("1",5,10,e_bullet->GetPos(),1.0f,0.0f);
					}
				}
			}
			#pragma endregion

			for ( const std::unique_ptr<BaseBlock>& block : blocks )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				posA = enemy->GetPos();
				//敵弾の座標
				posB = block->obj->GetPosition();

				//判定
				if(CheckBoxXZ(posA,enemy->GetScale(),block->obj->GetPosition(),block->obj->GetScale()))
				{
					enemy->OnCollisionPos();
				}
			}
			

		}
		for ( const std::unique_ptr<BaseBlock>& block : blocks )
		{
			
			#pragma region 自弾とブロックの当たり判定
			for ( const std::unique_ptr<Bullet>& p_bullet : playerBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//自弾の座標
				posA = p_bullet->GetPos();
				//ブロック
				posB = block->obj->GetPosition();				
				
				//判定
				if ( CheckBoxXZ(posA,p_bullet->GetScale(),posB,block->obj->GetScale()) )
				{
					//自弾のコールバックを呼び出し
					p_bullet->OnCollision();
					//パーティクルの呼び出し
					particle->Add("1",5,10,p_bullet->GetPos(),1.0f,0.0f);
				}
			}
			#pragma endregion

			{
				Vector3 topLP,topRP,bottomRP,bottomLP;
				Vector3 topLB,topRB,bottomRB,bottomLB;
				/*プレイヤー*/
				//左奥
				topLP.x = player->GetPos().x - player->GetScale().x;
				topLP.z = player->GetPos().z + player->GetScale().z;
				//右奥
				topRP.x = player->GetPos().x + player->GetScale().x;
				topRP.z = player->GetPos().z + player->GetScale().z;

				//左前
				bottomLP.x = player->GetPos().x - player->GetScale().x;
				bottomLP.z = player->GetPos().z - player->GetScale().z;
				//右前
				bottomRP.x = player->GetPos().x + player->GetScale().x;
				bottomRP.z = player->GetPos().z - player->GetScale().z;

				/*ブロック*/
				//左奥
				topLB.x = block->obj->GetPosition().x - block->obj->GetScale().x;
				topLB.z = block->obj->GetPosition().z + block->obj->GetScale().z;
				//右奥
				topRB.x = block->obj->GetPosition().x + block->obj->GetScale().x;
				topRB.z = block->obj->GetPosition().z + block->obj->GetScale().z;

				//左前
				bottomLB.x = block->obj->GetPosition().x - block->obj->GetScale().x;
				bottomLB.z = block->obj->GetPosition().z - block->obj->GetScale().z;
				//右前
				bottomRB.x = block->obj->GetPosition().x + block->obj->GetScale().x;
				bottomRB.z = block->obj->GetPosition().z - block->obj->GetScale().z;
			}

			#pragma region 自機とブロックの当たり判定
			if ( CheckBoxXZ(player->GetPos(),player->GetScale(),block->obj->GetPosition(),block->obj->GetScale()))
			{
				player->OnCollisionPos();
			}
			/*if ( SideHit(posA,scaleA,posB,scaleB,"-x") )
			{
				block->obj->SetColor({ 0.0f,1.0f,0.0f,1.0f });
			}
			if ( SideHit(posA,scaleA,posB,scaleB,"z") )
			{
				block->obj->SetColor({ 0.0f,0.0f,1.0f,1.0f });
			}
			if ( SideHit(posA,scaleA,posB,scaleB,"-z") )
			{
				block->obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
			}*/
			#pragma endregion
			
		}
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