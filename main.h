#pragma once
#include"obj.h"

Object3d	player;
int	hp;

//シーン管理
enum Scene
{
	title,
	play,
	clear,
	over
};
int scene;

//構造体
//プレイヤー
struct Player
{

	//変数
	int32_t HP_ = 3;
	//半径
	float radius = 1.0f;
};
//エネミー
struct Enemy
{
	//関数
	/// 生成
	void Initialize(ID3D12Device* device, uint32_t textureHandle);

	/// 更新
	void Update(int movie);

	/// 描画
	void Draw(XMMATRIX matView);

	//ループ内の初期化
	void	State();

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//半径を返す関数
	float GetRadius();

	//敵弾デリート
	void	BulletDelete();

	int32_t GetHP()const { return HP_; }


	//変数
	// //ワールドトランスフォーム
	Object3d object3d_;
	// モデル
	ID3D12Device* device_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	 
	//速度
	XMFLOAT3 velocity_ = { 0.0f,0.0f,0.1f };

	//体力
	int32_t HP_ = 20;

	//半径
	float radius = 15.0f;
};
