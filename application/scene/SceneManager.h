#pragma once
#include<BaseScene.h>
#include<memory>
#include<GamePlayScene.h>
#include<TitleScene.h>
#include<DirectxCommon.h>
#include<Input.h>

class SceneManager
{
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();
	//更新
	void Update();
	//描画
	void ObjDraw();
	void SpriteDraw();
	//次シーン予約
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }
	
	void SetDxCommon(DirectXCommon* dxCommon_) { dxCommon.reset(dxCommon_); }
	void SetInput(Input* input_) { input.reset(input_); }

private:
	//現在のシーン
	BaseScene* scene = nullptr;
	// 次のシーン
	BaseScene* nextScene = nullptr;

	std::unique_ptr<DirectXCommon>dxCommon;
	std::unique_ptr<Input>input;


};

