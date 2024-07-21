#include "BulletManager.h"
#include"Homing.h"
#include<BillboardParticle.h>

int32_t BulletManager::eBulletCount = 0;

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
	pBulletCount = 3;
	playerBullets = std::make_unique<ObjectPool<Bullet>>(pBulletCount);
	enemyBullets = std::make_unique<ObjectPool<EnemyBullet>>(50);
}

void BulletManager::Update() {
	
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets->GetPool() )
	{
		if ( playerBullet->IsDead() )
		{
			playerBullet->Update();
			endColor = { 0.0f,1.0f,0.0f,0.0f };
			pos = { playerBullet->GetPos().x,playerBullet->GetPos().y,playerBullet->GetPos().z };
			billParticle->Add(life,pos,vec,accel,startScale,endScale,startColor,endColor);
		}
	}

	for ( size_t i = 0; i < eBulletCount; i++ )
	{
		if ( enemyBullets->GetPool()[i]->IsDead() )
		{
			enemyBullets->GetPool()[ i ]->Update();
			endColor = { 1.0f,0.0f,0.0f,0.0f };
			pos = { enemyBullets->GetPool()[ i ]->GetPos().x,enemyBullets->GetPool()[ i ]->GetPos().y,enemyBullets->GetPool()[ i ]->GetPos().z };
			billParticle->Add(life,pos,vec,accel,startScale,endScale,startColor,endColor);
		}
	}
}

void BulletManager::Draw(){
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets->GetPool() )
	{
		if ( playerBullet->IsDead() )
		{
			playerBullet->Draw();
		}
	}

	for ( size_t i = 0; i < pBulletCount; i++ )
	{
		if ( enemyBullets->GetPool()[ i ]->IsDead() )
		{
			enemyBullets->GetPool()[ i ]->Draw();
		}
	}
}

void BulletManager::PlayerBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_){
	// 弾生成
	Bullet* newBullet;
	for ( size_t i = 0; i < pBulletCount; i++ )
	{
		if ( !playerBullets->GetPool()[ i ]->IsDead())
		{
			newBullet = playerBullets->GetPool()[ i ].get();
			newBullet->Initialize(playerBulletModel.get(),pos_,vec_,rot_);
			newBullet->SetMotionSpeed(mSpeed);
			break;
		}
	}
}

void BulletManager::EnemyBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_,const std::string type_){
	// 弾生成
	EnemyBullet* newBullet;
	for ( size_t i = 0; i < eBulletCount; i++ )
	{
		if ( !enemyBullets->GetPool()[ i ]->IsDead() )
		{
			if ( type_ == "normal" )
			{
				newBullet = enemyBullets->GetPool()[ i ].get();
				newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_);
				newBullet->SetMotionSpeed(mSpeed);
				break;
			}
			else if ( type_ == "homing" )
			{
				newBullet = enemyBullets->GetPool()[ i ].get();
				newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_);
				newBullet->SetMotionSpeed(mSpeed);
				break;
			}
			else
			{
				newBullet = enemyBullets->GetPool()[ i ].get();
				newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_);
				newBullet->SetMotionSpeed(mSpeed);
				break;
			}
			
		}
	}
	
	
}


void BulletManager::AllBulletDelete(){
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets->GetPool() )
	{
		if ( playerBullet->IsDead() )
		{
			playerBullet->XORDead();
		}
	}

	for ( size_t i = 0; i < pBulletCount; i++ )
	{
		if ( enemyBullets->GetPool()[ i ]->IsDead() )
		{
			enemyBullets->GetPool()[ i ]->XORDead();
		}
	}
}

void BulletManager::SetMotionSpeed(const float& mSpeed_)
{
	//速度の調整
	mSpeed = mSpeed_;
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets->GetPool() )
	{
		if ( playerBullet->IsDead() )
		{
			playerBullet->SetMotionSpeed(mSpeed);
		}
	}

	for ( size_t i = 0; i < pBulletCount; i++ )
	{
		if ( enemyBullets->GetPool()[ i ]->IsDead() )
		{
			enemyBullets->GetPool()[ i ]->SetMotionSpeed(mSpeed);
		}
	}
}

void BulletManager::CountReset()
{
	eBulletCount = 0;
}
