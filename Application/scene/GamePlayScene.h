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
#include<Map.h>
#include<BaseScene.h>
#include<ModelParticleManager.h>
#include<LevelLoader.h>
#include<list>
#include "EnemyManager.h"
#include"BlockManager.h"
#include"BulletManager.h"
#include<BillboardParticle.h>
#include<string>

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

	//ジオメトリ描画
	void GeometryDraw()override;

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
	/// 操作UIの表示
	/// </summary>
	void MemoDisplay();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GamePlayScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GamePlayScene()override;

	//スタート演出
	enum start
	{
		Down,
		Bound1,
		Bound2,
		Wait,
		Redy,
		Go,
	};

	//ステージの文字列を取得
	void StageString();
	
public:
	//スタート演出のカウント
	static int8_t startCount;
	//スタートフラグ
	static  bool isStart;

private:
	//インプット
	Input* input = nullptr;
	//カメラ
	std::unique_ptr<Camera>camera;
	//ライト
	std::unique_ptr <Light>light;
	XMVECTOR lightDir = { 0.73f,-30.0f,-0.21f,0.0f };
	//スプライト
	std::unique_ptr <Sprite>stage = nullptr;
	std::unique_ptr <Sprite>ready = nullptr;
	std::unique_ptr <Sprite>sight = nullptr;
	std::unique_ptr <Sprite>memo = nullptr;
	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> body = nullptr;
	std::unique_ptr <Model> had = nullptr;
	std::unique_ptr <Model> tank = nullptr;
	std::unique_ptr <Model> parachute = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;
	std::unique_ptr <Model> fixedgun = nullptr;
	std::unique_ptr <Model> wall = nullptr;
	std::unique_ptr <Model> bullet = nullptr;
	std::unique_ptr <Model> under = nullptr;

	//ジオメトリ
	std::unique_ptr <BillboardParticle> geo = nullptr;
	//天球
	std::unique_ptr <Object3d> objSkydome = nullptr;
	std::unique_ptr <Object3d> obj = nullptr;
	//プレイヤー
	std::unique_ptr<Player> player;
	//敵
	std::unique_ptr<EnemyManager>enemyManager;
	//壁
	std::unique_ptr<BlockManager>blockManager;
	//床
	std::unique_ptr<Map>map;
	//弾
	std::unique_ptr<BulletManager>bulletManager;
	//外部ファイルの読み込み
	std::unique_ptr< LevelData>jsonLoader;
	std::map<std::string, Model*> models;

	//パーティクルマネージャー
	std::unique_ptr<ModelParticleManager> particle;

	//スプライトイージング
	float spriteEaseTime;
	const float spriteEaseTimer = 30;
	float spriteWaitTime;
	const float spriteWaitTimer = 60;
	float waitTime;
	const float waitTimer = 100;
	float rPosStartY;
	float sPosStartY;
	float mPosStartX;
	float rPosEndY;
	float sPosEndY;
	float mPosEndX;
	//カメラのy座標
	const float cameraY=100.0f;

	//操作UIフラグ
	bool isDisplay = false;
	bool isSlide = false;
	//ステージの文字列
	std::string stageStr;
};

