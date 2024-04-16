#include "AStar.h"
#include"Map.h"
#include"Vector3.h"
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
			graph[ i ][ j ].pos.x = initialPos.x + ( ( diameter * 2.0f ) * j );
			graph[ i ][ j ].pos.y = initialPos.y + ( ( diameter * 2.0f ) * i );
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
{	for 
( int i = 0; i < height; ++i )
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
