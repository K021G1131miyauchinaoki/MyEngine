#pragma once
#include<DirectXMath.h>
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>

class Map
{
public:
	//コンストラクタ
	Map(float value) :constStartY(value) {};
	//デストラクタ
	~Map();
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*model_);

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector3 get() { return block[1][1].obj->GetPosition(); }

private://構造体
	//ブロック
	struct Block {
		std::unique_ptr<Object3d> obj;
		Vector3	pos;
		bool isUp = false;
		float frame;
		float y = 0.0f;
	};
private://静的メンバ変数
	static const int8_t width = 5;
	static const int8_t heith = 7;

private://メンバ変数
	//イージング
	float endFrame;
	float startY;
	float endY;
	//スケール
	Vector3 scale;
	//ブロック
	Block block[heith][width];
	int16_t num;
	const float constStartY;

	const int time = 60;
	int timer = 0;
	int16_t nowMax;
	int16_t setPoint;
	int16_t numH;
	int16_t numW;
};

