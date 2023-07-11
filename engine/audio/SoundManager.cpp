#include "SoundManager.h"
#include <windows.h>
#include<cassert>
#pragma comment(lib,"xaudio2.lib")

std::string directoryPath = "Resources/";

void SoundManager::Initialize() {
	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;
	//xAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

}



void SoundManager::LoadWave(const std::string& filename)
{
	//�d���̏ꍇ������
	if (soundDatas.find(filename)!=soundDatas.end())
	{
		return;
	}
	std::string filePath = directoryPath + filename;
	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filePath, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	assert(file.is_open());
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}
	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}
	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}
	//Data�`�����N�̃f�[�^���̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//wave�t�@�C�������
	file.close();
	//return���邽�߂̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//�T�E���h�f�[�^��A�z�z��Ɋi�[
	soundDatas.insert(std::make_pair(filename, soundData));
}

void SoundManager::Finalize() {
	xAudio2.Reset();
	
	//�����f�[�^�����
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();
	for (;it!=soundDatas.end(); ++it)
	{
		UnLoad(&it->second);
	}
	soundDatas.clear();
}

void SoundManager::UnLoad(SoundData* soundData) {
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void SoundManager::PlayWave(const std::string& filename) {
	HRESULT	result;
	
	std::map<std::string, SoundData>::iterator it=soundDatas.find(filename);
	//���ǂݍ��݂̌��o
	assert(it != soundDatas.end());
	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;
	//�g�`�t�H�[�}�b�g�����Ƃ�SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

}