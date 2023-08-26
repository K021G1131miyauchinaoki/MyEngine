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

class GamePlayScene:public BaseScene
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

	//当たり判定
	void CheckAllCollision();

private:
	//インプット
	std::unique_ptr<Input>input;

	//スプライト
	//SpriteCommon* spriteCommon = nullptr;
	//カメラ初期化
	std::unique_ptr<Camera>camera;
	//スプライト


	//json読み込み
	//LevelData* levelData = nullptr;

	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> tank = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;

	//FBX
	//FbxModel* fbxM = nullptr;
	//FbxObject3d* fbxObj = nullptr;
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy>enemy;
	std::unique_ptr<Map>map;

	std::unique_ptr<ImguiManager> ImgM;

	//マップクラスの初期座標
	float mapStratY;
};

