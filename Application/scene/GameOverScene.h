/**
 * @file GameOverScene.h
 * @brief ベースシーンを継承したゲームオーバーシーン
 */

#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<ImguiManager.h>
#include<Map.h>
#include<BaseScene.h>
#include<Object3d.h>
#include<BillboardParticle.h>
#include<ModelManager.h>

class GameOverScene :public BaseScene
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
	/// コンストラクタ
	/// </summary>
	GameOverScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameOverScene()override;

private:
	
	std::unique_ptr <Light>light;
	//操作
	Input* input = nullptr;
	//カメラ初期化
	std::unique_ptr<Camera>camera;
	//obj
	std::unique_ptr <Object3d> objSkydome = nullptr;
	std::unique_ptr <Object3d> tankBody = nullptr;
	std::unique_ptr <Object3d> tankHad = nullptr;
	//モデルマネージャー
	ModelManager* modelM;
	//マップ
	std::unique_ptr<Map>map;
	//パーティクル
	std::unique_ptr < BillboardParticle> particle;

	//スプライト
	std::unique_ptr<Sprite> overSprite;
	std::unique_ptr<Sprite> pushKey;

	//次シーンまでのタイマー
	int waitTime;
	const int waitTimer = 20;

	//ライト
	XMVECTOR lightDir = { 25,-100,10,0 };

	//パーティクル変数
	XMFLOAT3 particlePos;
	size_t num;
	//点滅
	int8_t flashTime;
	const int8_t flashTimer = 45;
	bool isFlash;
};