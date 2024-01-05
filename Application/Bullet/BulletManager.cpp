#include "BulletManager.h"
#include"Homing.h"
#include"ParticleManager.h"
#include<Geometry.h>

void BulletManager::Initialize(Model* model_,Player*player_,Geometry* geometry_) {
	playerBulletModel.reset(model_);
	enemyBulletModel.reset(model_);
	player = player_;
	geometry = geometry_;
}

void BulletManager::Update() {
	XMFLOAT3 pos,vec,accel;
	pos = { 0.0f,0.0f,0.0f };
	vec = { 0.0f,0.0f,0.0f };
	accel = { 0.0f,0.0f,0.0f };
	float startScale,endScale;
	int life = 20;
	startScale = 2.5f;
	endScale = 0.0f;
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
		pos = { playerBullet->GetPos().x,playerBullet->GetPos().y,playerBullet->GetPos().z };
		geometry->Add(life,pos,vec,accel,startScale,endScale);
	}

	for ( std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets )
	{
		enemyBullet->Update();
		pos = { enemyBullet->GetPos().x,enemyBullet->GetPos().y,enemyBullet->GetPos().z };
		geometry->Add(life,pos,vec,accel,startScale,endScale);
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