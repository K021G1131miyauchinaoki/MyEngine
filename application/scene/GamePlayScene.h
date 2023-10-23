/**
 * @file GamePlayScene.h
 * @brief ベースシーンを継承したプレイシーン
 */
#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"Vector2.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>
#include<ParticleManager.h>

class GamePlayScene:public BaseScene
{
public://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;

	/// <summary>
	/// obj描画
	/// </summary>
	void ObjDraw()override;

	/// <summary>
	/// スプライト描画
	/// </summary>
	void SpriteDraw()override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize()override;

	/// <summary>
	/// 当たり判定
	/// </summary>
	void CheckAllCollision();

	/// <summary>
	/// スタート演出
	/// </summary>
	void StartStaging();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GamePlayScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GamePlayScene()override;

public:
	//スタート演出のカウント
	static int8_t startCount;
	//スタートフラグ
	static  bool isStart;

private:
	//インプット
	std::unique_ptr<Input>input;
	//カメラ
	std::unique_ptr<Camera>camera;
	float cameraY;
	//スプライト
	std::unique_ptr <Sprite>stage = nullptr;
	std::unique_ptr <Sprite>ready = nullptr;
	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> tank = nullptr;
	std::unique_ptr <Model> parachute = nullptr;
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
	//パーティクルマネージャー
	ParticleManager* particle;
	//スプライトイージング
	float spriteEaseTime;
	const float spriteEaseTimer = 30;
	float spriteWaitTime;
	const float spriteWaitTimer = 60;
	float rPosStartY;
	float sPosStartY;
	float rPosEndY;
	float sPosEndY;
};

