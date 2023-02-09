#include<string>
#include <DirectXMath.h>
#include<d3dcompiler.h>
#include"DirectXTex/DirectXTex.h"
#include<cassert>
#include<vector>
#include"imgui/imgui.h"

#pragma	comment(lib,"d3dcompiler.lib")
#pragma	comment(lib, "d3d12.lib")
#pragma	comment(lib,"dxgi.lib")

//自作クラス
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"ImguiManager.h"
#include"Collision.h"

using namespace DirectX;
using	namespace Microsoft::WRL;

struct ConstBufferDataMaterial
{
	XMFLOAT4	color;//(RGBA)
};

struct ConstBufferDataTransform
{
	XMMATRIX	mat;//３D変換行列
};

struct Vertex
{
	XMFLOAT3	pos;//xyz座標
	XMFLOAT3	normal;//法線ベクトル
	XMFLOAT2	uv;	//uv座標
};
float	R = 1.0f;
float	G = 0.0f;
float	B = 0.0f;





//windowsアプリでのエントリーポイント(main関数)
int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma	region	基盤システムの初期化
	#pragma region WindowsAPIの初期化
	WinApp* winApp = nullptr;
	winApp = new	WinApp;
	winApp->Initialize();
	#pragma endregion

	#pragma region DirectX初期化処理
	//DirectX初期化処理　　ここから
	DirectXCommon* directXCom = nullptr;
	directXCom = new DirectXCommon;
	directXCom->Initialize(winApp);

	Input* input = nullptr;
	input = new	Input;
	input->Initialize(winApp);
	//DirectX初期化処理　　ここまで
	#pragma endregion
	//スプライト
	SpriteCommon* spriteCommon = nullptr;
	//スプライト共通部分の初期化
	spriteCommon = new	SpriteCommon;
	spriteCommon->Initialize(directXCom);

#pragma	endregion
#pragma	region	最初のシーンの初期化
	//一度しか宣言しない
	Object3d::StaticInitialize(directXCom->GetDevice(), WinApp::window_width, WinApp::window_height);
	//スプライト
	
	//モデル
	Model* modelPlane = Model::LoadFromOBJ("Plane");
	Model* modelSphere = Model::LoadFromOBJ("Sphere");
	//3dオブジェクト生成
	Object3d* planeObj = Object3d::Create();
	Object3d* sphereObj = Object3d::Create();
	//modelクラスをひも付け
	//平面
	planeObj->SetModel(modelPlane);
	planeObj->SetPos({ 0,0,0 });
	planeObj->SetScale({255,1,255 });
	//球
	sphereObj->SetModel(modelSphere);
	sphereObj->SetPos({ +5,20,0 });
	sphereObj->SetScale({10,10,10});

	//imguiクラス
	ImguiManager* imguiM = new ImguiManager;
	imguiM->Initialize(winApp, directXCom);
	//変数
	float position[2] = {100,100};
	Sphere sphere;
	sphere.radius = 10.0f;
	Plane plane;
	plane.distance = 0.0f;
	plane.normal = { 0,1,0 };

#pragma	endregion
	while (true)
	{

	#pragma region メッセージ
		if (winApp->ProcessMessage()) {
			break;
		}
	#pragma endregion

	#pragma region 毎フレーム処理

		input->Update();
		//imgui関連
		imguiM->Begin();
		//ここから中身を書いていく
		//デモウィンドウの表示オン
		//ImGui::ShowDemoWindow();
		//ImGui::SliderFloat2("mario",position ,  0.0f, WinApp::window_width);
		imguiM->End();
		
		//数字の0キーが押されてたら
		if (input->TriggerKey(DIK_0))
		{
			OutputDebugStringA("Hit 0\n");//出力ウィンドウに表示
		}
		//球の移動処理
		{
			Vector3 move=sphereObj->GetPos();
			sphere.center = sphereObj->GetPos();
			if (input->PushKey(DIK_LEFT))
			{
				move.x -= 1.0f;
			}
			else if (input->PushKey(DIK_RIGHT))
			{
				move.x += 1.0f;
			}
			if (input->PushKey(DIK_DOWN))
			{
				move.y -= 1.0f;
			}
			if (input->PushKey(DIK_UP))
			{
				move.y += 1.0f;
			}
			sphereObj->SetPos(move);
		}
		//球と平面の当たり判定
		{
			bool hit = Collision::CheckAphere2Plane(sphere, plane);
			if (hit)
			{
				sphereObj->SetColor({ 1,0,0,1 });
			}
			else
			{
				sphereObj->SetColor({ 1,1,1,1 });
			}
		}
		
		planeObj->Update();
		sphereObj->Update();
		//-------------------描画処理-------------------
		//Direct毎フレーム処理　ここから
		directXCom->PreDraw();
		//3Dオブジェクト
		Object3d::PreDraw(directXCom->GetCommandList());
		planeObj->Draw();
		sphereObj->Draw();
		Object3d::PostDraw();

		//スプライト

		//imgui
		imguiM->Draw();

		directXCom->PostDraw();

		//Direct毎フレーム処理　ここまで
	#pragma endregion
		if (input->PushKey( DIK_ESCAPE))
		{
			break;
		}
	}
#pragma	region	最初のシーンの終了
	winApp->Finalize();
	imguiM->Finalize();
	delete	input;
	delete winApp;
	delete	directXCom;
	delete	spriteCommon;
	delete modelPlane;
	delete modelSphere;
	delete planeObj;
	delete sphereObj;
	delete imguiM;
#pragma	endregion
	return 0;
}