/**
 * @file EnemyManager.h
 * @brief エネミーマネージャー
 */

#pragma once
#include"BaseEnemy.h"
#include<list>
#include<memory>
#include<Vector3.h>
#include<BulletManager.h>
class EnemyManager
{
public:
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
	void Add(const std::string& name_,Model* model_,Player* player_,const Vector3& pos_,const Vector3& rot_,BulletManager*bulletManager_);

	//弾リストを取得
	std::list<std::unique_ptr<BaseEnemy>>& GetEnemys() {
		return enemys;
	}

	size_t GetSize() {return enemys.size();}

private:
	std::list<std::unique_ptr<BaseEnemy>>enemys;
};

