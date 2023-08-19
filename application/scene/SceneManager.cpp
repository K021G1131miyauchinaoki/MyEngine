#include "SceneManager.h"
void SceneManager::Update()
{
	// �V�[���؂�ւ�
	if (nextScene)
	{
		// ���V�[���I��
		if (scene)
		{
			scene->Finalize();
			delete scene;
		}

		// �V�[���؂�ւ�
		scene = nextScene;
		nextScene = nullptr;
		scene->SetSceneManager(this);
		scene->Initialize();
	}

	// ���s���̃V�[���X�V
	scene->Update();
}

void SceneManager::SpriteDraw()
{
	scene->SpriteDraw();
}

void SceneManager::ObjDraw()
{
	scene->ObjDraw();
}

SceneManager::SceneManager(){}

SceneManager::~SceneManager()
{
	scene->Finalize();
	scene = nullptr;
	delete scene;
}