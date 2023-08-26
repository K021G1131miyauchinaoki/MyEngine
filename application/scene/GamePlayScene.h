#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<ImguiManager.h>
#include<Enemy.h>
#include<Map.h>
#include<BaseScene.h>

class GamePlayScene:public BaseScene
{
public://�����o�֐�

	//������
	void Initialize()override;

	//�X�V
	void Update()override;

	//�`��
	void ObjDraw()override;
	void SpriteDraw()override;

	//�I��
	void Finalize()override;

	//�����蔻��
	void CheckAllCollision();

private:
	//�C���v�b�g
	std::unique_ptr<Input>input;

	//�X�v���C�g
	//SpriteCommon* spriteCommon = nullptr;
	//�J����������
	std::unique_ptr<Camera>camera;
	//�X�v���C�g


	//json�ǂݍ���
	//LevelData* levelData = nullptr;

	//obj���f��
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> tank = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;

	//FBX
	//FbxModel* fbxM = nullptr;
	//FbxObject3d* fbxObj = nullptr;
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy>enemy;
	std::unique_ptr<Map>map;

	std::unique_ptr<ImguiManager> ImgM;

	//�}�b�v�N���X�̏������W
	float mapStratY;
};

