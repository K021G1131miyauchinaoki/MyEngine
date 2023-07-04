#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex.h"
#include<cassert>
#include<imgui.h>
#include"GameScene.h"
#include"Framework.h"
#include<dxgidebug.h>

#pragma comment(lib,"dxguid.lib")
#pragma	comment(lib,"d3dcompiler.lib")
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

//windowsアプリでのエントリーポイント(main関数)
int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma	region	基盤システムの初期化
	Framework* game = new GameScene();

	game->Run();

	delete game;
//	GameScene gameScene;
//	gameScene.Initialize();
//	while (true)
//	{
//		//更新
//		gameScene.Update();
//		if (gameScene.IsEnd())
//		{
//			break;
//		}
//		//描画
//		gameScene.Draw();
//
//	}
//#pragma	region	最初のシーンの終了
//	gameScene.Finalize();


#pragma	endregion
	//リソースリークチェック
	IDXGIDebug1* debug;
	if (SUCCEEDED(DXGIGetDebugInterface1(0,IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
		debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
		debug->Release();
	}

	return 0;
}