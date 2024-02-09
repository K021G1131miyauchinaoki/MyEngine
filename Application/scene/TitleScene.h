/**
 * @file TitleScene.h
 * @brief ベースシーンを継承したタイトルシーン
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
#include<Map.h>
#include<BaseScene.h>
#include<array>
#include"Vector3.h"
#include"Light.h"

class TitleScene:public BaseScene
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
	/// 暗転
	/// </summary>
	void BlackOutStaging();

	/// <summary>
	/// movieCountの加算
	/// </summary>
	static void AddMovieCount();

	static int8_t movieCount;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene()override;

	//カメラワーク
	enum Staging
	{
		CameraFirst,
		CameraSecond,
		CameraThird,
		Title,

	};

private:
	//ライト
	std::unique_ptr <Light>light;
	XMVECTOR lightDir = { 0.73f,-30.0f,-0.21f,0.0f };
	//操作
	Input* input = nullptr;;
	//カメラ初期化
	std::unique_ptr<Camera>camera;
	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> body = nullptr;
	std::unique_ptr <Model> had = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;

	std::unique_ptr<Player> player;
	std::unique_ptr<Map>map;

	//スプライト
	std::unique_ptr<Sprite> titleSprite;
	std::unique_ptr<Sprite> blackOutSprite;
	std::unique_ptr<Sprite> pushKey;

	//シェイク
	Vector3 shake;
	//ムービー
	int32_t movieTime;
	const int32_t movieTimer = 600;
	bool isMovie;

	//暗転
	float transTime;
	const float transTimer=1.0f;
	float alpha;
	bool isFadeOut;
	bool isFadeIn;

	//次シーンまでのタイマー
	int waitTime;
	const int waitTimer = 20;

	//点滅
	int8_t flashTime;
	const int8_t flashTimer = 45;
	bool isFlash;

};

