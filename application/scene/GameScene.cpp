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
	camera->SetEye({ 0,80,-20 });
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
	cube = Model::LoadFromOBJ("cube");
	box = Model::LoadFromOBJ("player");

	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome);
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });


	aim = std::make_unique<Aimposition>();
	aim->Initialeze(box, input);
	
	player = std::make_unique<Player>();
	player->Initialeze(box, input, aim.get());

	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(cube, player.get());

	//�|�X�g�G�t�F�N�g
	PostEffect::StaticInitialize(dxCommon);
	//pe = new PostEffect;
	//pe->Initialize();
	velocity = new ImguiManager;
	velocity->Initialize(winApp, dxCommon);

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
	

	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Updata();
	enemy->Updata();
	aim->Updata();
	objSkydome->Update();
	float vec[2] = { input->GetPos().x,input->GetPos().y};
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
	velocity->End();
	velocity->Draw();
	//pe->Draw(dxCommon->GetCommandList());

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
	delete modelSkydome;
	delete cube;
	delete box;
}