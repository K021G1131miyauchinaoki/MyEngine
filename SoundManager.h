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
	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];
		int32_t size;
	};
	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;
		char type[4];
	};
	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�̃T�C�Y
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

