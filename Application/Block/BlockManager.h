#pragma once
#include<BaseBlock.h>
#include<BulletManager.h>

class BlockManager
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(BulletManager* bulletManager_);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// プレイヤーの弾発射
	/// </summary>
	/// <param name="name"></param>
	/// <param name="model"></param>
	/// <param name="pos"></param>
	/// <param name="rot_"></param>
	/// <param name="scale_"></param>
	void Add(const std::string name_, Model*model_,const Vector3& pos_,const Vector3& rot_,const Vector3& scale_);

	// リストを取得
	std::list<std::unique_ptr<BaseBlock>>& GetBlocks() {
		return blocks;
	}

	//個数の取得
	size_t GetSize() {
		return blocks.size();
	}


private:
	// ブロック
	std::list<std::unique_ptr<BaseBlock>> blocks;
	//弾マネージャー
	BulletManager* bulletManager;
};

