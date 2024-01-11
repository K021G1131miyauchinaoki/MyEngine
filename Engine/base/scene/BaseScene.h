/**
 * @file BaseScene.h
 * @brief シーン基盤
 */

#pragma once
#include<memory>

class SceneManager;
namespace MyEngin
{
	class BaseScene
	{
	public:
		//コンストラクタ
		virtual ~BaseScene() = default;
		//初期化
		virtual void Initialize();
		//終了
		virtual	void Finalize();
		//更新
		virtual	void Update();
		//オブジェクト描画
		virtual	void ObjDraw();
		//スプライト描画
		virtual	void SpriteDraw();
		//ジオメトリ描画
		virtual void GeometryDraw();
		//シーンマネージャーをセット
		virtual void SetSceneManager(SceneManager* sceneManager_) {
			sceneManager = sceneManager_;
		}

		void DecisionSound();
	protected:
		SceneManager* sceneManager = nullptr;
	};
}
