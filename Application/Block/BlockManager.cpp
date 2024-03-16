#include "BlockManager.h"
#include"Fixedgun.h"
#include"Wall.h"
#include<random>
#include<ModelManager.h>

void BlockManager::Initialize(BulletManager* bulletManager_,Map* map_) {
	bulletManager = bulletManager_;
	map = map_;
}

void BlockManager::Update()
{
	for (std::unique_ptr<BaseBlock>&block:blocks)
	{
		block->Update();
	}
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
	Vector3 pos,rot,scale;
	int16_t h,w;
	Model* model;
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//std::uniform_real_distribution<float> rotDist(-shift, shift);
	std::uniform_int_distribution<int16_t> numDist(3,10);
	int16_t num = numDist(engine);
	for ( int16_t i = 0; i < num; i++ )
	{
		std::unique_ptr<BaseBlock> b;
		
		std::uniform_int_distribution<int16_t> htDist(0,Map::height-1);
		std::uniform_int_distribution<int16_t> wDist(0,Map::width - 1);
		std::uniform_int_distribution<int16_t> dDist(0, 100);
		h = htDist(engine);
		w = wDist(engine);
		

		while ( h == 0 && w == 0 ||
		h == Map::height - 1 && w == Map::width - 1 )
		{
			h = htDist(engine);
			w = wDist(engine);
		}
		
		int16_t divide=dDist(engine);
		bool isDivide;
		//あとでデータを分離する
		if ( divide>=40 )
		{
			isDivide = true;
		}
		else
		{
			isDivide = false;
		}
		if ( isDivide )
		{
			b = std::make_unique<Wall>();
			model = ModelManager::GetInstance()->GetModel(ModelData::wall);
		}
		else
		{
			b = std::make_unique<Fixedgun>();
			model = ModelManager::GetInstance()->GetModel(ModelData::fixedgun);
			b->SetBulletManager(bulletManager);
		}
		scale = { 5.0f,5.0f,5.0f };
		rot = { 0.0f,0.0f,0.0f };
		pos = map->GetBlocks(w,h).GetPos();
		//マップ端に置くように設定
		/*ｚ*/
		if ( h==0 )
		{
			pos.z -= scale.z;
		}
		if ( h == Map::height - 1 )
		{
			pos.z += scale.z;
		}
		/*ｘ*/
		if ( w == 0 )
		{
			pos.x -= scale.x;
		}
		if ( w == Map::width - 1 )
		{
			pos.x += scale.x;
		}
		pos.y = 5.0f;
		b->Initialize(pos,rot,scale,model);
		blocks.push_back(std::move(b));
	}
}
