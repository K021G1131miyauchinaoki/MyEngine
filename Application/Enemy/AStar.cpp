#include "AStar.h"
#include"Map.h"
#include"Vector3.h"
#include<SpriteCommon.h>
#include<random>

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
			graph[ i ][ j ].cost = graph[ i ][ j ].estimateCost = graph[ i ][ j ].score = NULL;
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
}
