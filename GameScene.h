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
	Sprite* mario = nullptr;
	Sprite* flies = nullptr;
	
	//モデル
	Model* model = nullptr;
	Model* model2 = nullptr;
	//3dオブジェクト生成
	Object3d* triangle = nullptr;
	Object3d* square = nullptr;
	//imguiクラス
	ImguiManager* imguiM = nullptr;
	//変数
	float position[2] = { 100,100 };

	//wav読み込み
	SoundManager* audio = nullptr;
	
	//パーティクル
	ParticleManager* particle = nullptr;
	//ゲーム終了フラグ
	bool isEnd = false;
};

