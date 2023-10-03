#pragma once
#include<BaseScene.h>
#include<memory>
#include<AbstractSceneFactory.h>

class SceneManager
{
public:
	//�X�V
	void Update();
	//�`��
	void ObjDraw();
	void SpriteDraw();
	//���V�[���\��
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }
	//�V�[���t�@�N�g���[�̃Z�b�^�[
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }
	
	void ChangeScene(const std::string& sceneName);

	//�C���X�^���X����
	static SceneManager* GetInstance();

	//�i��j���@���^�C�g�����Ǝ���,�v���C���͔C�ӂňړ����ł���悤�ɊǗ�����
	static int8_t sceneNum;
	enum
	{
		title,
		select,
		play,
		clear,
		over,
	};
	
private:
	//���݂̃V�[��
	BaseScene* scene = nullptr;
	// ���̃V�[��
	BaseScene* nextScene = nullptr;
	//�V�[���t�@�N�g���[
	AbstractSceneFactory* sceneFactory = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
};

