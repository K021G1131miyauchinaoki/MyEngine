#pragma once
#include<vector>
#include"Framework.h"
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
#include<GamePlayScene.h>


class MyGame:public Framework
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

	//�����蔻��
	void CheckAllCollision();

private:
	//imgui
	std::unique_ptr<ImguiManager> ImgM;
	//�v���C�V�[��
	GamePlayScene* scene;
};

