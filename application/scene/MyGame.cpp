#include "MyGame.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void MyGame::Initialize() {
	Framework::Initialize();
	ImgM = std::make_unique<ImguiManager>();
	ImgM->Initialize(winApp.get(), dxCommon.get());
	BaseScene* scene = new TitleScene();
	//シーンマネージャーに最初のシーンをセット
	sceneManager->SetNextScene(scene);
}

void MyGame::Update(){
	Framework::Update();
	//------------------------------
	CheckAllCollision();
	sceneManager->Update();
	
	float vec[2] = { input->GetMausePos().x,input->GetMausePos().y};
	//imgui関連
	ImgM->Begin();
	//ここから中身を書いていく
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, -100.0f, static_cast<float>(WinApp::width));
	ImGui::End();
}

void MyGame::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

	//Direct毎フレーム処理　ここから
	dxCommon->PreDraw();
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(dxCommon->GetCommandList());
	sceneManager->ObjDraw();
	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();

	//スプライト描画
	SpriteCommon::GetInstance()->PreDraw();
	sceneManager->SpriteDraw();
	SpriteCommon::GetInstance()->PostDraw();

	//imgui
	ImgM->End();
	ImgM->Draw();
	//pe->Draw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

void MyGame::Finalize(){
	Bullet::Finalize();
	EnemyBullet::Finalize();
	Map::Finalize();
	Framework::Finalize();
}

void MyGame::CheckAllCollision() {
//	//判定対象AとBの座標
//	Vector3 posA, posB;
//
//	//自弾リストを取得
//	const std::list<std::unique_ptr<Bullet>>& playerBullets = player->GetBullets();
//	//敵弾リストを取得
//	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
//#pragma	region	自キャラと敵弾の当たり判定
//	//自キャラの座標
//	posA = player->GetMausePos();
//	//自キャラと敵弾全ての当たり判定
//	for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
//		//敵弾の座標
//		posB = e_bullet->GetBulletPosition();
//		//A,Bの距離
//		Vector3 vecPos = MyMath::lens(posA, posB);
//		float dis = MyMath::length(vecPos);
//		//
//		float	radius = player->GetRadius() + e_bullet->GetRadius();
//		//判定
//		if (dis <= radius) {
//			//自キャラのコールバックを呼び出し
//			player->OnCollision();
//			//敵弾のコールバックを呼び出し
//			e_bullet->OnCollision();
//		}
//	}
//#pragma	endregion
//
//#pragma region 自弾と敵キャラの当たり判定
//	//敵弾の座標
//	posA = enemy->GetMausePos();
//	//敵キャラと自弾全ての当たり判定
//	for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
//		//自弾の座標
//		posB = p_bullet->GetBulletPosition();
//		// A,Bの距離
//		Vector3 vecPos = MyMath::lens(posA, posB);
//		float dis = MyMath::length(vecPos);
//		//
//		float radius = enemy->GetRadius() + p_bullet->GetRadius();
//		//判定
//		if (dis <= radius) {
//			//敵キャラのコールバックを呼び出し
//			enemy->OnCollision();
//			//自弾のコールバックを呼び出し
//			p_bullet->OnCollision();
//		}
//	}
//#pragma endregion
//
//#pragma region 自弾と敵弾の当たり判定
//	//自弾の座標
//	for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
//		posA = p_bullet->GetBulletPosition();
//		//自弾と敵弾全ての当たり判定
//		for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
//			//敵弾の座標
//			posB = e_bullet->GetBulletPosition();
//			// A,Bの距離
//			Vector3 vecPos = MyMath::lens(posA, posB);
//			float dis = MyMath::length(vecPos);
//			//
//			float radius = e_bullet->GetRadius() + p_bullet->GetRadius();
//			//判定
//			if (dis <= radius) {
//				//自弾のコールバックを呼び出し
//				p_bullet->OnCollision();
//				//敵弾のコールバックを呼び出し
//				e_bullet->OnCollision();
//			}
//		}
//	}
//#pragma endregion
}