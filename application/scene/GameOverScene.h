#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<ImguiManager.h>
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>

class GameOverScene :public BaseScene
{
public://メンバ関数

//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void ObjDraw()override;
	void SpriteDraw()override;

	//終了
	void Finalize()override;

	GameOverScene();
	~GameOverScene()override;

private:
	std::unique_ptr<Sprite> overSprite;
	std::unique_ptr <Input>input;

};