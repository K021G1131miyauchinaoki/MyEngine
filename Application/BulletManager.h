#pragma once
#include"Vector3.h"
#include<list>
#include<memory>
#include<Bullet.h>
#include<EnemyBullet.h>

class BulletManager
{
public:
	/// <summary>
	///　初期化
	/// </summary>
	void Initialize();

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
	/// <param name="model"></param>
	/// <param name="pipeline_"></param>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed"></param>
	void PlayerBulletShot(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_);

	/// <summary>
	/// 敵の弾発射
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="vec_"></param>
	/// <param name="rot_"></param>
	/// <param name="bulletSpeed"></param>
	void EnemyBulletShot(const Vector3& pos,const Vector3& vec_,const Vector3& rot_);

	// インスタンスゲッター
	static BulletManager* GetInstance();

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
	//KModel* playersBulletModel = nullptr;
	std::unique_ptr<Model> playersBulletModel = nullptr;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	std::unique_ptr<Model> enemysBulletModel = nullptr;

	// インスタンス
	static BulletManager* bulletManager;
};

