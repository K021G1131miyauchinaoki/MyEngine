#include "Framework.h"

void Framework::Initialize(){}

void Framework::Update(){}

void Framework::Draw(){}

void Framework::Finalize(){}

void Framework::Run() {
	//初期化
	Initialize();

	//ループ
	while (true) {
		//毎フレーム更新
		Update();
		//終了リクエストが来たら抜ける
		if (IsEndRequst())
		{
			break;
		}

		//描画
		Draw();
	}
	//ゲーム終了
	Finalize();
}