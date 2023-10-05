#pragma once
#include<memory>

class SceneManager;

class BaseScene
{
public:
	virtual ~BaseScene() = default;
	//������
	virtual void Initialize();
	//�I��
	virtual	void Finalize();
	//�X�V
	virtual	void Update();
	//�`��
	virtual	void ObjDraw();
	virtual	void SpriteDraw();
	//�V�[���}�l�[�W���[���Z�b�g
	virtual void SetSceneManager(SceneManager* sceneManager_) { sceneManager = sceneManager_; }

protected:
	SceneManager* sceneManager = nullptr;	
};

