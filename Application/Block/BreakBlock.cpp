#include "BreakBlock.h"

void BreakBlock::Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_)
{
	rotEnd = rot_;
	rotStart = { 0.0f,360.0f,0.0f };
	scaleEnd = scale_;
	scaleStart = { 0.0f,0.0f,0.0f };
	BaseBlock::Initialize(pos_,rotStart,scaleStart,model_);
	isStaging = false;
	stagingTime = 0;
	//モデル差し替えたら消す
	obj->SetColor({ 0.0f,0.0f,0.0f,1.0f });
}

void BreakBlock::Update()
{
	BaseBlock::StartStaging();
	BaseBlock::OutStaging();
	BaseBlock::Update();
}

void BreakBlock::Draw()
{
	BaseBlock::Draw();
}

void BreakBlock::OnCollision()
{
	hp--;
	if ( hp<=0 )
	{
		isDead = true;
	}
}
