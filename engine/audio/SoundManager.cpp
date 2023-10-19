/**
 * @file SoundManager.cpp
 * @brief　waveファイル操作
 */
#include "SoundManager.h"
#include <windows.h>
#include<cassert>
#pragma comment(lib,"xaudio2.lib")

std::string directoryPath = "Resources/";

void SoundManager::Initialize() {
	HRESULT result;
	IXAudio2MasteringVoice* masterVoice;
	//xAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));
	//マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

}



void SoundManager::LoadWave(const std::string& filename_)
{
	//重複の場合抜ける
	if (soundDatas.find(filename_)!=soundDatas.end())
	{
		return;
	}
	std::string filePath = directoryPath + filename_;
	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(filePath, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());
	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFがチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}
	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0)
	{
		assert(0);
	}
	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);
	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}
	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}
	//Dataチャンクのデータ部の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	//waveファイルを閉じる
	file.close();
	//returnするための音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列に格納
	soundDatas.insert(std::make_pair(filename_, soundData));
}

void SoundManager::Finalize() {
	xAudio2.Reset();
	
	//音声データを解放
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();
	for (;it!=soundDatas.end(); ++it)
	{
		UnLoad(&it->second);
	}
	soundDatas.clear();
}

void SoundManager::UnLoad(SoundData* soundData_) {
	//バッファのメモリを解放
	delete[] soundData_->pBuffer;

	soundData_->pBuffer = 0;
	soundData_->bufferSize = 0;
	soundData_->wfex = {};
}

void SoundManager::PlayWave(const std::string& filename_) {
	HRESULT	result;
	
	std::map<std::string, SoundData>::iterator it=soundDatas.find(filename_);
	//未読み込みの検出
	assert(it != soundDatas.end());
	//サウンドデータの参照を取得
	SoundData& soundData = it->second;
	//波形フォーマットをもとにSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生するデータの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

}