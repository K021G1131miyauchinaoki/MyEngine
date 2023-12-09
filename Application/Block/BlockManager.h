#pragma once
#include<BaseBlock.h>

class BlockManager
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
	private:
	// プレイヤーの弾
	std::list<std::unique_ptr<BaseBlock>> blocks;
};

