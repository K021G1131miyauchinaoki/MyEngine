#include "EnemyManager.h"
#include"NormalEnemy.h"
#include"ShotgunEnemy.h"
#include<random>
#include<ModelManager.h>
#include"Player.h"

void EnemyManager::Initialize(Player* player_,BulletManager* bulletManager_)
{
	player = player_;
	parachuteModel = ModelManager::GetInstance()->GetModel(ModelData::parachute);
	bulletManager = bulletManager_;
}

void EnemyManager::Update() {
	//フラグが立ったら削除
	enemys.remove_if([ ] (std::unique_ptr<BaseEnemy>& e){
	 return e->IsDead();
	});

	for ( std::unique_ptr<BaseEnemy>& e : enemys )
	{
		e->Update();
	}
}

void EnemyManager::Draw() {
	for ( std::unique_ptr<BaseEnemy>& e : enemys )
	{
		e->Draw();
	}
}

void EnemyManager::Add(const std::string& name_,Model* model_,const Vector3& pos_,const Vector3& rot_) {

	std::unique_ptr <BaseEnemy> e;
	if ( name_ == "Normal" )
	{
		e = std::make_unique <NormalEnemy>();
	}
	else if(name_=="Shotgun" )
	{
		e = std::make_unique<ShotgunEnemy>();
	}
	else
	{
		e = std::make_unique <NormalEnemy>();
	}
	e->Initialize(model_,parachuteModel,player,pos_,rot_,bulletManager);

	enemys.push_back(std::move(e));
}

void EnemyManager::RandomCreate(Map* map_)
{
	std::string name= "Normal";//後でランダムに
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	std::uniform_int_distribution<int16_t> hDist(0,Map::height - 1);
	std::uniform_int_distribution<int16_t> wDist(0,Map::width - 1);
	std::uniform_int_distribution<int16_t> numDist(1,5);

	bool isOverlap = false;
	int16_t num = numDist(engine);
	Vector3 pos,rot;

	//プレイヤーがマップ上のどの位置にいるかを調べる
	float diameterW = Map::mapScaleW * 2.0f;
	float diameterH = Map::mapScaleH * 2.0f;
	int16_t playerW,playerH;
	playerW = static_cast< int16_t >( ( player->GetPos().x + Map::moveLimitW ) / diameterW );
	playerH = static_cast< int16_t >( ( player->GetPos().z + Map::moveLimitH ) / diameterH );

	//num分敵を生成する
	for ( size_t i = 0; i < num; i++ )
	{
		std::unique_ptr <BaseEnemy> e;
		int16_t h;
		int16_t w;
		//敵の種類は後でランダムにする
		if ( name == "Normal" )
		{
			e = std::make_unique <NormalEnemy>();
		}
		else if ( name == "Shotgun" )
		{
			e = std::make_unique<ShotgunEnemy>();
		}
		else
		{
			e = std::make_unique <NormalEnemy>();
		}
		rot = { 0.0f,0.0f,0.0f };
		//プレイヤーの範囲に入っていたら回し続ける
		isOverlap = true;
		while (isOverlap)
		{
			h = hDist(engine);
			w = wDist(engine);
			while( playerH == h && playerW == w )
			{
				h = hDist(engine);
				w = wDist(engine);
			}
			//プレイヤーに重なっていないので
			//falseにする
			isOverlap = false;
			//位置を代入する
			pos = map_->GetBlocks(w,h).GetPos();
			pos.y = 5.0f;//csvなどに落とし込む
			//敵同士が一度でも重なっていたら
			for ( std::unique_ptr<BaseEnemy>& enemy : enemys )
			{
				if ( enemy->GetPos().x == pos.x
				  && enemy->GetPos().z == pos.z )
				{
					isOverlap = true;
					break;
				}
			}
			
		}
		e->Initialize(ModelManager::GetInstance()->GetModel(ModelData::enemy),parachuteModel,player,pos,rot,bulletManager);
		enemys.push_back(std::move(e));
	}
	

}
