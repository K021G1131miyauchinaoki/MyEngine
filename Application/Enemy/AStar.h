#pragma once
#include"Vector2.h"
#include"Vector3.h"
#include<array>
#include<map>
#include<vector>
#include"BlockManager.h"
#include<Sprite.h>
#include<memory>
using namespace MyEngin;
struct AStarVec2
{
	//X座標値を格納する
	int32_t x{};
	//Y座標値を格納する
	int32_t y{};
	constexpr AStarVec2() noexcept = default;
	constexpr AStarVec2(const int32_t x_,const int32_t y_) noexcept :x(x_),y(y_) {}
	constexpr bool operator==(const AStarVec2& vec2_) const noexcept {
		return vec2_.x == x && vec2_.y == y;
	}
};
struct AStarNode
{
	//位置
	Vector2 pos{};
	//マップチップ位置
	AStarVec2 mapPos{};
	//親ノード
	AStarVec2 parentNode{};
	//コスト
	int32_t cost = NULL;
	//障害物フラグ
	bool isObstacle = false;
};

class AStar
{
public:
	//初期化
	void Initialize(BlockManager*blockManager_);
	//コスト、スコアのリセット
	void ResetValue();
	//スプライト描画
	void Draw();
	//推定コストの算出
	int32_t CalculateEstimate(AStarVec2 vec_);
	//スタート、エンド位置の取得
	void SetPositions(const Vector3& pos);

	int32_t Search(int32_t cost_);

	AStarNode* SetNode(AStarNode* n_,AStarVec2 mapPos_,AStarVec2 parent_,int32_t cost_);

private:

	//エンド位置のランダム取得
	void SetRondomEnd();
	//スタート位置の取得
	void SetStart(const Vector3& pos);
private://変数
//始点
	AStarVec2 startVec2{};
   //終点
	AStarVec2 endVec2{};
	std::map<int32_t,AStarNode> open{};
	std::map<int32_t,AStarNode> closed{};
	std::vector<AStarNode>root;
	std::vector<std::vector<AStarNode>>graph;
	//std::vector < std::vector < std::unique_ptr<Sprite>>> s;
	float diameter = 5.0f;
	float diameterW;
	float diameterH;

	Vector2 initialPos;
	int16_t width,height;
};


