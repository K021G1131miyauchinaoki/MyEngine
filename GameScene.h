#pragma once
#include"Input.h"
#include"WinApp.h"
#include"DirectXCommon.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"Object3d.h"
#include"Model.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include "ParticleManager.h"
#include"Camera.h"
#include"FbxLoader.h"
#include"LevelLoader.h"
#include<vector>

class GameScene
{
public://�����o�֐�
	//������
	void Initialize();
	//�I��
	void Finalize();
	//�X�V
	void Update();
	//�`��
	void Draw();
	//�I���t���O�̃`�F�b�N
	bool IsEnd() { return isEnd; }
private:
	// WindowsAPI
	WinApp* winApp = nullptr;
	//DirectX
	DirectXCommon* dxCommon = nullptr;
	//�L�[
	Input* input = nullptr;
	//�X�v���C�g
	SpriteCommon* spriteCommon = nullptr;
	//�J����������
	std::unique_ptr<Camera>camera = std::make_unique<Camera>();
	//�X�v���C�g
		
	//���f��

	//json�ǂݍ���
	LevelData* levelData = nullptr;

	Model* modelSkydome = nullptr;
	Model* modelGround = nullptr;
	Model* modelChr = nullptr;
	Model* modelSphere = nullptr;

	/*Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objChr = nullptr;
	Object3d* objSphere = nullptr;*/

	std::map<std::string, Model*> models;

	std::vector<Object3d*> objects;

	//3d�I�u�W�F�N�g����
	
	//imgui�N���X

	//wav�ǂݍ���
	
	//�p�[�e�B�N��
	
	//�Q�[���I���t���O
	bool isEnd = false;
};

