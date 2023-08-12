#pragma once
#include<Input.h>
#include<DirectXCommon.h>
#include"Sprite.h"
#include"ImguiManager.h"
#include"SoundManager.h"
#include"Camera.h"
#include"LevelLoader.h"
#include<Player.h>
#include<Aimposition.h>
#include<ImguiManager.h>
#include<Enemy.h>
#include<Map.h>


class GamePlayScene
{
public://�����o�֐�

	//������
	void Initialize(DirectXCommon*dxCommon,Input*input);

	//�X�V
	void Update();

	//�`��
	void ObjDraw();
	void SpriteDraw();

	//�I��
	void Finalize();
private:
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
	std::unique_ptr<Aimposition> aim;
	std::unique_ptr<Enemy>enemy;
	std::unique_ptr<Map>map;

	std::unique_ptr<ImguiManager> ImgM;

	//�}�b�v�N���X�̏������W
	float mapStratY;
};

