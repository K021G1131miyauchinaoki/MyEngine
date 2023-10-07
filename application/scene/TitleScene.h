#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>

class TitleScene:public BaseScene
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

	TitleScene();
	~TitleScene()override;

private:
	//スプライト
	std::unique_ptr<Sprite> titleSprite;
	//操作
	std::unique_ptr <Input>input;
	//カメラ初期化
	std::unique_ptr<Camera>camera;
	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> tank = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;

	std::unique_ptr<Player> player;
	std::unique_ptr<Map>map;

	std::unique_ptr<ImguiManager> ImgM;
};

