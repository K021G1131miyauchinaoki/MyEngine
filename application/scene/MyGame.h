#pragma once
#include<vector>
#include"Framework.h"
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<Aimposition.h>
#include<ImguiManager.h>
#include<Enemy.h>
#include<Map.h>
#include<GamePlayScene.h>


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

	//当たり判定
	void CheckAllCollision();

private:
	//imgui
	std::unique_ptr<ImguiManager> ImgM;
	//プレイシーン
	GamePlayScene* scene;
};

