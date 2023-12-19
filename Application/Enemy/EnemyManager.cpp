#include "EnemyManager.h"
#include"Enemy.h"
#include"ShotgunEnemy.h"

void EnemyManager::Add(const std::string& name_,Model* model_,Model* parachuteModel_,Player* player_,const Vector3& pos_,const Vector3& rot_,BulletManager* bulletManager_) {

	std::unique_ptr <BaseEnemy> e;
	if ( name_ == "Normal" )
	{
		e = std::make_unique <Enemy>();
	}
	else if(name_=="Shotgun" )
	{
		e = std::make_unique<ShotgunEnemy>();
	}
	else
	{
		e = std::make_unique <Enemy>();
	}
	e->Initialeze(model_,parachuteModel_,player_,pos_,rot_,bulletManager_);

	enemys.emplace_front(std::move(e));
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