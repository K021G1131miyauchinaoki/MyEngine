#pragma once
#include<memory>
#include<Object3d.h>
#include<Vector3.h>
//ブロック

class BaseBlock
{
public:
	//初期化
	virtual void Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_);
	//更新
	virtual void Updata();
	//描画
	virtual void Draw();

	virtual void SetParameter(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_);

	void SetIsUp(const bool& isUp_);
	bool GetIsUp() {return isUp;}

	Vector3 GetPos() {return obj->GetPosition();}

	Vector3 GetRot() {return obj->GetRotation();}

	Vector3 GetScale() {return obj->GetScale();}

	float GetTime() {return stagingTime;}

	void SetTime(const float&time_) {stagingTime=time_;}
private:
	std::unique_ptr<Object3d> obj;
	float frame;
	float stagingTime;
	bool isUp = false;
};

