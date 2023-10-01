#include "MyGame.h"
#include<MyMath.h>
#include<EnemyBullet.h>
#include<Bullet.h>
#include<Map.h>
#include<SceneFactory.h>
#define safe_delete(p)  {delete p; p = nullptr;}

void MyGame::Initialize() {
	Framework::Initialize();
	ImgM = std::make_unique<ImguiManager>();
	ImgM->Initialize(winApp.get(), dxCommon.get());
	
	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void MyGame::Update(){
	Framework::Update();
	//------------------------------
	SceneManager::GetInstance()->Update();
	
	float vec[2] = { input->GetMausePos().x,input->GetMausePos().y};
	//imgui�֘A
	ImgM->Begin();
	//�������璆�g�������Ă���
	/*ImGui::Begin("a");
	ImGui::SliderFloat2("mousePos", vec, -100.0f, static_cast<float>(WinApp::width));
	ImGui::End();*/
}

void MyGame::Draw(){
	//PostEffect::PostDrawScene(dxCommon->GetCommandList());
	//PostEffect::PreDrawScene(dxCommon->GetCommandList());

	//Direct���t���[�������@��������
	dxCommon->PreDraw();
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());
	SceneManager::GetInstance()->ObjDraw();
	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();

	//�X�v���C�g�`��
	SpriteCommon::GetInstance()->PreDraw();
	SceneManager::GetInstance()->SpriteDraw();
	SpriteCommon::GetInstance()->PostDraw();

	//imgui
	ImgM->End();
	ImgM->Draw();
	//pe->ObjDraw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

void MyGame::Finalize(){
	Bullet::Finalize();
	EnemyBullet::Finalize();
	Map::Finalize();
	Framework::Finalize();
}

