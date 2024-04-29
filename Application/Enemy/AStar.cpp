#include "AStar.h"
#include"Map.h"
#include"Vector3.h"
#include<SpriteCommon.h>
#include<random>

#define GX	(5)
#define GY	(5)

#define SX	(1)
#define SY	(1)

//縦横99まで対応
#define	KEY(x,y) (x*100+y)
#define	KEYDATA(x,y,n) std::make_pair((x*100+y),n)

void AStar::Initialize(BlockManager* blockManager_)
{
	diameterW = diameter * 2.0f;
	diameterH = diameter * 2.0f;

	initialPos.x = -Map::moveLimitW + diameter;
	initialPos.y = -Map::moveLimitH + diameter;
	width = Map::width * 2;
	height = Map::height * 2;
	//クリアする
	for ( auto& innerVec : graph )
	{
		innerVec.clear();
	}
	graph.clear();

	// 二次元配列のサイズを初期化
	graph.resize(height);
	//s.resize(height);
	for ( int i = 0; i < height; ++i )
	{
		graph[ i ].resize(width);
		//s[ i ].resize(width);
	}
	//フラグのオンオフ設定
	for ( int i = 0; i < height; ++i )
	{
		for ( int j = 0; j < width; ++j )
		{
			graph[ i ][ j ].pos.x = initialPos.x + ( diameterW * j );
			graph[ i ][ j ].pos.y = initialPos.y + ( diameterH * i );
			//重なっているかの処理
			for ( const std::unique_ptr<BaseBlock>& block : blockManager_->GetBlocks() )
			{
				Vector2 pos;
				//float shift = 1.0f;
				pos = graph[ i ][ j ].pos;
				//ブロックの位置に
				if ( pos.x == block->GetPos().x &&
					 pos.y == block->GetPos().z )
				{
					//重なってたらbreakする
					graph[ i ][ j ].isObstacle = true;
					break;
				}
			}

			/*s[ i ][ j ] = std::make_unique<Sprite>();
			s[ i ][ j ]->Initialize(SpriteCommon::GetInstance(),5);
			if ( graph[ i ][ j ].isObstacle )
			{
				s[ i ][ j ]->SetColor({ 0.0f,0.0f, 0.0f, 0.0f });
			}
			float size = 10.0f;
			s[ i ][ j ]->SetSize({ size,size });
			float size2 = 12.0f;
			
			s[ i ][ j ]->SetPosition({10.0f+( size2 *j ),400.0f - ( size2 * i ) });
			s[ i ][ j ]->Update();*/
		}
	}
	//値のリセット
	ResetValue();
}

void AStar::ResetValue()
{	for ( int i = 0; i < height; ++i )
	{
		for ( int j = 0; j < width; ++j )
		{
			graph[ i ][ j ].cost = NULL;
		}
	}

}

void AStar::Draw()
{
	/*for ( int i = 0; i < height; ++i )
	{
		for ( int j = 0; j < width; ++j )
		{
			s[ i ][ j ]->Draw();
		}
	}*/
}

int32_t AStar::CalculateEstimate(AStarVec2 vec_)
{
	int32_t x = endVec2.x - vec_.x;
	int32_t y = endVec2.y - vec_.y;
	return ( x >= y ) ? x : y;
}

void AStar::SetPositions(const Vector3& pos)
{
	SetStart(pos);
	SetRondomEnd();
}

int32_t AStar::Search(int32_t cost_)
{
	//openが空なら抜ける
	if ( open.size() == 0 )return -2;
	std::map<int32_t,AStarNode>::iterator it = open.begin();
	std::map<int32_t,AStarNode>::iterator itMin;
	//最小コスト
	int32_t costMin=9999;
	//移動コスト
	//int32_t moveCost = 0;
	while ( it!=open.end() )
	{
		//openの中から最小コストを算出
		if ( costMin> CalculateEstimate(it->second.mapPos))
		{
			costMin = CalculateEstimate(it->second.mapPos);
			itMin = it;
		}
		it++;
	}

	int32_t shift[ 8 ][ 2 ] =
	{
		{-1,-1},
		{-1, 0},
		{-1, 1},
		{ 0,-1},
		{ 0, 1},
		{ 1,-1},
		{ 1, 0},
		{ 1, 1}
	};
	AStarNode nodes[ 8 ];
	for ( size_t i = 0; i < 8; i++ )
	{

	}

	return Search(cost_+1);
}

AStarNode* AStar::SetNode(AStarNode* n_,AStarVec2 mapPos_,AStarVec2 parent_,int32_t cost_)
{
	n_->mapPos = mapPos_;
	n_->parentNode = parent_;
	n_->cost = cost_;
	return n_;
}

void AStar::SetRondomEnd()
{
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	std::uniform_int_distribution<int32_t> wDist(0,Map::width - 1);
	std::uniform_int_distribution<int32_t> hDist(0,Map::height - 1);

	endVec2.x = wDist(engine);
	endVec2.y = hDist(engine);
	//isObstacleがいないところが出るまで回す
	while ( graph[ endVec2.y ][ endVec2.x ].isObstacle )
	{
		endVec2.x = wDist(engine);
		endVec2.y = hDist(engine);
	}
}

void AStar::SetStart(const Vector3& pos)
{
	startVec2.x= static_cast< int32_t >( ( pos.x + Map::moveLimitW ) / diameterW );
	startVec2.y = static_cast< int32_t >( ( pos.z + Map::moveLimitH ) / diameterH );
	//親ノードを参照できない値に
	AStarVec2 p = { -1,-1 };
	AStarNode n;
	SetNode(&n,startVec2,p,0);
	open.insert(KEYDATA(n.mapPos.x,n.mapPos.y,n));
}