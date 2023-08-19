#include "SceneManager.h"
void SceneManager::Update()
{
	// シーン切り替え
	if (nextScene)
	{
		// 旧シーン終了
		if (scene)
		{
			scene->Finalize();
			delete scene;
		}

		// シーン切り替え
		scene = nextScene;
		nextScene = nullptr;
		scene->SetSceneManager(this);
		scene->Initialize();
	}

	// 実行中のシーン更新
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