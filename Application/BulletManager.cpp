#include "BulletManager.h"

void BulletManager::Initialize(Model* model_) {
	playerBulletModel.reset(model_);
	enemyBulletModel.reset(model_);
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

void BulletManager::PlayerBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_){
	// 弾生成
	std::unique_ptr<Bullet> newBullet=std::make_unique<Bullet>();
	newBullet->Initialize(playerBulletModel.get(),pos_,vec_,rot_);
	// 登録
	playerBullets.push_back(std::move(newBullet));
}

void BulletManager::EnemyBulletCreate(const Vector3& pos_,const Vector3& vec_,const Vector3& rot_){
	// 弾生成
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(enemyBulletModel.get(),pos_,vec_,rot_);
	// 登録
	enemyBullets.push_back(std::move(newBullet));
}


void BulletManager::AllBulletDelete(){
	playerBullets.clear();
	enemyBullets.clear();
}