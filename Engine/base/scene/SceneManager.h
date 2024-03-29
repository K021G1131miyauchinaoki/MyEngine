/**
 * @file SceneManager.h
 * @brief シーンマネージャー
 */

#pragma once
#include<BaseScene.h>
#include<memory>
#include<AbstractSceneFactory.h>
using namespace MyEngin;

class SceneManager
{
public:
	//更新
	void Update();
	//オブジェクト描画
	void ObjDraw();
	//スプライト描画
	void SpriteDraw();
	//ジオメトリ描画
	void GeometryDraw();
	//次シーン予約
	void SetNextScene(BaseScene* nextScene_) { nextScene = nextScene_; }
	//シーンファクトリーのセッター
	void SetSceneFactory(AbstractSceneFactory* sceneFactory_) { sceneFactory = sceneFactory_; }
	//シーンの切り替え
	void ChangeScene(const std::string& sceneName_);

	//インスタンス生成
	static SceneManager* GetInstance();

	static int8_t playerHP;
	static int8_t stage;

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

