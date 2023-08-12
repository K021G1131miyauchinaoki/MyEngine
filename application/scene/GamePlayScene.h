#pragma once
#include<Input.h>
#include<DirectXCommon.h>
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


class GamePlayScene
{
public://メンバ関数

	//初期化
	void Initialize(DirectXCommon*dxCommon,Input*input);

	//更新
	void Update();

	//描画
	void ObjDraw();
	void SpriteDraw();

	//終了
	void Finalize();
private:
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
	std::unique_ptr<Aimposition> aim;
	std::unique_ptr<Enemy>enemy;
	std::unique_ptr<Map>map;

	std::unique_ptr<ImguiManager> ImgM;

	//マップクラスの初期座標
	float mapStratY;
};

