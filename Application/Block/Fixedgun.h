#pragma once
#include"BaseBlock.h"

class Fixedgun :public BaseBlock
{
public:
	//初期化
	void Initialize(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_,Model* model_)override;
	//更新
	void Updata()override;
	//描画
	void Draw()override;
	//パラメータのセット
	void SetParameter(const Vector3& pos_,const Vector3& rot_,const Vector3& scale_)override;
	//弾を撃つ
	void Shot();

	//演出セッター
	void SetIsUp(const bool& isUp_);
	//演出ゲッター
	bool GetIsUp() {return isUp;}
	/*getter*/
	//位置
	Vector3 GetPos()override;
	//回転
	Vector3 GetRot()override;
	//スケール
	Vector3 GetScale()override;
	/*タイム*/
	//getter
	float GetTime() {return stagingTime;}
	//setter
	void SetTime(const float& time_) {stagingTime = time_;}
	//弾マネージャーのセット
	void SetBulletManager(BulletManager* bulletManager_)override {bulletManager = bulletManager_;}
private:
	Vector3 velocity;
	std::unique_ptr<Object3d> obj;
	float frame;
	float stagingTime;
	bool isUp = false;
	float waitTime;
	const float waitTimer=30;
	Vector3 pos;
};

