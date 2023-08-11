#include "GameScene.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void GameScene::Initialize() {
	Framework::Initialize();
	/*変数*/
	mapStratY = -50;
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

	
	//pe = new PostEffect;
	//pe->Initialize();
	ImgM = std::make_unique<ImguiManager>();
	ImgM->Initialize(winApp.get(), dxCommon.get());
	//マップ
	map = std::make_unique<Map>(mapStratY);
	map->Initialize();
	map->LoadCSV("1");

}

void GameScene::Update(){
	Framework::Update();
	
	//------------------------------
	CheckAllCollision();

	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Updata();
	enemy->Updata();
	aim->Updata();
	map->Updata();
	objSkydome->Update();
	float vec[2] = { input->GetPos().x,input->GetPos().y};
	//imgui関連
	ImgM->Begin();
	//ここから中身を書いていく
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, -100.0f, static_cast<float>(WinApp::width));
	ImGui::End();
}

void GameScene::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	enemy->Draw();
	objSkydome->Draw();
	player->Draw();
	aim->Draw();
	map->Draw();

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//imgui
	ImgM->End();
	ImgM->Draw();
	//pe->Draw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

void GameScene::Finalize(){
	Bullet::Finalize();
	EnemyBullet::Finalize();
	Map::Finalize();
	Framework::Finalize();
}

void GameScene::CheckAllCollision() {
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