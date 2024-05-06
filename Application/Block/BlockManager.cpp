#include "BlockManager.h"
#include"Fixedgun.h"
#include"Wall.h"
#include"BreakBlock.h"
#include<random>
#include<ModelManager.h>
#include<GamePlayScene.h>

void BlockManager::Initialize(BulletManager* bulletManager_,Map* map_,EnemyManager* enemyManager_,Player* player_,ModelParticleManager* particle_) {
	bulletManager = bulletManager_;
	map = map_;
	enemyManager = enemyManager_;
	player = player_;
	particle = particle_;
	shift = 1;
}

void BlockManager::Update()
{
	for (std::unique_ptr<BaseBlock>&block:blocks)
	{
		block->Update();
		if ( block->IsDead() )
		{
			particle->Add("explosion",10,20,block->GetPos(),2.0f,0.0f);
		}
	}
	//フラグが立ったら削除
	blocks.remove_if([ ] (std::unique_ptr<BaseBlock>& e)
{	return e->IsDead();
	});
	
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
		b = std::make_unique<Wall>();
	}
	else if ( name_=="fixedgun" )
	{
		b = std::make_unique<Fixedgun>();
		b->SetBulletManager(bulletManager);
	}
	else
	{
		b = std::make_unique<Wall>();
	}
	b->Initialize(pos_,rot_,scale_,model_);
	blocks.push_back(std::move(b));
}

void BlockManager::RandomCreate()
{
	//要素をクリア
	blocks.clear();
	diameterMW = Map::mapScaleW * 2.0f;
	diameterMH = Map::mapScaleH * 2.0f;
	playerW = static_cast< int16_t >( ( player->GetPos().x + Map::moveLimitW ) / diameterMW );
	playerH = static_cast< int16_t >( ( player->GetPos().z + Map::moveLimitH ) / diameterMH );

	scale = { 5.0f,5.0f,5.0f };

	Model* model;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//std::uniform_real_distribution<float> rotDist(-shift, shift);
	std::uniform_int_distribution<int16_t> numDist(3,3);
	std::uniform_int_distribution<int16_t> htDist(0,Map::height - 1);
	std::uniform_int_distribution<int16_t> wDist(0,Map::width - 1);
	std::uniform_int_distribution<int16_t> dDist(0,100);
	std::uniform_int_distribution<int16_t> directionDist(0,3);
	std::uniform_int_distribution<int16_t> rotDist(0,1);

	int16_t num = numDist(engine);
	open.resize(num);
	isLimit.resize(num);
	for ( int16_t i = 0; i < num; i++ )
	{
		rot = { 0.0f,0.0f,0.0f };
		std::unique_ptr<BaseBlock> b;

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
					( h == Map::height - 1 && w == 0 ) ||			//左上
					( h == Map::height - 1 && w == Map::width - 1 ) )	//右上
			{
				h = htDist(engine);
				w = wDist(engine);
			}
			//プレイヤーのH,Wと同じなら
			while ( playerH == h && playerW == w )
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
			if ( w > half )pos.x += scale.x;
			else pos.x -= scale.x;
			
			//敵に一度でも重なっていたら
			for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys() )
			{
				enemyW = static_cast< int16_t >( ( enemy->GetPos().x + Map::moveLimitW ) / diameterMW );
				enemyH = static_cast< int16_t >( ( enemy->GetPos().z + Map::moveLimitH ) / diameterMH );
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

			//HかWが端にある場合方向を調整
			if ( h == Map::height - 1 )direction = 0;
			if ( h == 0 )direction = 1;
			if ( w == Map::width - 1 )direction = 2;
			if ( w == 0 )direction = 3;

			border = dDist(engine);

		//あとでデータを分離する
		if ( border >= 40 )
		{
			b = std::make_unique<Wall>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else if ( border >= 20 )
		{
			b = std::make_unique<BreakBlock>();
			model = ModelManager::GetInstance()->GetModel("BreakBlock");
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

			LineCreate(open[ i ],pos,scale);
			open[ i ][ 0 ].pos = pos;
			b->Initialize(pos,rot,scale,model);
			blocks.push_back(std::move(b));
		}

	}
	Search();
}

