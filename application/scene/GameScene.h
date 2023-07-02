#pragma once
#include<vector>
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
#include"LevelLoader.h"
#include"Framework.h"
#include"FbxLoader.h"
#include"FbxObject3d.h"
#include"FbxModel.h"

#include<Player.h>
#include<Aimposition.h>

class GameScene:public Framework
{
public://�����o�֐�
	//������
	void Initialize()override;
	//�I��
	void Finalize()override;
	//�X�V
	void Update()override;
	//�`��
	void Draw()override;

	bool IsEndRequst() override;

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
	Model* box = nullptr;

	Object3d* objSkydome = nullptr;
	Object3d* objGround = nullptr;
	Object3d* objChr = nullptr;
	std::unique_ptr < Object3d> objSphere = nullptr;

	std::map<std::string, Model*> models;

	std::vector<Object3d*> objects;

	Sprite* s = nullptr;
	//FBX
	//FbxModel* fbxM = nullptr;
	//FbxObject3d* fbxObj = nullptr;
	std::unique_ptr<Player> player;
	std::unique_ptr<Aimposition> aim;

	//�Q�[���I���t���O
	bool isEndRequst = false;
};

