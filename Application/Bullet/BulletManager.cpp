#include "BulletManager.h"
#include"Homing.h"
#include"ParticleManager.h"

void BulletManager::Initialize(Model* model_,Player*player_,ParticleManager* particleManager_) {
	playerBulletModel.reset(model_);
	enemyBulletModel.reset(model_);
	player = player_;
	particleManager = particleManager_;
}

void BulletManager::Update() {
	//要素の削除
	enemyBullets.remove_if([ ] (std::unique_ptr<EnemyBullet>& enemyBullet)
		{
		return	enemyBullet->IsDead();
	});
	playerBullets.remove_if([ ] (std::unique_ptr<Bullet>& playerBullet)
		{
			return playerBullet->IsDead();
	});

	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets )
	{
		playerBullet->Update();
		//particleManager->Add("orbit",1,20,playerBullet->GetPos(),1.0f,0.0f,{ 0.0f,0.5f,0.0f });
	}

	for ( std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets )
	{
		enemyBullet->Update();
		//particleManager->Add("orbit",1,20,enemyBullet->GetPos(),1.0f,0.0f,{ 0.0f,0.5f,0.0f });

	}
}

void BulletManager::Draw(){
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets )
	{
		playerBullet->Draw();
	}

	for ( std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets )
	{
		enemyBullet->Draw();
	}
}

void BulletManager::PlayerBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_){
	// 弾生成
	std::unique_ptr<Bullet> newBullet=std::make_unique<Bullet>();
	newBullet->Initialize(playerBulletModel.get(),pos_,vec_,rot_);
	// 登録
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_,const std::string type_){
	// 弾生成
	std::unique_ptr<EnemyBullet> newBullet;
	if ( type_=="normale" )
	{
		newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_);
	}
	else if(type_=="homing" )
	{
		newBullet = std::make_unique<Homing>();
		newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_,player);
	}
	else
	{
		newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_);
	}
	// 登録
	enemyBullets.push_back(std::move(newBullet));
}


void BulletManager::AllBulletDelete(){
	playerBullets.clear();
	enemyBullets.clear();
}