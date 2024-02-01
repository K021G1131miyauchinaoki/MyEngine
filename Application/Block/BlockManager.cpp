#include "BlockManager.h"
#include"Fixedgun.h"
#include"Wall.h"

void BlockManager::Initialize(BulletManager* bulletManager_) {
	bulletManager = bulletManager_;
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
	blocks.emplace_front(std::move(b));
}