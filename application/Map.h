#pragma once
#include<DirectXMath.h>
#include<Object3d.h>
#include<Model.h>
#include<Vector3.h>


class Map
{
public:
	//コンストラクタ
	Map();
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
private://構造体
	//ブロック
	struct Block {
		std::unique_ptr<Object3d> obj;
		Vector3	pos;
	};
private://静的メンバ変数
	static const int8_t width = 10;
	static const int8_t heith = 10;

private://メンバ変数
	Vector3 scale;
	Block block[heith][width];

};

