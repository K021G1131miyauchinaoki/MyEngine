#include "GameScene.h"
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
	//�ÓI������
	//---------------------------2D----------------------------------


	//---------------------------3D----------------------------------
	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetTarget({ 0,0,0 });
	camera->SetEye({ 0,60,-20 });
	camera->Update();
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::width, WinApp::height, camera.get());

	/*---------------- - FBX------------------------*/
	//fbxM = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	////�f�o�C�X���Z�b�g
	//FbxObject3d::SetDevice(dxCommon->GetDevice());
	//// �J�������Z�b�g
	//FbxObject3d::SetCamera(camera.get());
	////�O���t�B�b�N�p�C�v���C������
	//FbxObject3d::CreateGraphicsPipeline();
	////3D�I�u�W�F�N�g�����ƃ��f���Z�b�g
	//fbxObj = new FbxObject3d;
	//fbxObj->Initialize();
	//fbxObj->SetModel(fbxM);
	//fbxObj->PlayAnimation();
	
	// ���f���ǂݍ���
	modelSkydome = Model::LoadFromOBJ("skydome");
	/*modelGround = Model::LoadFromOBJ("ground");
	modelChr = Model::LoadFromOBJ("chr_sword");
	modelSphere = Model::LoadFromOBJ("sphere");*/
	box = Model::LoadFromOBJ("player");

	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome);
	
	/*--------------���x���G�f�B�^----------------*/
	// ���x���f�[�^�̓ǂݍ���
	levelData = LevelLoader::LoadJson("testScene");
	


	aim= std::make_unique<Aimposition>();
	aim->Initialeze(box, input);
	player = std::make_unique<Player>();
	player->Initialeze(box,input,aim.get());

	//�|�X�g�G�t�F�N�g
	PostEffect::StaticInitialize(dxCommon);
	pe = new PostEffect;
	pe->Initialize();
}

void GameScene::Update(){
#pragma region ���b�Z�[�W
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEndRequst = true;
	}
#pragma endregion
	input->Update();
	//imgui
	// �f��
	//ImGui::ShowDemoWindow();
	
	//------------------------------
	

	camera->Update();
	player->Updata();
	aim->Updata();
	objSkydome->Update();
	float vec[2] = { player->GetVelocity().x,player->GetVelocity().z };
	float posA[2] = { aim->GetPosition().x,aim->GetPosition().z };
	//imgui�֘A
	velocity->Begin();
	//�������璆�g�������Ă���
	ImGui::Begin("a");
	ImGui::SliderFloat2("volocity", vec, 0.0f, WinApp::width);
	ImGui::End();
	//ImGui::SliderFloat2("pos",posA,0.0f, WinApp::width);
	//fbxObj->Update();

	//for (auto object : objects) {
	//	object->Update();
	//}
}

void GameScene::Draw(){
	//Direct���t���[�������@��������
	dxCommon->PreDraw();
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>

	/*for (auto object : objects) {
		object->Draw();
	}*/

	objSkydome->Draw();
	player->Draw();
	aim->Draw();
	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	//fbxObj->Draw(dxCommon->GetCommandList());

	//�p�[�e�B�N���`��
	
	//�X�v���C�g�`��

	//imgui
	//imguiM->Draw();
	pe->Draw(dxCommon->GetCommandList());

	dxCommon->PostDraw();
}

bool GameScene::IsEndRequst(){
	return isEndRequst;
}

void GameScene::Finalize(){
	//�������
	winApp->Finalize();
	
	FbxLoader::GetInstance()->Finalize();
	//safe_delete(fbxObj);
	//safe_delete(fbxM);

	delete	input;
	delete winApp;
	delete	dxCommon;
	delete velocity;
	/*delete modelChr;
	delete modelGround;
	delete modelSkydome;
	delete modelSphere;
	delete objChr;
	delete	objSkydome;
	delete	objGround;
	delete	objSphere;*/

}