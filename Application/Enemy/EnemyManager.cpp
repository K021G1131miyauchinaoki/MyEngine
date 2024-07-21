#include "EnemyManager.h"
#include"NormalEnemy.h"
#include"ShotgunEnemy.h"
#include<random>
#include<ModelManager.h>
#include"Player.h"
#include"GamePlayScene.h"
#include"BulletManager.h"

void EnemyManager::Initialize(Player* player_,BulletManager* bulletManager_)
{
	player = player_;
	parachuteModel = ModelManager::GetInstance()->GetModel("parachute");
	bulletManager = bulletManager_;
	mSpeed = 1.0f;
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
	if ( name_ == "normal" )
	{
		e = std::make_unique <NormalEnemy>();
		BulletManager::eBulletCount++;
	}
	else if(name_=="shotgun" )
	{
		e = std::make_unique<ShotgunEnemy>();
		BulletManager::eBulletCount+=3;
	}
	else
	{
		e = std::make_unique <NormalEnemy>();
	}
	e->Initialize(model_,parachuteModel,player,pos_,rot_,bulletManager);
	e->SetMotionSpeed(mSpeed);
	enemys.push_back(std::move(e));
}

void EnemyManager::RandomCreate(Map* map_)
{
	enemys.clear();
	std::string name= "Normal";//後でランダムに
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	std::uniform_int_distribution<int16_t> hDist(0,Map::height - 1);
	std::uniform_int_distribution<int16_t> wDist(0,Map::width - 1);
	std::uniform_int_distribution<int16_t> numDist(1,5);
	std::uniform_int_distribution<int16_t> nu(0,100);

	bool isOverlap = false;
	int16_t num = numDist(engine);
	Vector3 pos,rot;
	
	if ( GamePlayScene::gimmickCount>=Gimmick::Shotgun )
	{
		nums = 50;
	}
	else
	{
		nums = 0;
	}

	//プレイヤーがマップ上のどの位置にいるかを調べる
	float diameterW = Map::mapScaleW * 2.0f;
	float diameterH = Map::mapScaleH * 2.0f;
	int16_t playerW,playerH,shift;
	playerW = static_cast< int16_t >( ( player->GetPos().x + Map::moveLimitW ) / diameterW );
	playerH = static_cast< int16_t >( ( player->GetPos().z + Map::moveLimitH ) / diameterH );
	shift = 1;

	//num分敵を生成する
	for ( size_t i = 0; i < num; i++ )
	{
		std::unique_ptr <BaseEnemy> e;
		int16_t h;
		int16_t w;
		n = nu(engine);
		if ( n>nums )
		{
			name = "Normal";
		}
		else
		{
			name = "Shotgun";
		}
		//敵の種類は後でランダムにする
		if ( name == "Normal" )
		{
			e = std::make_unique <NormalEnemy>();
			BulletManager::eBulletCount++;
		}
		else if ( name == "Shotgun" )
		{
			e = std::make_unique<ShotgunEnemy>();
			BulletManager::eBulletCount+=3;
		}
		else
		{
			e = std::make_unique <NormalEnemy>();
			BulletManager::eBulletCount++;
		}
		rot = { 0.0f,0.0f,0.0f };
		//プレイヤーの範囲に入っていたら回し続ける
		isOverlap = true;
		while (isOverlap)
		{
			h = hDist(engine);
			w = wDist(engine);
			//プレイヤーのH,Wから±shiftの範囲に入っていたら
			while( playerH-shift <= h&& playerH + shift >= h
				&& playerW-shift <= w&& playerW + shift >= w )
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
		e->Initialize(ModelManager::GetInstance()->GetModel("enemy"),parachuteModel,player,pos,rot,bulletManager);
		e->SetMotionSpeed(mSpeed);
		enemys.push_back(std::move(e));
	}
	

}

void EnemyManager::SetMotionSpeed(const float& mSpeed_)
{
	for ( std::unique_ptr<BaseEnemy>& enemy : enemys )
	{
		enemy->SetMotionSpeed(mSpeed_);
	}
}

