#include "Framework.h"

void Framework::Initialize(){
	winApp = std::make_unique<WinApp>();
	winApp->Initialize();

	dxCommon = std::make_unique<DirectXCommon>();
	dxCommon->Initialize(winApp.get());

	input = std::make_unique<Input>();
	input->Initialize(winApp.get());
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDevice());
	//�f�o�C�X���Z�b�g
	FbxObject3d::SetDevice(dxCommon->GetDevice());
	//�O���t�B�b�N�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();
	//�f�o�C�X���Z�b�g
	Object3d::StaticInitialize(dxCommon->GetDevice());
	//�|�X�g�G�t�F�N�g
	PostEffect::StaticInitialize(dxCommon.get());

}

void Framework::Update(){
	input->Update();
}

void Framework::Draw(){}

void Framework::Finalize(){
	winApp->Finalize();
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