/**
 * @file WorldTransform.cpp
 * @brief 変換行列
 */
#include "WorldTransform.h"

void WorldTransform::Initialize() {

	scale = { 1 , 1 , 1 };
	rotation = { 0 , 0 , 0 };
	translation = { 0 , 0 , 0 };
	matWorld=MatIdentity();

	Update();

}

//ワールド変換行列の更新
void WorldTransform::Update() {

	//ワールド変換行列を用意
	Matrix4 affineMat;
	affineMat=MatIdentity();

	//ワールド変換行列にスケーリング,回転,平行移動の行列を合成
	affineMat*= MatScale(scale);
	affineMat*=MatRot(rotation);
	affineMat*=MatTrans(translation);

	//ワールド行列に単位行列を代入
	matWorld=MatIdentity();

	//行列の計算
	matWorld *= affineMat;

	//もし親がある場合
	if (parent) {
		//親のワールド行列との計算を行う
		matWorld *= parent->matWorld;
	}

}