/**
 * @file SceneFactory.cpp
 * @brief シーン工場(概念)を継承したシーン工場
 */

#include "SceneFactory.h"
#include<TitleScene.h>
#include<GamePlayScene.h>
#include<GameOverScene.h>
#include<GameClear.h>
#include<StageSelect.h>
#include<SceneManager.h>
using namespace MyEngin;

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンを生成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE")
	{
		newScene = new TitleScene();
	}
	else if ( sceneName == "Select" )
	{
		newScene = new StageSelect();
	}
	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlayScene();
	}
	else if (sceneName == "GAMEOVER")
	{
		newScene = new GameOverScene();
	}
	else if (sceneName == "GAMECLEAR")
	{
		newScene = new GameClear();
	}
	return newScene;
}