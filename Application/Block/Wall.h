#pragma once
#include "BaseBlock.h"
class Wall : public BaseBlock
{
public:
	//初期化
	void Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_)override;
	//更新
	void Update()override;
	//描画
	void Draw()override;
	//位置
	Vector3 GetPos()override {
		return obj->GetPosition();
	}
	//スケール
	Vector3 GetScale()override {
		return obj->GetScale();
	}
private:
	
};

