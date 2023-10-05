﻿#pragma once
#include<BaseScene.h>
#include<string>
/// <summary>
/// シーン工場(概念)
/// </summary>
class AbstractSceneFactory
{
public:
	//仮想デストラクタ
	virtual ~AbstractSceneFactory() = default;
	//シーン生成
	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;
};

