#include "BulletManager.h"
#include"Homing.h"
#include<BillboardParticle.h>

void BulletManager::Initialize(Model* model_,Player*player_,BillboardParticle* geometry_) {
	playerBulletModel.reset(model_);
	enemyBulletModel.reset(model_);
	player = player_;
	billParticle = geometry_;
	startColor = { 1,1,1,1 };
	endColor = { 1,1,1,0 };
	pos = { 0.0f,0.0f,0.0f };
	vec = { 0.0f,0.0f,0.0f };
	accel = { 0.0f,0.0f,0.0f };
	startScale = 2.5f;
	endScale = 1.0f;
	mSpeed = 1.0f;
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
		endColor = { 0.0f,1.0f,0.0f,0.0f };
		pos = { playerBullet->GetPos().x,playerBullet->GetPos().y,playerBullet->GetPos().z };
		billParticle->Add(life,pos,vec,accel,startScale,endScale,startColor,endColor);
	}

	for ( std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets )
	{
		enemyBullet->Update();
		endColor = { 1.0f,0.0f,0.0f,0.0f };                  
		pos = { enemyBullet->GetPos().x,enemyBullet->GetPos().y,enemyBullet->GetPos().z };
		billParticle->Add(life,pos,vec,accel,startScale,endScale,startColor,endColor);
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
	newBullet->SetMotionSpeed(mSpeed);
	// 登録
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_,const std::string type_){
	// 弾生成
	std::unique_ptr<EnemyBullet> newBullet;
	if ( type_=="normal" )
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
	newBullet->SetMotionSpeed(mSpeed);

	// 登録
	enemyBullets.push_back(std::move(newBullet));
}


void BulletManager::AllBulletDelete(){
	playerBullets.clear();
	enemyBullets.clear();
}

void BulletManager::SetMotionSpeed(const float& mSpeed_)
{
	mSpeed = mSpeed_;
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets )
	{
		playerBullet->SetMotionSpeed(mSpeed);
	}

	for ( std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets )
	{
		enemyBullet->SetMotionSpeed(mSpeed);
	}
}
