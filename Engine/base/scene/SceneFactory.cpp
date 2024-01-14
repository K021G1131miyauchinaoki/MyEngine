/**
 * @file SceneFactory.cpp
 * @brief シーン工場(概念)を継承したシーン工場
 */

#include "SceneFactory.h"
#include"scene.h"
#include<SceneManager.h>
using namespace MyEngin;

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンを生成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE")
	{
		SceneManager::sceneNum = SceneManager::title;
		newScene = new scene();
	}
	else
	{
		SceneManager::sceneNum = SceneManager::title;
		newScene = new scene();
	}
	return newScene;
}