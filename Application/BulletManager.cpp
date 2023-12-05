#include "BulletManager.h"

BulletManager* BulletManager::GetInstance() {
	static BulletManager *instance;
	return instance;
}

void BulletManager::Initialize() {
	playersBulletModel.reset(Model::LoadFromOBJ("cube"));
	enemysBulletModel.reset(Model::LoadFromOBJ("cube"));
}

void BulletManager::Update() {
	for ( std::unique_ptr<Bullet>& playerBullet : playerBullets )
	{
		playerBullet->Update();
	}

	for ( std::unique_ptr<EnemyBullet>& enemyBullet : enemyBullets )
	{
		enemyBullet->Update();
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

void BulletManager::PlayerBulletShot(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_){
	// 弾生成
	std::unique_ptr<EnemyBullet> newBullet=std::make_unique<EnemyBullet>();
	newBullet->Initialize(pos_,vec_,rot_);
	// 登録
	enemyBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletShot(const Vector3& pos,const Vector3& vec_,const Vector3& rot_){}


void BulletManager::AllBulletDelete(){}