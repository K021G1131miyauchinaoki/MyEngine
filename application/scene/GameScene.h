#pragma once
#include<vector>
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include "ParticleManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include"Framework.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"FbxModel.h"


class GameScene:public Framework
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
	// WindowsAPI
	WinApp* winApp = nullptr;
	//DirectX
	DirectXCommon* dxCommon = nullptr;
	//キー
	Input* input = nullptr;
	//スプライト
	SpriteCommon* spriteCommon = nullptr;
	//カメラ初期化
	std::unique_ptr<Camera>camera = std::make_unique<Camera>();
	//スプライト
		
	//モデル

	//json読み込み
	LevelData* levelData = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelChr = nullptr;
	Model* modelSphere = nullptr;

	/*Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objChr = nullptr;
	Object3d* objSphere = nullptr;*/

	std::map<std::string, Model*> models;

	std::vector<Object3d*> objects;

	Sprite* s = nullptr;
	//FBX
	//FbxModel* fbxM = nullptr;
	//FbxObject3d* fbxObj = nullptr;
	
	//ゲーム終了フラグ
	bool isEnd = false;
};

