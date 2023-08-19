#pragma once
#include<memory>

class SceneManager;

class BaseScene
{
public:
	virtual ~BaseScene() = default;
	//初期化
	virtual void Initialize();
	//終了
	virtual	void Finalize();
	//更新
	virtual	void Update();
	//描画
	virtual	void ObjDraw();
	virtual	void SpriteDraw();
	//シーンマネージャーをセット
	virtual void SetSceneManager(SceneManager* sceneManager_) { sceneManager = sceneManager_; }

protected:
	SceneManager* sceneManager = nullptr;	
};

