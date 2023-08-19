#pragma once
#include<BaseScene.h>
#include<memory>

class SceneManager
{
public:
	//コンストラクタ
	//SceneManager();
	//デストラクタ
	~SceneManager();
	//更新
	void Update();
	//描画
	void ObjDraw();
	void SpriteDraw();
	//次シーン予約
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }

private:
	//現在のシーン
	BaseScene* scene = nullptr;
	// 次のシーン
	BaseScene* nextScene = nullptr;
};

