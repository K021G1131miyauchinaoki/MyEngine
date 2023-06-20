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
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::width, WinApp::height, camera.get());
	//---------------------------2D----------------------------------

	//�X�v���C�g���ʕ����̏�����
	
	//imgui
	

	//---------------------------3D----------------------------------
	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	camera->SetTarget({ 0,20,-30 });

	/*---------------- - FBX------------------------*/
	modelF = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	// �J�������Z�b�g
	FbxObject3d::SetCamera(camera.get());
	//�O���t�B�b�N�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	//3D�I�u�W�F�N�g�����ƃ��f���Z�b�g
	objF = new FbxObject3d;
	objF->Initialize();
	objF->SetModel(modelF);
	
	/*--------------���x���G�f�B�^----------------*/
	// ���x���f�[�^�̓ǂݍ���
	levelData = LevelLoader::LoadJson("testScene");

	// ���f���ǂݍ���
	modelSkydome = Model::LoadFromOBJ("skydome");
	modelGround = Model::LoadFromOBJ("ground");
	modelChr = Model::LoadFromOBJ("chr_sword");
	modelSphere = Model::LoadFromOBJ("sphere");
	
	//objSkydome = Object3d::Create();
	//objGround = Object3d::Create();
	//objChr = Object3d::Create();
	//objSphere = Object3d::Create();

	//objSkydome->SetModel(modelSkydome);
	//objGround->SetModel(modelGround);
	//objChr->SetModel(modelChr);
	//objSphere->SetModel(modelSphere);
	//
	//objSkydome->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	//objGround->SetPosition(XMFLOAT3(0.0f, 0.0f, -4.8f));
	//objChr->SetPosition(XMFLOAT3(-7.5f, 0.0f, 0.0f));
	//objSphere->SetPosition(XMFLOAT3(7.0f, 0.0f, 0.0f));


	models.insert(std::make_pair("skydome", modelSkydome));
	models.insert(std::make_pair("ground", modelGround));
	models.insert(std::make_pair("chr_sword", modelChr));
	models.insert(std::make_pair("sphere", modelSphere));

	// ���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		// ���f�����w�肵��3D�I�u�W�F�N�g�𐶐�
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);
		

		// ���W
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, objectData.translation);
		newObject->SetPosition(pos);

		// ��]�p
		DirectX::XMFLOAT3 rot;
		DirectX::XMStoreFloat3(&rot, objectData.rotation);
		newObject->SetRotation(rot);

		// ���W
		DirectX::XMFLOAT3 scale;
		DirectX::XMStoreFloat3(&scale, objectData.scaling);
		newObject->SetScale(scale);

		// �z��ɓo�^
		objects.push_back(newObject);
	}

	//spriteCommon->Loadtexture(1,)
}

void GameScene::Update(){
#pragma region ���b�Z�[�W
	if (winApp->ProcessMessage()|| input->PushKey(DIK_ESCAPE)) {
		isEnd = true;
	}
#pragma endregion
	input->Update();
	//imgui�֘A
	
	//�������璆�g�������Ă���
	//�f���E�B���h�E�̕\���I��
	//ImGui::ShowDemoWindow();
	
	//------------------------------
	
	/*objSkydome->Update();
	objGround->Update();
	objChr->Update();
	objSphere->Update();*/

	objF->Update();

	/*for (auto object : objects) {
		object->Update();
	}*/

}

void GameScene::Draw(){
	//Direct���t���[�������@��������
	dxCommon->PreDraw();
	// 3D�I�u�W�F�N�g�`��O����
	Object3d::PreDraw(dxCommon->GetCommandList());
	/// <summary>
	/// ������3D�I�u�W�F�N�g�̕`�揈����ǉ��ł���
	/// </summary>
	//triangle->Draw();
	//square->Draw();
	/*for (auto object : objects) {
		object->Draw();
	}*/
	/*objSkydome->Draw();
	objGround->Draw();
	objChr->Draw();
	objSphere->Draw();*/


	// 3D�I�u�W�F�N�g�`��㏈��
	Object3d::PostDraw();
	objF->Draw(dxCommon->GetCommandList());

	//�p�[�e�B�N���`��
	
	//�X�v���C�g�`��

	//imgui
	//imguiM->Draw();

	dxCommon->PostDraw();
}

void GameScene::Finalize(){
	//�������
	winApp->Finalize();
	
	FbxLoader::GetInstance()->Finalize();
	safe_delete(objF);
	safe_delete(modelF);

	delete	input;
	delete winApp;
	delete	dxCommon;
	delete	spriteCommon;
	delete modelChr;
	delete modelGround;
	delete modelSkydome;
	delete modelSphere;
	/*delete objChr;
	delete	objSkydome;
	delete	objGround;
	delete	objSphere;*/

}