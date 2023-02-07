﻿#include<string>
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
#include"SoundManager.h"

//bgm
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

using namespace DirectX;
using	namespace Microsoft::WRL;

////チャンクヘッダ
//struct ChunkHeader
//{
//	char id[4];
//	int32_t size;
//};
////RIFFヘッダチャンク
//struct RiffHeader
//{
//	ChunkHeader chunk;
//	char type[4];
//};
////FMTチャンク
//struct FormatChunk
//{
//	ChunkHeader chunk;
//	WAVEFORMATEX fmt;
//};
//
////音声データ
//struct SoundData
//{
//	//波形フォーマット
//	WAVEFORMATEX wfex;
//	//バッファの先頭アドレス
//	BYTE* pBuffer;
//	//バッファのサイズ
//	unsigned int bufferSize;
//};
//
//SoundData SoundLoadWave(const char* filename)
//{
//	HRESULT result;
//	std::string filePath = defaultPath + filename;
//	//ファイル入力ストリームのインスタンス
//	std::ifstream file;
//	//.wavファイルをバイナリモードで開く
//	file.open(filePath, std::ios_base::binary);
//	//ファイルオープン失敗を検出する
//	assert(file.is_open());
//	//RIFFヘッダーの読み込み
//	RiffHeader riff;
//	file.read((char*)&riff, sizeof(riff));
//	//ファイルがRIFFがチェック
//	if (strncmp(riff.chunk.id,"RIFF",4)!=0)
//	{
//		assert(0);
//	}
//	//タイプがWAVEかチェック
//	if (strncmp(riff.type, "WAVE", 4) != 0)
//	{
//		assert(0);
//	}
//	//Formatチャンクの読み込み
//	FormatChunk format = {};
//	//チャンクヘッダーの確認
//	file.read((char*)&format, sizeof(ChunkHeader));
//	if (strncmp(format.chunk.id, "fmt", 4) != 0)
//	{
//		assert(0);
//	}
//	//チャンク本体の読み込み
//	assert(format.chunk.size <= sizeof(format.fmt));
//	file.read((char*)&format.fmt, format.chunk.size);
//	//Dataチャンクの読み込み
//	ChunkHeader data;
//	file.read((char*)& data, sizeof(data));
//	//JUNKチャンクを検出した場合
//	if (strncmp(data.id,"JUNK",4)==0)
//	{
//		//読み取り位置をJUNKチャンクの終わりまで進める
//		file.seekg(data.size, std::ios_base::cur);
//		//再読み込み
//		file.read((char*)&data, sizeof(data));
//	}
//	if (strncmp(data.id,"data",4)!=0)
//	{
//		assert(0);
//	}
//	//Dataチャンクのデータ部の読み込み
//	char* pBuffer = new char[data.size];
//	file.read(pBuffer, data.size);
//	//waveファイルを閉じる
//	file.close();
//	//returnするための音声データ
//	SoundData soundData = {};
//
//	soundData.wfex = format.fmt;
//	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
//	soundData.bufferSize = data.size;
//
//	return soundData;
//}
//
//void SoundUnLoad(SoundData* soundData) {
//	//バッファのメモリを解放
//	delete[] soundData->pBuffer;
//
//	soundData->pBuffer = 0;
//	soundData->bufferSize = 0;
//	soundData->wfex = {};
//}
//
//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {
//	HRESULT result;
//	//波形フォーマットをもとにSourceVoiceの生成
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	//再生するデータの設定
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	//波形データの再生
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//
//}

//windowsアプリでのエントリーポイント(main関数)
int	WINAPI	WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#pragma	region	基盤システムの初期化
	#pragma region WindowsAPIの初期化
	WinApp* winApp = nullptr;
	winApp = new	WinApp;
	winApp->Initialize();
	#pragma endregion

	#pragma region DirectX初期化処理
	//音
	HRESULT result;
	//DirectX初期化処理　　ここから
	DirectXCommon* directXCom = nullptr;
	directXCom = new DirectXCommon;
	directXCom->Initialize(winApp);
	SoundManager::CreateAudio();

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
	spriteCommon->Loadtexture(1, "mario.jpg");
	spriteCommon->Loadtexture(2, "tex.png");

