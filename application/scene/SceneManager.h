#pragma once
#include<BaseScene.h>
#include<memory>

class SceneManager
{
public:
	//�R���X�g���N�^
	//SceneManager();
	//�f�X�g���N�^
	~SceneManager();
	//�X�V
	void Update();
	//�`��
	void ObjDraw();
	void SpriteDraw();
	//���V�[���\��
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }

private:
	//���݂̃V�[��
	BaseScene* scene = nullptr;
	// ���̃V�[��
	BaseScene* nextScene = nullptr;
};

