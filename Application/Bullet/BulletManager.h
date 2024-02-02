/*
*@file BulletManager.h
* @brief 弾マネージャー
*/
#pragma once
#include"Vector3.h"
#include<list>
#include<memory>
#include<Bullet.h>
#include<EnemyBullet.h>
#include<Model.h>

//前方宣言
class Player;
namespace MyEngin
{
	class BillboardParticle;
}

using namespace MyEngin;
class BulletManager
{
public:
	/// <summary>
	///　初期化
	/// </summary>
	void Initialize(Model*model_,Player*player_,BillboardParticle* geometry_);

	/// <summary>
	/// 弾の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 弾の描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーの弾発射
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	void PlayerBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_);

	/// <summary>
	/// 敵の弾発射
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	void EnemyBulletCreate(const Vector3& pos,const Vector3& vec_,const Vector3& rot_,const std::string type_);

	// 存命の弾を全部削除
	void AllBulletDelete();

	// 自弾のリストを取得
	std::list<std::unique_ptr<Bullet>>& GetPlayerBullets() {
		return playerBullets;
	}

	// 敵弾のリストを取得
	std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullets() {
		return enemyBullets;
	}

private:
	// プレイヤーの弾
	std::list<std::unique_ptr<Bullet>> playerBullets;
	std::unique_ptr<Model> playerBulletModel = nullptr;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	std::unique_ptr<Model> enemyBulletModel = nullptr;

	//プレイヤー
	Player* player;

	//パーティクル
	BillboardParticle* billParticle;

	XMFLOAT4 startColor,endColor;
	XMFLOAT3 pos,vec,accel;
	int8_t life = 20;
	float startScale,endScale;
};

