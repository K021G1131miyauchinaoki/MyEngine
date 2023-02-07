#pragma once
#include <Windows.h>
#include <wrl.h>
#include<cassert>
#include <DirectXMath.h>
#include<xaudio2.h>
#include<string>
#include<fstream>
#include<sstream>
#pragma comment(lib,"xaudio2.lib")


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
		unsigned int bufferSize;
	};
public:
	SoundData SoundLoadWave(std::string filename);

	void SoundUnLoad();

	void SoundPlayWave();

	static void CreateAudio();
private:
	
	static	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
	static	HRESULT result;

	SoundData soundData;
};

