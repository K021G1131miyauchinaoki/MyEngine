#include "SceneManager.h"
#include<cassert>

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update()
{
	// �V�[���؂�ւ�
	if (nextScene)
	{
		// ���V�[���I��
		if (scene)
		{
			scene->Finalize();
			scene = nullptr;
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
void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	// ���̃V�[���𐶐�
	nextScene = sceneFactory->CreateScene(sceneName);
}