#include "BlockManager.h"
#include"Fixedgun.h"
#include"Wall.h"
#include"BreakBlock.h"
#include<random>
#include<ModelManager.h>
#include<GamePlayScene.h>

void BlockManager::Initialize(BulletManager* bulletManager_,Map* map_,EnemyManager* enemyManager_,Player* player_) {
	bulletManager = bulletManager_;
	map = map_;
	enemyManager = enemyManager_;
	player = player_;
	shift = 1;
}

void BlockManager::Update()
{
	//フラグが立ったら削除
	blocks.remove_if([ ] (std::unique_ptr<BaseBlock>& e)
{	return e->IsDead();
	});

	for (std::unique_ptr<BaseBlock>&block:blocks)
	{
		block->Update();
	}
    Count();
}

void BlockManager::Draw() {
	for (std::unique_ptr<BaseBlock>&block:blocks)
	{
		block->Draw();
	}
}

void BlockManager::Add(const std::string name_,Model* model_,const Vector3& pos_,const Vector3& rot_,const Vector3& scale_) {
	std::unique_ptr<BaseBlock> b;
	if ( name_=="block" )
	{
		b = std::make_unique<BreakBlock>();
	}
	else if ( name_=="fixedgun" )
	{
		b = std::make_unique<Fixedgun>();
		b->SetBulletManager(bulletManager);
	}
	else
	{
		b = std::make_unique<BreakBlock>();
	}
	b->Initialize(pos_,rot_,scale_,model_);
	blocks.push_back(std::move(b));
}

