#pragma once
#include<BaseBlock.h>
#include<BulletManager.h>
#include"Map.h"

class BlockManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BulletManager* bulletManager_,Map*map_);
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
	void LineCreate(const Vector3&pos_,const Vector3 &scale_,const int16_t &w,const int16_t&h);

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

private:
	// ブロック
	std::list<std::unique_ptr<BaseBlock>> blocks;
	//弾マネージャー
	BulletManager* bulletManager;
	//マップ
	Map* map;
	//境界値
	int16_t border;
	//境界フラグ
	bool isBorder;
	//位置、回転、サイズ
	Vector3 pos,rot,scale;
};

