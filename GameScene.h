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
	Sprite* mario = nullptr;
	Sprite* flies = nullptr;
	
	//���f��
	Model* model = nullptr;
	Model* model2 = nullptr;
	//3d�I�u�W�F�N�g����
	Object3d* triangle = nullptr;
	Object3d* square = nullptr;
	//imgui�N���X
	ImguiManager* imguiM = nullptr;
	//�ϐ�
	float position[2] = { 100,100 };

	//wav�ǂݍ���
	SoundManager* audio = nullptr;
	
	//�p�[�e�B�N��
	ParticleManager* particle = nullptr;
	//�Q�[���I���t���O
	bool isEnd = false;
};

