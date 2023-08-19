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
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	sceneManager->SetNextScene(scene);
}

void MyGame::Update(){
	Framework::Update();
	//------------------------------
	CheckAllCollision();
	sceneManager->Update();
	
	float vec[2] = { input->GetMausePos().x,input->GetMausePos().y};
	//imgui�֘A
	ImgM->Begin();
	//�������璆�g�������Ă���
	ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, -100.0f, static_cast<float>(WinApp::width));
	ImGui::End();
}

void MyGame::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

	//Direct���t���[�������@��������
	dxCommon->PreDraw();
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());
	sceneManager->ObjDraw();
	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g�`��
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
//	//����Ώ�A��B�̍��W
//	Vector3 posA, posB;
//
//	//���e���X�g���擾
//	const std::list<std::unique_ptr<Bullet>>& playerBullets = player->GetBullets();
//	//�G�e���X�g���擾
//	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
//#pragma	region	���L�����ƓG�e�̓����蔻��
//	//���L�����̍��W
//	posA = player->GetMausePos();
//	//���L�����ƓG�e�S�Ă̓����蔻��
//	for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
//		//�G�e�̍��W
//		posB = e_bullet->GetBulletPosition();
//		//A,B�̋���
//		Vector3 vecPos = MyMath::lens(posA, posB);
//		float dis = MyMath::length(vecPos);
//		//
//		float	radius = player->GetRadius() + e_bullet->GetRadius();
//		//����
//		if (dis <= radius) {
//			//���L�����̃R�[���o�b�N���Ăяo��
//			player->OnCollision();
//			//�G�e�̃R�[���o�b�N���Ăяo��
//			e_bullet->OnCollision();
//		}
//	}
//#pragma	endregion
//
//#pragma region ���e�ƓG�L�����̓����蔻��
//	//�G�e�̍��W
//	posA = enemy->GetMausePos();
//	//�G�L�����Ǝ��e�S�Ă̓����蔻��
//	for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
//		//���e�̍��W
//		posB = p_bullet->GetBulletPosition();
//		// A,B�̋���
//		Vector3 vecPos = MyMath::lens(posA, posB);
//		float dis = MyMath::length(vecPos);
//		//
//		float radius = enemy->GetRadius() + p_bullet->GetRadius();
//		//����
//		if (dis <= radius) {
//			//�G�L�����̃R�[���o�b�N���Ăяo��
//			enemy->OnCollision();
//			//���e�̃R�[���o�b�N���Ăяo��
//			p_bullet->OnCollision();
//		}
//	}
//#pragma endregion
//
//#pragma region ���e�ƓG�e�̓����蔻��
//	//���e�̍��W
//	for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
//		posA = p_bullet->GetBulletPosition();
//		//���e�ƓG�e�S�Ă̓����蔻��
//		for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
//			//�G�e�̍��W
//			posB = e_bullet->GetBulletPosition();
//			// A,B�̋���
//			Vector3 vecPos = MyMath::lens(posA, posB);
//			float dis = MyMath::length(vecPos);
//			//
//			float radius = e_bullet->GetRadius() + p_bullet->GetRadius();
//			//����
//			if (dis <= radius) {
//				//���e�̃R�[���o�b�N���Ăяo��
//				p_bullet->OnCollision();
//				//�G�e�̃R�[���o�b�N���Ăяo��
//				e_bullet->OnCollision();
//			}
//		}
//	}
//#pragma endregion
}