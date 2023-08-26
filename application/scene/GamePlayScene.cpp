#include "GamePlayScene.h"
#include<SceneManager.h>

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
	modelMap.reset(Model::LoadFromOBJ("map"));

	//モデルのセット
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });

	//レクティル
	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input.get());
	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input.get(), aim.get());
	//エネミー
	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//マップ
	map = std::make_unique<Map>(mapStratY);
	map->Initialize();
	map->LoadCSV("1");
}

void GamePlayScene::Update(){
	CheckAllCollision();
	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Update();
	enemy->Update();
	aim->Update();
	map->Update();
	objSkydome->Update();
	if (input->TriggerKey(DIK_1)||player->IsDead())
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
	if (input->TriggerKey(DIK_2))
	{
		//シーンの切り替え
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}
}

void GamePlayScene::SpriteDraw() {
	player->SpriteDraw();
}

void GamePlayScene::ObjDraw(){
	enemy->Draw();
	objSkydome->Draw();
	player->ObjDraw();
	//aim->ObjDraw();
	map->Draw();
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
			posB = e_bullet->GetBulletPosition();
			//A,Bの距離
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float	radius = player->GetRadius() + e_bullet->GetRadius();
			//判定
			if (dis <= radius) {
				//自キャラのコールバックを呼び出し
				player->OnCollision();
				//敵弾のコールバックを呼び出し
				e_bullet->OnCollision();
			}
		}
	#pragma	endregion
	
	#pragma region 自弾と敵キャラの当たり判定
		//敵弾の座標
		posA = enemy->GetPos();
		//敵キャラと自弾全ての当たり判定
		for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
			//自弾の座標
			posB = p_bullet->GetBulletPosition();
			// A,Bの距離
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float radius = enemy->GetRadius() + p_bullet->GetRadius();
			//判定
			if (dis <= radius) {
				//敵キャラのコールバックを呼び出し
				enemy->OnCollision();
				//自弾のコールバックを呼び出し
				p_bullet->OnCollision();

				//シーンの切り替え
				SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
			}
		}
	#pragma endregion
	
	#pragma region 自弾と敵弾の当たり判定
		//自弾の座標
		for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
			posA = p_bullet->GetBulletPosition();
			//自弾と敵弾全ての当たり判定
			for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
				//敵弾の座標
				posB = e_bullet->GetBulletPosition();
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
				}
			}
		}
	#pragma endregion
}

void GamePlayScene::Finalize(){}