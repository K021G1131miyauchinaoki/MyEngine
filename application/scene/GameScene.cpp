#include "GameScene.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void GameScene::Initialize() {
	winApp = new	WinApp;
	winApp->Initialize();

	dxCommon = new DirectXCommon;
	dxCommon->Initialize(winApp);

	input = new	Input;
	input->Initialize(winApp);
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	Model::SetDevice(dxCommon->GetDevice());
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,80,-20 });
	camera->Update();
	//デバイスをセット
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::width, WinApp::height, camera.get());
	//デバイスをセット
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// カメラをセット
	FbxObject3d::SetCamera(camera.get());
	//グラフィックパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();
	
	//---------------------------2D----------------------------------


	//---------------------------3D----------------------------------


	/*---------------- - FBX------------------------*/
	////3Dオブジェクト生成とモデルセット
	//fbxM = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//fbxObj = new FbxObject3d;
	//fbxObj->Initialize();
	//fbxObj->SetModel(fbxM);
	//fbxObj->PlayAnimation();

	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());

	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });


	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input);
	
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input, aim.get());

	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//ポストエフェクト
	PostEffect::StaticInitialize(dxCommon);
	//pe = new PostEffect;
	//pe->Initialize();
	ImgM = std::make_unique<ImguiManager>();
	ImgM->Initialize(winApp, dxCommon);
	//マップ
	map = std::make_unique<Map>();
	map->Initialize(modelMap.get());

}

void GameScene::Update(){
#pragma region メッセージ
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEndRequst = true;
	}
#pragma endregion
	input->Update();
	//imgui
	// デモ
	//ImGui::ShowDemoWindow();
	
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
	float posA[2] = { aim->GetPosition().x,aim->GetPosition().z };
	//imgui関連
	ImgM->Begin();
	//ここから中身を書いていく
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, 0.0f, static_cast<float>(WinApp::width));
	ImGui::End();
	//ImGui::SliderFloat2("pos",posA,0.0f, WinApp::width);
	//fbxObj->Update();

	//for (auto object : objects) {
	//	object->Update();
	//}
}

void GameScene::Draw(){
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

bool GameScene::IsEndRequst(){
	return isEndRequst;
}

void GameScene::Finalize(){
	//解放処理
	winApp->Finalize();
	
	FbxLoader::GetInstance()->Finalize();
	//safe_delete(fbxObj);
	//safe_delete(fbxM);
	Bullet::Finalize();
	EnemyBullet::Finalize();

	delete	input;
	delete winApp;
	delete	dxCommon;
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