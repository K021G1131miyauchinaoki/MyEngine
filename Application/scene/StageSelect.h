/**
 * @file StageSelect.h
 * @brief ベースシーンを継承したステージ選択シーン
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
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>
#include<Object3d.h>
#include<ParticleManager.h>
#include<Geometry.h>
#include<Vector2.h>
#include<array>

using namespace MyEngin;

class StageSelect : public BaseScene
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
	StageSelect();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StageSelect()override;

private:
	static const int8_t slideNum = 2;
	static const int8_t stageNum = 2;

	std::unique_ptr <Light>light;
	//操作
	std::unique_ptr <Input>input;
	//カメラ初期化
	std::unique_ptr<Camera>camera;
	//objモデル
	std::unique_ptr <Model> modelSkydome = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;

	//スプライト
	std::unique_ptr<Sprite> selectSprite;
	std::array< std::unique_ptr<Sprite>,slideNum> slide;
	std::array< std::unique_ptr<Sprite>,stageNum> stage;

	//次シーンまでのタイマー
	int waitTime;
	const int waitTimer = 20;

	//ライト
	XMVECTOR lightDir = { 25,-100,10,0 };

	//選択中のスプライトを拡大
	float scale = 1.5f;
	//
	float alpha=0.5f;
	//スプライトのサイズ
	XMFLOAT2 slideSize;
	XMFLOAT2 stageSize;
};