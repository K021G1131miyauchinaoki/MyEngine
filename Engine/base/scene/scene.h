#pragma once
#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"Vector2.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include"BaseScene.h"
#include<LevelLoader.h>
#include<list>
#include<Geometry.h>
#include<Light.h>
#include<Model.h>
#include<Object3d.h>

using namespace MyEngin;

class scene :public BaseScene
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
	/// コンストラクタ
	/// </summary>
	scene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~scene()override;

private:
	
};