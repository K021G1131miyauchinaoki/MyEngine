#include "BlockManager.h"
#include"Fixedgun.h"
#include"Wall.h"
#include<random>
#include<ModelManager.h>
#include<GamePlayScene.h>

void BlockManager::Initialize(BulletManager* bulletManager_,Map* map_,EnemyManager* enemyManager_,Player* player_) {
	bulletManager = bulletManager_;
	map = map_;
	enemyManager = enemyManager_;
	player = player_;
}

void BlockManager::Update()
{
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
		b = std::make_unique<Wall>();
	}
	else if ( name_=="fixedgun" )
	{
		b = std::make_unique<Fixedgun>();
		b->SetBulletManager(bulletManager);
	}
	else
	{
		b = std::make_unique<BaseBlock>();
	}
	b->Initialize(pos_,rot_,scale_,model_);
	blocks.push_back(std::move(b));
}

void BlockManager::RandomCreate()
{
	//要素をクリア
	blocks.clear();

	scale = { 5.0f,5.0f,5.0f };
	rot = { 0.0f,0.0f,0.0f };
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
		std::unique_ptr<BaseBlock> b;
		
		std::uniform_int_distribution<int16_t> htDist(0,Map::height-1);
		std::uniform_int_distribution<int16_t> wDist(0,Map::width - 1);
		std::uniform_int_distribution<int16_t> dDist(0, 100);

		bool isOverlap = false;

		//置く位置の設定
		h = htDist(engine);
		w = wDist(engine);
		while ( h == 0 && w == 0 ||
		h == Map::height - 1 && w == Map::width - 1 )
		{
			//プレイヤーの範囲に入っていたら回し続ける
			isOverlap = true;
			while ( isOverlap )
			{
				h = htDist(engine);
				w = wDist(engine);
				//プレイヤーのH,Wから±shiftの範囲に入っていたら
				/*while ( playerH - shift <= h && playerH + shift >= h
					&& playerW - shift <= w && playerW + shift >= w )
				{
					h = hDist(engine);
					w = wDist(engine);
				}*/
				//プレイヤーに重なっていないので
				//falseにする
				isOverlap = false;
				//位置を代入する
				pos = map->GetBlocks(w,h).GetPos();
				pos.y = 5.0f;//csvなどに落とし込む
				//敵同士が一度でも重なっていたら
				for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys())
				{
					if ( enemy->GetPos().x == pos.x
					  && enemy->GetPos().z == pos.z )
					{
						isOverlap = true;
						break;
					}
				}

			}
			h = htDist(engine);
			w = wDist(engine);
		}
		
		border =dDist(engine);
		
		//あとでデータを分離する
		isBorder = border >= 40;
		if ( isBorder )
		{
			b = std::make_unique<Wall>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else
		{
			b = std::make_unique<Fixedgun>();
			model = ModelManager::GetInstance()->GetModel("fixedgun");
			b->SetBulletManager(bulletManager);
		}
		pos = map->GetBlocks(w,h).GetPos();
		//マップ端に置くように設定
		int16_t half = (Map::height - 1)/2;
		/*ｚ*/
		if ( h >half )
		{
			pos.z += scale.z;
		}
		else
		{
			pos.z -= scale.z;
		}
		/*ｘ*/
		half= ( Map::width - 1 ) / 2;
		if ( w >half)
		{
			pos.x += scale.x;
		}
		else
		{
			pos.x -= scale.x;
		}
		pos.y = 5.0f;
		//LineCreate(pos,scale,w,h);
		b->Initialize(pos,rot,scale,model);
		blocks.push_back(std::move(b));
	}
}

void BlockManager::LineCreate(const Vector3& pos_,const Vector3& scale_,const int16_t& w_,const int16_t& h_)
{

	Model* model;
	Vector3 p;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	std::uniform_int_distribution<int16_t> numDist(4,6);
	std::uniform_int_distribution<int16_t> directionDist(0,3);
	int16_t num = numDist(engine);
	int16_t direction = directionDist(engine);
	//HかWが端にある場合方向を調整
	if ( h_==Map::height-1 )direction = 0;

	if ( h_ == 0 )direction = 1;
	
	if ( w_ ==Map::width-1)direction = 2;

	if ( w_ ==0)direction = 3;
	
	std::unique_ptr<BaseBlock> b;
	for ( int16_t i = 1; i < num; i++ )
	{
		p = pos_;
		std::uniform_int_distribution<int16_t> dDist(0,100);
		border = dDist(engine);

		//あとでデータを分離する
		isBorder = border >= 20;
		if ( isBorder )
		{
			b = std::make_unique<Wall>();
			model = ModelManager::GetInstance()->GetModel("wall");
		}
		else
		{
			b = std::make_unique<Fixedgun>();
			model = ModelManager::GetInstance()->GetModel("fixedgun");
			b->SetBulletManager(bulletManager);
		}
		//下
		if ( direction == 0 )
		{
			p.z -= ( scale_.z * 2.0f ) * i;
		}
		//上
		else if ( direction == 1 )
		{
			p.z += ( scale_.z * 2.0f ) * i;
		}
		//左
		else if ( direction == 2 )
		{
			p.x -= ( scale_.z * 2.0f ) * i;
		}
		//右
		else if ( direction == 3 )
		{
			p.x += ( scale_.z * 2.0f ) * i;
		}
		//マップ外に出たら
		if ( p.x<=-Map::moveLimitW|| p.x >= Map::moveLimitW
			||p.z <= -Map::moveLimitH || p.z >= Map::moveLimitH )
		{
			continue;

		}
		b->Initialize(p,rot,scale,model);
		blocks.push_back(std::move(b));
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