#pragma	endregion
#pragma	region	最初のシーンの初期化
	//一度しか宣言しない
	Object3d::StaticInitialize(directXCom->GetDevice(), WinApp::window_width, WinApp::window_height);
	//スプライト
	Sprite* sprite = new	Sprite();
	sprite->Initialize(spriteCommon, 1);
	sprite->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	sprite->SetPosition(XMFLOAT2(100.0f, 100.0f));
	sprite->SetSize(XMFLOAT2(100.0f, 100.0f));
	Sprite* sprite2 = new	Sprite();
	sprite2->Initialize(spriteCommon, 2);
	sprite2->SetPosition(XMFLOAT2(50.0f, 50.0f));
	sprite2->SetColor(XMFLOAT4(0.1f, 0.0f, 0.0f, 0.5f));
	sprite2->SetAnchorPoint(XMFLOAT2(0.5f, 0.5f));
	//sprite2->SetIsFlipX(true);
	//sprite2->SetIsFlipY(true);

	sprite2->SetSize(XMFLOAT2(100.0f, 100.0f));
	//モデル
	Model* model = Model::LoadFromOBJ("triangle_mat");
	Model* model2 = Model::LoadFromOBJ("box_mat");
	//3dオブジェクト生成
	Object3d* obj3d = Object3d::Create();
	Object3d* obj3d2 = Object3d::Create();
	//modelクラスをひも付け
	obj3d->SetModel(model);
	obj3d2->SetModel(model2);
	obj3d->SetPosition({ -5,0,-5 });
	obj3d2->SetPosition({ +5,0,+50 });
	//imguiクラス
	ImguiManager* imguiM = new ImguiManager;
	imguiM->Initialize(winApp, directXCom);
	//変数
	float position[2] = {100,100};

	SoundManager* se = new SoundManager;
	se->SoundLoadWave("0321.wav");
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
		ImGui::ShowDemoWindow();
		ImGui::SliderFloat2("mario",position ,  0.0f, WinApp::window_width);
		imguiM->End();
		sprite->SetPosition(XMFLOAT2{ position[0], position[1] });

		//数字の0キーが押されてたら
		if (input->TriggerKey(DIK_0))
		{
			//音声再生
			se->SoundPlayWave();
			
			//OutputDebugStringA("Hit 0\n");//出力ウィンドウに表示
		}
		//スプライトの回転
		{
			float rotation = sprite->GetRotation();
			if (input->PushKey(DIK_O))
			{
				rotation += 10.0f;
			}
			else if (input->PushKey(DIK_P))
			{
				rotation -= 10.0f;
			}
			sprite->SetRotation(rotation);
			//sprite2->SetSize(XMFLOAT2(150.0f, 50.0f));
			sprite2->SetRotation(rotation);
		}
		//スプライトの座標
		{
			XMFLOAT2 position = sprite->GetPosition();
			if (input->PushKey(DIK_UP))
			{
				position.y -= 10.0f;
			}
			else if (input->PushKey(DIK_DOWN))
			{
				position.y += 10.0f;
			}
			if (input->PushKey(DIK_LEFT))
			{
				position.x -= 10.0f;
			}
			else if (input->PushKey(DIK_RIGHT))
			{
				position.x += 10.0f;

			}
			sprite->SetPosition(position);
		}
		obj3d->Update();
		obj3d2->Update();
		//-------------------描画処理-------------------
		//Direct毎フレーム処理　ここから
		directXCom->PreDraw();
		Object3d::PreDraw(directXCom->GetCommandList());
		obj3d->Draw();
		obj3d2->Draw();
		Object3d::PostDraw();

		//sprite->SetIsInvisible(true);
		sprite->SetTexIndex(1);
		sprite2->SetTexIndex(2);
		sprite->Draw();
		sprite2->Draw();

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
	delete	sprite;
	delete model;
	delete model2;
	delete obj3d;
	delete obj3d2;
	delete imguiM;
	delete se;

#pragma	endregion
	return 0;
}