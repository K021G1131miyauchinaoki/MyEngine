#include "SceneFactory.h"
#include<TitleScene.h>
#include<GamePlayScene.h>

BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇê∂ê¨
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE")
	{
		newScene = new TitleScene();
	}
	else if (sceneName == "GAMEPLAY")
	{
		newScene = new GamePlayScene();
	}
	else if (sceneName == "GAMEOVER")
	{
		newScene = new GamePlayScene();
	}
	else if (sceneName == "GAMECLEAR")
	{
		newScene = new GamePlayScene();
	}
	return newScene;
}