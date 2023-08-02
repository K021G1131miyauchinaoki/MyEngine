#pragma once
#include<DirectXMath.h>

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
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Updata();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public://メンバ変数
	const int8_t width = 10;
	const int8_t heith = 10;



};

