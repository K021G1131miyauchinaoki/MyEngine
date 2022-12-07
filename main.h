#pragma once
#include"obj.h"

Object3d	player;
int	hpP;
bool	isPlayer;

Object3d	enemy;
int	hpE;
bool	isEnemy;


//シーン管理
enum Scene
{
	title,
	play,
	clear,
	over
};
int scene;

enum Movie
{
	nonMovie,	 //ムービーなし
	appearance,  //ボス登場
};
int movie;


// 線形補間
//先頭と最後に制御点を追加している
std::vector<XMFLOAT3>points;
//p1からスタートする
size_t startIndex;
XMFLOAT3	start = { 30,-14,26 };//スタート地点
XMFLOAT3	p2 = { 20, -7 , 10 };
XMFLOAT3	p3 = { 14,-4,-6 };
XMFLOAT3	p4 = { 0,3,-35 };
XMFLOAT3	end = { 0,3,-55 };//エンド地点
float timerTrans;
const	float timerTransMax = 100;
float	timeRate;

int	switchTimer;

XMFLOAT3	startTrans;
XMFLOAT3	endTrans;
const	float startFovAngle = 20.0f;
const	float endFovAngle = 45.0f;
float timerFovAngle;
const float timeFoAngleMax = 25;

//構造体
//プレイヤー
struct Obj
{
	//関数
		//初期化
	void Initialize(ID3D12Device* device, uint32_t textureHandle);

	//ループ内の初期化
	void State();

	//更新
	void Update(int	movie);

	//移動
	void Move();

	//回転
	void Rotate();

	//攻撃
	void Attack();

	//描画
	void Draw(XMMATRIX matView);

	//ワールド座標を取得
	XMFLOAT3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//半径を返す関数
	float GetRadius();

	int32_t GetHP()const { return HP_; }
	//変数

	// //ワールドトランスフォーム
	Object3d object3d_;
	// モデル
	ID3D12Device* device_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//体力
	int32_t HP_ = 3;
	//半径
	float radius = 1.0f;
};
