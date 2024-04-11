#include "AStar.h"
#include"Map.h"
#include<SpriteCommon.h>
void AStar::Initialize(BlockManager* blockManager_)
{
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
	s.resize(height);
	for ( int i = 0; i < height; ++i )
	{
		graph[ i ].resize(width);
		s[ i ].resize(width);
	}
	//フラグのオンオフ設定
	for ( int i = 0; i < height; ++i )
	{
		for ( int j = 0; j < width; ++j )
		{
			for ( const std::unique_ptr<BaseBlock>& block : blockManager_->GetBlocks() )
			{
				//ブロックの位置に
				if ( initialPos.x + ( diameter * j ) == block->GetPos().x &&
					initialPos.y + ( diameter * i ) == block->GetPos().z )
				{
					graph[ i ][ j ].isObstacle = true;
				}
			}
			s[ i ][ j ] = std::make_unique<Sprite>();
			s[ i ][ j ]->Initialize(SpriteCommon::GetInstance(),5);
			
		}
	}
}

void AStar::Draw()
{
}
