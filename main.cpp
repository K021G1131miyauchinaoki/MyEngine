#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex/DirectXTex.h"
#include<cassert>
#include<vector>
#include"imgui/imgui.h"
#include"GameScene.h"

#pragma	comment(lib,"d3dcompiler.lib")
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

//windowsアプリでのエントリーポイント(main関数)
int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma	region	基盤システムの初期化
	GameScene gameScene;
	gameScene.Initialize();
	while (true)
	{
		//更新
		gameScene.Update();
		if (gameScene.IsEnd())
		{
			break;
		}
		//描画
		gameScene.Draw();

	}
#pragma	region	最初のシーンの終了
	gameScene.Finalize();


#pragma	endregion
	return 0;
}