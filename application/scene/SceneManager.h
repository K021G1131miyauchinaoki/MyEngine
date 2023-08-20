#pragma once
#include<BaseScene.h>
#include<memory>
#include<AbstractSceneFactory.h>

class SceneManager
{
public:
	//更新
	void Update();
	//描画
	void ObjDraw();
	void SpriteDraw();
	//次シーン予約
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }
	//シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }
	
	void ChangeScene(const std::string& sceneName);

	//インスタンス生成
	static SceneManager* GetInstance();
	
private:
	//現在のシーン
	BaseScene* scene = nullptr;
	// 次のシーン
	BaseScene* nextScene = nullptr;
	//シーンファクトリー
	AbstractSceneFactory* sceneFactory = nullptr;

	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;
};

