/**
 * @file Input.h
 * @brief　入力装置の操作
 */

#define	DIRECTINPUT_VERSION	0x0800 //DirectInputのバージョン指定
#pragma warning(push)
#pragma warning(disable:4820)
#include<dinput.h>
#include<Windows.h>
#include<wrl.h>
#pragma warning(pop)
#include<cassert>
#include"WinApp.h"
#include<Vector2.h>

#pragma	comment(lib,"dinput8.lib")
#pragma	comment(lib,"dxguid.lib")
#pragma once

enum Botton {
	LEFT,
	RIGHT,
	CENTER
};
namespace MyEngin
{
	class Input
	{
	public:
		template<class T>using	ComPtr = Microsoft::WRL::ComPtr<T>;

	public://メンバ関数
		//初期化
		void Initialize();
		//更新
		void Update();
		/*キーボード*/
		//押したとき
		bool PushKey(BYTE	keyNumber);
		//押した瞬間
		bool TriggerKey(BYTE	keyNumber);
		//離した瞬間
		bool TriggerReleaseKey(BYTE keyNumber);

		/*マウス*/
		//押したとき
		bool PushClick(Botton botton);
		//押した瞬間
		bool TriggerClick(Botton botton);
		//離した瞬間
		bool TriggerReleaseClick(BYTE botton);
		//2D座標の取得
		const Vector2 GetMausePos() {
			return mPos;
		}

//インスタンス生成
		static Input* GetInstance();

	private:
		WinApp* winApp = nullptr;

		HRESULT	result = 0;

		ComPtr<	IDirectInput8> directInput = nullptr;

		ComPtr < IDirectInputDevice8> keyboard = nullptr;
		ComPtr < IDirectInputDevice8> mouseDevice = nullptr;
		//全キーの入力状態を取得する
		BYTE	key[ 256 ] = {};
		//前回の全キーの状態
		BYTE	keyPre[ 256 ] = {};

		DIMOUSESTATE mouse;
		DIMOUSESTATE oldMouse;
		Vector2 mPos;

	};

}