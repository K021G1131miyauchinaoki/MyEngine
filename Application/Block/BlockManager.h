#pragma once
#include<BaseBlock.h>
#include<BulletManager.h>
#include"Map.h"
#include"EnemyManager.h"
#include"Player.h"
#include"ModelParticleManager.h"
#include<vector>

class BlockManager
{
private:
	struct Node
	{
		Vector3 pos;//位置
		int32_t ConnectCount;//上下左右にあるブロック数
		bool isExistLR;//左右どちらかにあれば立つ
		bool isExistUD;//上下どちらかにあれば立つ

	};
	//分岐してる場合
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BulletManager* bulletManager_,Map* map_,EnemyManager* enemyManager_,Player* player_,ModelParticleManager* particle_);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ブロックの追加
	/// </summary>
	/// <param name="name"></param>
	/// <param name="model"></param>
	/// <param name="pos"></param>
	/// <param name="rot_"></param>
	/// <param name="scale_"></param>
	void Add(const std::string name_, Model*model_,const Vector3& pos_,const Vector3& rot_,const Vector3& scale_);

	/// <summary>
	/// ランダム生成
	/// </summary>
	void RandomCreate();

	/// <summary>
	/// ライン生成
	/// </summary>
	void LineCreate(std::vector<Node>&node ,const Vector3&pos_,const Vector3 &scale_);

	// リストを取得
	std::list<std::unique_ptr<BaseBlock>>& GetBlocks() {
		return blocks;
	}

	//個数の取得
	size_t GetSize() {
		return blocks.size();
	}

	//カウント
	void Count();

	//探索
	void Search();

	//クリア
	void Clear();
private:
	std::vector<std::vector<Node>>open;
	std::vector<bool>isLimit;

	// ブロック
	std::list<std::unique_ptr<BaseBlock>> blocks;
	//弾マネージャー
	BulletManager* bulletManager;
	//マップ
	Map* map;
	EnemyManager* enemyManager;
	Player* player;
	ModelParticleManager* particle;
	//境界値
	int16_t border;
	//方向
	int16_t direction;
	//回転　
	bool isRot;
	//位置、回転、サイズ
	Vector3 pos,rot,scale;
	int16_t h,w;
	int16_t playerH,playerW,enemyH,enemyW,shift;
	float diameterW,diameterH,diameterMW,diameterMH,limitW,limitH;
	const float fixedValue = 90.0f;
};

