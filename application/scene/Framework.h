#pragma once
#include<Input.h>
#include<WinApp.h>
#include<DirectXCommon.h>
#include<SpriteCommon.h>
#include<PostEffect.h>
#include<Model.h>
#include<Object3d.h>
#include<FbxObject3d.h>
#include<FbxLoader.h>
#include<FbxModel.h>
#include<memory>

class Framework
{
public://�����o�֐�
	virtual ~Framework() = default;
	//������
	virtual void Initialize();
	//�I��
	virtual	void Finalize();
	//�X�V
	virtual	void Update();
	//�`��
	virtual	void Draw() = 0;
	//���s
	void Run();
	//�I���`�F�b�N
	virtual	bool IsEndRequst();
protected:
	std::unique_ptr<Input>input;
	std::unique_ptr<WinApp>winApp;
	std::unique_ptr<DirectXCommon>dxCommon;
	//std::unique_ptr<SpriteCommon>spriteCom;
	std::unique_ptr<PostEffect>postEffect;

};

