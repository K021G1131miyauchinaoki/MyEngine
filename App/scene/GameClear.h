/**
 * @file GameClear.h
 * @brief ベースシーンを継承したゲームクリアシーン
 */

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

class GameClear :public BaseScene
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

	

private:
	struct Score
	{
		std::unique_ptr<Sprite> sprite;
		float easeTime;
	};

	XMFLOAT4 clearColor;
	XMFLOAT2 startClearSize;
	XMFLOAT2 endClearSize;
	XMFLOAT2 startScoreSize;
	XMFLOAT2 endScoreSize;
	
	//スプライト
	std::unique_ptr<Sprite> clearSprite;
	std::vector<Score>score;
	
	//操作
	std::unique_ptr <Input>input;
	//ライト
	std::unique_ptr <Light>light;
	XMVECTOR lightDir = { -250.0f,-350.0f,20.0f,0.0f };
	//カメラ初期化
	std::unique_ptr<Camera>camera;
	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> body = nullptr;
	std::unique_ptr <Model> had = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;
	std::unique_ptr <Object3d> tankBody = nullptr;
	std::unique_ptr <Object3d> tankHad = nullptr;

	std::unique_ptr<Map>map;

	//画角
	float startFovAngle;
	float endFovAngle;
	float fovAngleTime;
	const float fovAngleTimer=30;
	//クリアスプライト
	float clearTime;
	const float clearTimer = 30;
	const float alphaMax = 1.0f;
	const float alphaMin = 0.0f;
	//スコア
	const float easeTimer = 30;
	float startRot;
	float endRot;
	//次シーンまでのタイマー
	int waitTime;
	const int waitTimer = 20;
};