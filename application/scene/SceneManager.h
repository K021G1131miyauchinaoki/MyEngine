#pragma once
#include<BaseScene.h>
#include<memory>
#include<GamePlayScene.h>
#include<TitleScene.h>
#include<DirectxCommon.h>
#include<Input.h>

class SceneManager
{
public:
	//�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();
	//�X�V
	void Update();
	//�`��
	void ObjDraw();
	void SpriteDraw();
	//���V�[���\��
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }
	
	void SetDxCommon(DirectXCommon* dxCommon_) { dxCommon.reset(dxCommon_); }
	void SetInput(Input* input_) { input.reset(input_); }

private:
	//���݂̃V�[��
	BaseScene* scene = nullptr;
	// ���̃V�[��
	BaseScene* nextScene = nullptr;

	std::unique_ptr<DirectXCommon>dxCommon;
	std::unique_ptr<Input>input;


};