void BlockManager::LineCreate(std::vector<Node>& node,const Vector3& pos_,const Vector3& scale_)
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
	node.resize(num);
	bool isOverlap = false;
	bool isSameOver = false;
	
	std::unique_ptr<BaseBlock> b;
	for ( int16_t i = 1; i < num; i++ )
	{
		isOverlap = false;
		isSameOver = false;
		p = pos_;
		std::uniform_int_distribution<int16_t> dDist(0,100);
		border = dDist(engine);

		b = std::make_unique<Wall>();
		model = ModelManager::GetInstance()->GetModel("wall");
		//あとでマジックナンバーを分離する
		if ( border >= 20 )
		{
		}
		else if ( border >= 10 )
		{
			b = std::make_unique<BreakBlock>();
			model = ModelManager::GetInstance()->GetModel("BreakBlock");
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
		
		int16_t blockW = static_cast< int16_t >( ( p.x + Map::moveLimitW ) / diameterMW );
		int16_t blockH = static_cast< int16_t >( ( p.z + Map::moveLimitH ) / diameterMH );
		//プレイヤーのH,Wと同じなら
		if( playerH == blockH&& playerW == blockW)
		{
			isOverlap = true;
		}
		//敵が一度でも重なっていたらfor文を抜ける
		for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys() )
		{
			enemyW = static_cast< int16_t >( ( enemy->GetPos().x + Map::moveLimitW ) / diameterMW );
			enemyH = static_cast< int16_t >( ( enemy->GetPos().z + Map::moveLimitH ) / diameterMH );
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
				isSameOver = true;
				break;
			}
		}
		//マップ外かisSameOverが立ってたら生成をやめる
		if (( p.x<=-Map::moveLimitW||
			p.x >= Map::moveLimitW||
			p.z <= -Map::moveLimitH ||
			p.z >= Map::moveLimitH)||
			isSameOver)
		{
			node[ i ].pos = { NULL,NULL,NULL};
			continue;
		}
		//重なっていなければ追加
		if ( !isOverlap )
		{
			node[ i ].pos = p;
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

void BlockManager::Search()
{
	int32_t value = 0;
	int32_t count=0;
	diameterW = scale.x*2.0f;
	diameterH = scale.z*2.0f;
	const int32_t borderCount = 2;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	limitH = Map::moveLimitH - scale.z;
	limitW = Map::moveLimitW - scale.x;
	for ( size_t i = 0; i < open.size(); i++ )
	{
		for ( size_t j = 0; j < open[i].size(); j++ )
		{
			if ( (open[ i ][ j ].pos.x == limitW || open[ i ][ j ].pos.x == -limitW) ||
				( open[ i ][ j ].pos.z == limitH || open[ i ][ j ].pos.z == -limitH ) )
			{
				count++;
			}
		}
	}
	int32_t num[ 4 ][ 2 ]{
		{-1, 0},//下
		{ 0,-1},//左
		{ 0, 1},//右
		{ 1, 0},//上
	};

	//線同士が繋がっていたら
	if ( count >= borderCount )
	{
		for ( size_t i = 0; i < open.size(); i++ )
		{
			for ( size_t j = 0; j < open.size(); j++ )
			{
				if ( i == j )continue;
				for ( size_t k = 0; k < open[ i ].size(); k++ )
				{
					//nullなら抜ける
					if ( open[ i ][ k ].pos.x == NULL &&
						open[ i ][ k ].pos.y == NULL &&
						open[ i ][ k ].pos.z == NULL )continue;

					for ( size_t m = 0; m < 4; m++ )
					{
						pos = open[ i ][ k ].pos;
						pos.z = open[ i ][ k ].pos.z + ( diameterH * num[ m ][ 0 ] );
						pos.x = open[ i ][ k ].pos.x + ( diameterW * num[ m ][ 1 ] );
						for ( size_t l = 0; l < open[ j ].size(); l++ )
						{
							if ( open[ j ][ l ].pos.x == pos.x &&
								 open[ j ][ l ].pos.y == pos.y &&
								 open[ j ][ l ].pos.z == pos.z )
							{
								open[ j ][ l ].ConnectCount++;
								for ( size_t n = 0; n < open[j].size(); n++ )
								{
									if ( n == l )continue;
									if (open[ j ][ n ].pos.x == limitW ||
										open[ j ][ n ].pos.x == -limitW||
										open[ j ][ n ].pos.z == limitH ||
										open[ j ][ n ].pos.z == -limitH  )
									{
										isLimit[ j ] = true;
									}
								}
							}
						}
					}
					
				}
			}
		}
	}

	for ( size_t i = 0; i < open.size(); i++ )
	{
		if ( isLimit[i] )
		{
			//要素を算出
			std::uniform_int_distribution<int32_t> numDist(0,static_cast<int32_t>( open[ i ].size() - 1));
			value = numDist(engine);
			while ( open[ i ][ value ].ConnectCount > 1 )
			{
				value = numDist(engine);
			}
			pos = open[ i ][ value ].pos;

			//要素の削除
			std::list<std::unique_ptr<BaseBlock>>::iterator it = blocks.begin();
			while ( it != blocks.end() )
			{
				if ( it->get()->GetPos().x == pos.x &&
					 it->get()->GetPos().y == pos.y &&
					 it->get()->GetPos().z == pos.z )
				{
					it= blocks.erase(it);
					//要素の追加
					std::unique_ptr<BaseBlock> b=std::make_unique<BreakBlock>();
					b->Initialize(pos,rot,scale,ModelManager::GetInstance()->GetModel("BreakBlock"));
					blocks.insert(blocks.begin(), std::move(b));
				}
				else
				{
					++it;
				}
			}
		}
	}
	//クリア
	for ( size_t i = 0; i < open.size(); i++ )
	{
		open[ i ].clear();
	}
}
