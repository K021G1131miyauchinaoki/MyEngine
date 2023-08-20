#include "Framework.h"

void Framework::Initialize(){
	winApp.reset(WinApp::GetInstance());
	winApp->Initialize();

	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	Input::GetInstance()->Initialize();
	input .reset(Input::GetInstance());
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	//�O���t�B�b�N�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	//�f�o�C�X���Z�b�g
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//�|�X�g�G�t�F�N�g
	PostEffect::StaticInitialize(dxCommon.get());
	//�X�v���C�g�R����
	SpriteCommon::GetInstance()->Initialize(dxCommon.get());
	SpriteCommon::GetInstance()->Loadtexture(1, "reimu.png");

	sceneManager = new SceneManager();	
}

void Framework::Update(){
	input->Update();
	//sceneManager->Update();
}

void Framework::Draw(){}

void Framework::Finalize(){
	winApp->Finalize();
	delete sceneManager;
	/*FbxLoader::GetInstance()->Finalize();
	FbxObject3d::Finalize();
	Model::Finalize();
	Object3d::Finalize();*/
}

bool Framework::IsEndRequst() {
	if (winApp->ProcessMessage() || input->PushKey(DIK_ESCAPE)) {
		return true;
	}
	return false;
}

void Framework::Run() {
	//������
	Initialize();

	//���[�v
	while (true) {
		//���t���[���X�V
		Update();
		//�I�����N�G�X�g�������甲����
		if (IsEndRequst())
		{
			break;
		}

		//�`��
		Draw();
	}
	//�Q�[���I��
	Finalize();
}