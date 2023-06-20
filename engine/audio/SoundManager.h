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
		uint32_t bufferSize;
	};
public:
	//������
	void Initialize();
	//�I������
	void Finalize();
	//�T�E���h�f�[�^�̉��
	void UnLoad(SoundData*sounData);
	// wav�ǂݍ���
	void LoadWave(const std::string& filename);
	//�Đ�
	void PlayWave(const std::string& filename);
private:
	//xAudio2�̃C���X�^���X
	Microsoft::WRL::ComPtr<IXAudio2>xAudio2;
	//�T�E���h�f�[�^�̘A�z�z��
	std::map<std::string, SoundData>soundDatas;
};

