/**
 * @file Framework.h
 * @brief シーン用フレームワーク
 */

#pragma once
#include<Input.h>
#include<WinApp.h>
#include<DirectXCommon.h>
#include<SpriteCommon.h>
#include<PostEffect.h>
#include<Model.h>
#include<Object3d.h>
#include<Geometry.h>
#include<FbxObject3d.h>
#include<FbxLoader.h>
#include<FbxModel.h>
#include<memory>
#include<SceneManager.h>
#include<AbstractSceneFactory.h>
#include<Light.h>
using namespace MyEngin;

class Framework
{
public://メンバ関数
	//デストラクタ
	virtual ~Framework() = default;
	//初期化
	virtual void Initialize();
	//終了
	virtual	void Finalize();
	//更新
	virtual	void Update();
	//描画
	virtual	void Draw() = 0;

	//実行
	void Run();
	//終了チェック
	virtual	bool IsEndRequst();
protected:
	std::unique_ptr<Input>input;
	std::unique_ptr<WinApp>winApp;
	std::unique_ptr<DirectXCommon>dxCommon;
	std::unique_ptr<PostEffect>postEffect;
	AbstractSceneFactory* sceneFactory;
};