void BlockManager::RandomCreate()
{
	//要素をクリア
	blocks.clear();
	diameterW = Map::mapScaleW * 2.0f;
	diameterH = Map::mapScaleH * 2.0f;
	playerW = static_cast< int16_t >( ( player->GetPos().x + Map::moveLimitW ) / diameterW );
	playerH = static_cast< int16_t >( ( player->GetPos().z + Map::moveLimitH ) / diameterH );

	scale = { 5.0f,5.0f,5.0f };
	int16_t h,w;
	Model* model;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//std::uniform_real_distribution<float> rotDist(-shift, shift);
	std::uniform_int_distribution<int16_t> numDist(3,3);
	int16_t num = numDist(engine);
	for ( int16_t i = 0; i < num; i++ )
	{
		rot = { 0.0f,0.0f,0.0f };
		std::unique_ptr<BaseBlock> b;
		
		std::uniform_int_distribution<int16_t> htDist(0,Map::height-1);
		std::uniform_int_distribution<int16_t> wDist(0,Map::width - 1);
		std::uniform_int_distribution<int16_t> dDist(0, 100);
		std::uniform_int_distribution<int16_t> directionDist(0,3);
		std::uniform_int_distribution<int16_t> rotDist(0,1);
		
		bool isOverlap = false;
		h = w = 0;
		//方向の設定
		direction = directionDist(engine);
		

		//プレイヤーの範囲に入っていたら回し続ける
		isOverlap = true;
		while ( isOverlap )
		{
			//置く位置の設定
			h = htDist(engine);
			w = wDist(engine);
			//四隅にいたら
			while ( ( h == 0 && w == 0 ) ||							//左下
					( h == 0 && w == Map::width - 1 ) ||			//右下
					( h == Map::height - 1 && w == 0) ||			//左上
					(h == Map::height - 1 && w == Map::width - 1) )	//右上
			{
				h = htDist(engine);
				w = wDist(engine);
			}
			//プレイヤーのH,Wと同じなら
			while ( playerH== h && playerW== w )
			{
				h = htDist(engine);
				w = wDist(engine);
			}
			//プレイヤーに重なっていないので
			//falseにする
			isOverlap = false;
			//位置を代入する
			pos = map->GetBlocks(w,h).GetPos();
			pos.y = 5.0f;//csvなどに落とし込む
			//ブロック端に置くように設定
			int16_t half = ( Map::height - 1 ) / 2;
			/*ｚ*/
			if ( h > half )pos.z += scale.z;

			else pos.z -= scale.z;
			/*ｘ*/
			half = ( Map::width - 1 ) / 2;
			if ( w > half )
			{
				pos.x += scale.x;
			}
			else
			{
				pos.x -= scale.x;
			}
			//敵に一度でも重なっていたら
			for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys())
			{
				enemyW = static_cast< int16_t >( ( enemy->GetPos().x + Map::moveLimitW ) / diameterW );
				enemyH = static_cast< int16_t >( ( enemy->GetPos().z + Map::moveLimitH ) / diameterH );
				if ( enemyW == w
				  && enemyH == h )
				{
					isOverlap = true;
					break;
				}
			}
			for ( std::unique_ptr<BaseBlock>& block : blocks )
			{
				if ( block->GetPos().x == pos.x
					  && block->GetPos().z == pos.z )
				{
					isOverlap = true;
					break;
				}
			}
		}
		
		//HかWが端にある場合方向を調整
		if ( h == Map::height - 1 )direction = 0;
		if ( h == 0 )direction = 1;		
		if ( w == Map::width - 1 )direction = 2;
		if ( w == 0 )direction = 3;
		
		border =dDist(engine);
		
		//あとでデータを分離する
		if ( border >= 40 )
		{
			b = std::make_unique<Wall>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else if ( border >= 20 )
		{
			b = std::make_unique<BreakBlock>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else
		{
			b = std::make_unique<Fixedgun>();
			model = ModelManager::GetInstance()->GetModel("fixedgun");
			b->SetBulletManager(bulletManager);
			//上下
			if ( direction <= 1 )rot.y=90.0f;
			//左右
			else if ( direction >= 2 )rot.y=0.0f;

			//回転
			isRot = rotDist(engine);
			if ( isRot )rot.y += fixedValue;
			else rot.y -= fixedValue;

		}
		
		LineCreate(pos,scale);
		b->Initialize(pos,rot,scale,model);
		blocks.push_back(std::move(b));
	}
}

void BlockManager::LineCreate(const Vector3& pos_,const Vector3& scale_)
{

	Model* model;
	Vector3 p;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	std::uniform_int_distribution<int16_t> numDist(4,6);
	std::uniform_int_distribution<int16_t> rotDist(0,1);

	int16_t num = numDist(engine);
	bool isOverlap = false;
	
	
	std::unique_ptr<BaseBlock> b;
	for ( int16_t i = 1; i < num; i++ )
	{
		isOverlap = false;
		p = pos_;
		std::uniform_int_distribution<int16_t> dDist(0,100);
		border = dDist(engine);

		//あとでマジックナンバーを分離する
		if ( border >= 20 )
		{
			b = std::make_unique<Wall>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else if ( border >= 10 )
		{
			b = std::make_unique<BreakBlock>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else
		{
			b = std::make_unique<Fixedgun>();
			model = ModelManager::GetInstance()->GetModel("fixedgun");
			b->SetBulletManager(bulletManager);
			//回転
			//上下
			if ( direction <= 1 )rot.y = 90.0f;
			//左右
			else if ( direction >= 2 )rot.y = 0.0f;
			isRot = rotDist(engine);
			if ( isRot )rot.y += fixedValue;
			else rot.y -= fixedValue;
		}
		//下
		if ( direction == 0 )p.z -= ( scale_.z * 2.0f ) * i;
		//上
		else if ( direction == 1 )p.z += ( scale_.z * 2.0f ) * i;
		//左
		else if ( direction == 2 )p.x -= ( scale_.z * 2.0f ) * i;
		//右
		else if ( direction == 3 )p.x += ( scale_.z * 2.0f ) * i;
		
		int16_t blockW = static_cast< int16_t >( ( p.x + Map::moveLimitW ) / diameterW );
		int16_t blockH = static_cast< int16_t >( ( p.z + Map::moveLimitH ) / diameterH );
		//プレイヤーのH,Wと同じなら
		if( playerH == blockH&& playerW == blockW)
		{
			isOverlap = true;
		}
		//敵が一度でも重なっていたらfor文を抜ける
		for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys() )
		{
			enemyW = static_cast< int16_t >( ( enemy->GetPos().x + Map::moveLimitW ) / diameterW );
			enemyH = static_cast< int16_t >( ( enemy->GetPos().z + Map::moveLimitH ) / diameterH );
			if ( enemyW == blockW&& enemyH == blockH )
			{
				isOverlap = true;
				break;
			}
		}
		//ブロック同士が一度でも重なっていたらfor文を抜ける
		for ( std::unique_ptr<BaseBlock>& block : blocks)
		{
			if ( block->GetPos().x == p.x
				  && block->GetPos().z == p.z )
			{
				isOverlap = true;
				break;
			}
		}
		//マップ外に出ていたら生成をやめる
		if ( p.x<=-Map::moveLimitW|| p.x >= Map::moveLimitW
			||p.z <= -Map::moveLimitH || p.z >= Map::moveLimitH )
		{
			continue;

		}
		//重なっていなければ追加
		if ( !isOverlap )
		{
			b->Initialize(p,rot,scale,model);
			blocks.push_back(std::move(b));
		}
	}
	
}

void BlockManager::Count()
{
	if (GamePlayScene::isOut)
	{
		int8_t count=0;
		for ( std::unique_ptr<BaseBlock>& block : blocks )
		{
			if ((block->GetScale().x<=0.0f&&block->GetScale().y <= 0.0f&&block->GetScale().z <= 0.0f&& GamePlayScene::outCount==GamePlayScene::RollOut )||
				(block->GetScale().x >= scale.x && block->GetScale().y >= scale.y && block->GetScale().z >= scale.z&& GamePlayScene::outCount == GamePlayScene::RollIn ))
			{
				count++;
			}
		}
		if ( count == blocks.size() )
		{
			GamePlayScene::outCount++;
		}
	}
}
