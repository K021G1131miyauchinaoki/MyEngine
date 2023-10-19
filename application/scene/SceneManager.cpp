/**
 * @file SceneManager.cpp
 * @brief シーンマネージャー
 */
#include "SceneManager.h"
#include"SceneTransition.h"
#include<cassert>

int8_t SceneManager::sceneNum = SceneManager::title;

SceneManager* SceneManager::GetInstance() {
	static SceneManager instance;
	return &instance;
}

void SceneManager::Update()
{
	// シーン切り替え
	if (nextScene)
	{
		// 旧シーン終了
		if (scene)
		{
			scene->Finalize();
			scene = nullptr;
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
void SceneManager::ChangeScene(const std::string& sceneName_)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	// 次のシーンを生成
	nextScene = sceneFactory->CreateScene(sceneName_);
}