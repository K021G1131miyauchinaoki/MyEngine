#pragma once
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
#include"FbxLoader.h"
#include"LevelLoader.h"
#include<vector>

class GameScene
{
public://メンバ関数
	//初期化
	void Initialize();
	//終了
	void Finalize();
	//更新
	void Update();
	//描画
	void Draw();
	//終了フラグのチェック
	bool IsEnd() { return isEnd; }
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

	//3dオブジェクト生成
	
	//imguiクラス

	//wav読み込み
	
	//パーティクル
	
	//ゲーム終了フラグ
	bool isEnd = false;
};

