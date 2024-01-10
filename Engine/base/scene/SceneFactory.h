/**
 * @file SceneFactory.h
 * @brief シーン工場(概念)を継承したシーン工場
 */

#pragma once
#include<AbstractSceneFactory.h>
namespace MyEngin
{
	class SceneFactory :public AbstractSceneFactory
	{
	public:
		/// <summary>
		/// シーン生成
		/// </summary>
		/// <param name="sceneName">シーン名</param>
		/// <returns>生成したシーン</returns>
		BaseScene* CreateScene(const std::string& sceneName)override;
	};

}