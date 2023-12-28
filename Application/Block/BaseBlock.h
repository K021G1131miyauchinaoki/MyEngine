#pragma once
#include<memory>
#include<Object3d.h>
#include<Vector3.h>
#include<BulletManager.h>

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
	//パラメータのセット
	virtual void SetParameter(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_);

	virtual void Staging();

	//演出セッター
	void SetIsStaging(const bool& isStaging_);
	//演出ゲッター
	bool GetIsStaging() {return isStaging;}
	/*getter*/
	//位置
	virtual Vector3 GetPos() {return obj->GetPosition();}
	//回転
	virtual Vector3 GetRot() {return obj->GetRotation();}
	//スケール
	virtual Vector3 GetScale() {return obj->GetScale();}
	/*タイム*/
	//getter
	float GetTime() {return stagingTime;}
	//setter
	void SetTime(const float&time_) {stagingTime=time_;}
	//弾マネージャーのセット
	virtual void SetBulletManager(BulletManager* bulletManager_) {bulletManager = bulletManager_;}

	void SetColor(const XMFLOAT4 color_) {obj->SetColor(color_);}

protected://メンバ変数
	std::unique_ptr<Object3d> obj;
	BulletManager* bulletManager;
	Vector3 rotEnd,rotStart;
	Vector3 scaleEnd,scaleStart;
	const float stagingTimer = 40;
	float stagingTime;
	bool isStaging = false;
};

