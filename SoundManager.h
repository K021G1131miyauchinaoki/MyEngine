#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include<string>
#include<fstream>
#include<sstream>
#include<map>
#include<xaudio2.h>
#include<cstdint>


class SoundManager
{
public:
	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];
		int32_t size;
	};
	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;
		char type[4];
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
	};
public:
	//初期化
	void Initialize();
	//終了処理
	void Finalize();
	//サウンドデータの解放
	void UnLoad(SoundData*sounData);
	// wav読み込み
	void LoadWave(const std::string& filename);
	//再生
	void PlayWave(const std::string& filename);
private:
	//xAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	//サウンドデータの連想配列
	std::map<std::string, SoundData>soundDatas;
};

