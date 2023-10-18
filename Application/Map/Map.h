/**
 * @file Map.h
 * @brief ステージ
 */
#pragma once
#include"Object3d.h"
#include"Model.h"
#include<Vector3.h>

class Map
{
public:
	/// <summary>
	///コンストラクタ
	/// </summary>
	Map(float value) :constStartY(value) {};
	Map() :constStartY(0.0f) {};
	/// <summary>
	///デストラクタ
	/// <summary>
	~Map();
public://静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize(Model* model_);
	/// <summary>
	/// 終了
	/// </summary>
	static void Finalize();

public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(bool isStaging_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 読み込み
	void LoadCSV(const std::string& num_);

	//準備
	void Preparation();
	//演出
	void Staging(size_t y_,size_t x_);


private://構造体
	//ブロック
	struct Block {
		std::unique_ptr<Object3d> obj;
		Vector3	pos;
		bool isUp = false;
		bool isDrow = false;
		float frame;
		float range = 0.0f;
	};
private://静的メンバ変数
	static	std::unique_ptr < Model> model;
public:
	static float moveLimitW;
	static float moveLimitH;
	static float mapScaleW;
	static float mapScaleH;
	//演出やるかやらないか用フラグ
	static bool isStaging;
private://メンバ変数

	/*イージング*/
	float endFrame;
	//位置
	float posStartY;
	float posEndY;
	//スケール
	Vector3 scaleEnd;
	Vector3 scaleStart;
	//回転
	float rotEndZ;
	float rotStartZ;
	//ブロック
	std::vector<std::vector< Block>> blocks;
	const float constStartY= -400;
	int8_t width;
	int8_t height;
	int16_t lineNum;

	const int time = 20;
	int timer;
	int16_t nowMax;
	int16_t setPoint;
	int16_t numH;
	int16_t numW;
};

