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

class TitleScene:public BaseScene
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

	TitleScene();
	~TitleScene()override;

private:
	//�X�v���C�g
	std::unique_ptr<Sprite> titleSprite;
	//����
	std::unique_ptr <Input>input;
	//�J����������
	std::unique_ptr<Camera>camera;
	//obj���f��
	std::unique_ptr <Model> modelSkydome = nullptr;
	std::unique_ptr <Model> cube = nullptr;
	std::unique_ptr <Model> tank = nullptr;
	std::unique_ptr <Model> modelMap = nullptr;

	std::unique_ptr <Object3d> objSkydome = nullptr;

	std::unique_ptr<Player> player;
	std::unique_ptr<Map>map;

	std::unique_ptr<ImguiManager> ImgM;
};

