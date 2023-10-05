#pragma once
#include<vector>
#include"Framework.h"
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include<ImguiManager.h>
#include<BaseScene.h>

class MyGame:public Framework
{
public://メンバ関数
	//初期化
	void Initialize()override;
	//終了
	void Finalize()override;
	//更新
	void Update()override;
	//描画
	void Draw()override;

private:
	//imgui
	std::unique_ptr<ImguiManager> ImgM;
};

