#pragma once
#include"Vector2.h"
#include<array>
#include<list>
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
	AStarVec2 pos{};
	//親ノード
	AStarVec2 parentNode{};
	//距離
	int32_t distance{};
	//推定コスト
	int32_t estimateCost = 0;
	//実コスト
	int32_t cost{};
	//障害物フラグ
	bool isObstacle = false;
};

class AStar
{
public:
	void Initialize(BlockManager*blockManager_);
	void Draw();
private://変数
//始点
	Vector2 startVec2{};
   //終点
	Vector2 endVec2{};
	std::list<AStarNode> open{};
	std::list<AStarNode> closed{};
	std::vector<std::vector<AStarNode>>graph;
	//std::vector < std::vector < std::unique_ptr<Sprite>>> s;
	float diameter = 5.0f;
	Vector2 initialPos;
	int16_t width,height;
};


