#pragma once
#include<fstream>
#include <vector>
#include <DirectXMath.h>
#include<nlohmann/json.hpp>

// ���x���f�[�^
struct LevelData {

	struct ObjectData {
		// �t�@�C����
		std::string fileName;
		// ���s�ړ�
		DirectX::XMVECTOR translation;
		// ��]�p
		DirectX::XMVECTOR rotation;
		// �X�P�[�����O
		DirectX::XMVECTOR scaling;
	};

	// �I�u�W�F�N�g�z��
	std::vector<ObjectData> objects;

	std::vector < LevelData> *chidren;
};

class LevelLoader
{
public:
	static LevelData* LoadJson(const std::string& fileName);

private:
	static LevelData* ImportValue(nlohmann::json& json, LevelData* levelData );
};

