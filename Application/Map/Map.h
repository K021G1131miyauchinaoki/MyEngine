/**
 * @file Map.h
 * @brief ステージ
 */

#pragma once
#include"Object3d.h"
#include"Model.h"
#include<Vector3.h>
#include<BaseBlock.h>

class Map
{
public:
	/// <summary>
	///コンストラクタ
	/// </summary>
	Map();
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

	BaseBlock& GetBlocks(const int32_t&w,const int32_t& h) {
		return blocks[h][w];
	}
	
private://静的メンバ変数
	static	std::unique_ptr < Model> model;
public:
	//移動の上限
	static float moveLimitW;
	static float moveLimitH;
	//スケール
	static float mapScaleW;
	static float mapScaleH;
	//演出やるかやらないか用フラグ
	static bool isStaging;
	static int16_t width;
	static int16_t height;
private://メンバ変数

	/*イージング*/
	//スケール
	Vector3 scaleEnd;
	Vector3 scaleStart;
	float endFrame;
	//位置
	float posStartY;
	float posEndY;
	//回転
	float rotEndZ;
	float rotStartZ;
	//ブロック
	std::vector<std::vector< BaseBlock>> blocks;
	const float constStartY = -400;
	
	int16_t lineNum;

	const int16_t stagingTimer = 50;
	const int16_t provisionTimer = 20;
	int16_t provisionTime = 0;
	int16_t nowMax;
	int16_t setPoint;
	int16_t numH;
	int16_t numW;
	bool change;
	bool flag;
	int8_t count;
};

