/**
 * @file SceneManager.h
 * @brief シーンマネージャー
 */
#pragma once
#include<BaseScene.h>
#include<memory>
#include<AbstractSceneFactory.h>

class SceneManager
{
public:
	//更新
	void Update();
	//オブジェクト描画
	void ObjDraw();
	//スプライト描画
	void SpriteDraw();
	//次シーン予約
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }
	//シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }
	//シーンの切り替え
	void ChangeScene(const std::string& sceneName_);

	//インスタンス生成
	static SceneManager* GetInstance();

	//（例）自機をタイトルだと自動,プレイ中は任意で移動ができるように管理する
	static int8_t sceneNum;
	enum
	{
		title,
		select,
		play,
		clear,
		over,
	};
	
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

