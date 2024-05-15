/**
 * @file EnemyManager.h
 * @brief エネミーマネージャー
 */

#pragma once
#include"BaseEnemy.h"
#include"Vector3.h"
#include<list>
#include<memory>
#include<Model.h>
#include<Map.h>
#include"BulletManager.h"

class EnemyManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Player* player_,BulletManager* bulletManager_);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 加算
	/// </summary>
	void Add(const std::string& name_,Model* model_,const Vector3& pos_,const Vector3& rot_);

	//リストを取得
	std::list<std::unique_ptr<BaseEnemy>>& GetEnemys() {
		return enemys;
	}

	size_t GetSize() {return enemys.size();}


	void RandomCreate(Map*map_);

	//モーションタイマーをセット　デフォルトで1.0f
	void SetMotionSpeed(const float& mSpeed_ = 1.0f);

private:
	std::list<std::unique_ptr<BaseEnemy>>enemys;
	Player* player;
	Model* parachuteModel;
	BulletManager* bulletManager;
	float mSpeed;
};

