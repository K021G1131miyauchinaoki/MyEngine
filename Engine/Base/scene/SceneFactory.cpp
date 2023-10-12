/**
 * @file SceneFactory.cpp
 * @brief シーン工場(概念)を継承したシーン工場
 */
#include "SceneFactory.h"
#include<TitleScene.h>
#include<GamePlayScene.h>
#include<GameOverScene.h>
#include<GameClear.h>
#include<SceneManager.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンを生成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE")
	{
		SceneManager::sceneNum = SceneManager::title;
		newScene = new TitleScene();
	}
	else if (sceneName == "GAMEPLAY")
	{
		SceneManager::sceneNum = SceneManager::play;
		newScene = new GamePlayScene();
	}
	else if (sceneName == "GAMEOVER")
	{
		SceneManager::sceneNum = SceneManager::over;
		newScene = new GameOverScene();
	}
	else if (sceneName == "GAMECLEAR")
	{
		SceneManager::sceneNum = SceneManager::clear;
		newScene = new GameClear();
	}
	return newScene;
}