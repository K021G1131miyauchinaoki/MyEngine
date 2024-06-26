/**
 * @file SoundManager.h
 * @brief　waveファイル操作
 */

#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include<string>
#include<fstream>
#include<sstream>
#include<map>
#include<xaudio2.h>
#include<cstdint>
#include <windows.h>
#pragma comment(lib,"xaudio2.lib")

namespace MyEngin
{
	class SoundManager
	{
	public:
		//チャンクヘッダ
		struct ChunkHeader
		{
			char id[ 4 ];
			int32_t size;
		};
		//RIFFヘッダチャンク
		struct RiffHeader
		{
			ChunkHeader chunk;
			char type[ 4 ];
		};
		//FMTチャンク
		struct FormatChunk
		{
			ChunkHeader chunk;
			WAVEFORMATEX fmt;
		};

		//音声データ
		struct SoundData
		{
			//波形フォーマット
			WAVEFORMATEX wfex;
			//バッファの先頭アドレス
			BYTE* pBuffer;
			//バッファのサイズ
			uint32_t bufferSize;

			IXAudio2SourceVoice* pSourceVoice;
		};
	public:
		//インスタンス生成
		static SoundManager* GetInstance();
		//初期化
		void Initialize();
		//終了処理
		void Finalize();
		//サウンドデータの解放
		void UnLoad(SoundData* SoundManagerata_);
		// wav読み込み
		void LoadWave(const std::string& filename_);
		//再生
		void PlayWave(const std::string& filename_,const float& volume_ = 1.0f,const bool& isLoop_ = false);
		//停止
		void StopWave(const std::string& filename_);
	private:
		//xAudio2のインスタンス
		Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
		//サウンドデータの連想配列
		std::map<std::string,SoundData>soundDatas;
	private:
		SoundManager() = default;
		~SoundManager() = default;

		SoundManager(const SoundManager&) = default;
		const SoundManager& operator=(const SoundManager&) = delete;
	};

}