#include "GamePlayScene.h"
#include<SceneManager.h>

void GamePlayScene::Initialize() {
	input.reset(Input::GetInstance());
	/*�ϐ�*/
	mapStratY = -400;
	//�J����
	camera = std::make_unique<Camera>();
	camera->Initialeze();
	Object3d::SetCamera(camera.get());


	// ���f���ǂݍ���
	modelSkydome.reset(Model::LoadFromOBJ("skydome"));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	modelMap.reset(Model::LoadFromOBJ("map"));

	//���f���̃Z�b�g
	EnemyBullet::StaticInitialize(cube.get());
	Bullet::StaticInitialize(cube.get());
	Map::StaticInitialize(modelMap.get());


	//�X�J�C�h�[��
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());
	objSkydome->SetScale({ 5.0f,5.0f,5.0f });

	//���N�e�B��
	aim = std::make_unique<Aimposition>();
	aim->Initialeze(cube.get(), input.get());
	//�v���C���[
	player = std::make_unique<Player>();
	player->Initialeze(tank.get(), input.get(), aim.get());
	//�G�l�~�[
	enemy = std::make_unique<Enemy>();
	enemy->Initialeze(tank.get(), player.get());

	//�}�b�v
	map = std::make_unique<Map>(mapStratY);
	map->Initialize();
	map->LoadCSV("1");
}

void GamePlayScene::Update(){
	CheckAllCollision();
	camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	camera->SetEye({ player->GetPos().x, 100, player->GetPos().z - 30 });
	camera->Update();
	player->Update();
	enemy->Update();
	aim->Update();
	map->Update();
	objSkydome->Update();
	if (input->TriggerKey(DIK_1)||player->IsDead())
	{
		//�V�[���̐؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMEOVER");
	}
	if (input->TriggerKey(DIK_2))
	{
		//�V�[���̐؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
	}
}

void GamePlayScene::SpriteDraw() {
	player->SpriteDraw();
}

void GamePlayScene::ObjDraw(){
	enemy->Draw();
	objSkydome->Draw();
	player->ObjDraw();
	//aim->ObjDraw();
	map->Draw();
}

void GamePlayScene::CheckAllCollision() {
		//����Ώ�A��B�̍��W
		Vector3 posA, posB;
	
		//���e���X�g���擾
		const std::list<std::unique_ptr<Bullet>>& playerBullets = player->GetBullets();
		//�G�e���X�g���擾
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();
	#pragma	region	���L�����ƓG�e�̓����蔻��
		//���L�����̍��W
		posA = player->GetPos();
		//���L�����ƓG�e�S�Ă̓����蔻��
		for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
			//�G�e�̍��W
			posB = e_bullet->GetBulletPosition();
			//A,B�̋���
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float	radius = player->GetRadius() + e_bullet->GetRadius();
			//����
			if (dis <= radius) {
				//���L�����̃R�[���o�b�N���Ăяo��
				player->OnCollision();
				//�G�e�̃R�[���o�b�N���Ăяo��
				e_bullet->OnCollision();
			}
		}
	#pragma	endregion
	
	#pragma region ���e�ƓG�L�����̓����蔻��
		//�G�e�̍��W
		posA = enemy->GetPos();
		//�G�L�����Ǝ��e�S�Ă̓����蔻��
		for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
			//���e�̍��W
			posB = p_bullet->GetBulletPosition();
			// A,B�̋���
			Vector3 vecPos = MyMath::lens(posA, posB);
			float dis = MyMath::length(vecPos);
			//
			float radius = enemy->GetRadius() + p_bullet->GetRadius();
			//����
			if (dis <= radius) {
				//�G�L�����̃R�[���o�b�N���Ăяo��
				enemy->OnCollision();
				//���e�̃R�[���o�b�N���Ăяo��
				p_bullet->OnCollision();

				//�V�[���̐؂�ւ�
				SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
			}
		}
	#pragma endregion
	
	#pragma region ���e�ƓG�e�̓����蔻��
		//���e�̍��W
		for (const std::unique_ptr<Bullet>& p_bullet : playerBullets) {
			posA = p_bullet->GetBulletPosition();
			//���e�ƓG�e�S�Ă̓����蔻��
			for (const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets) {
				//�G�e�̍��W
				posB = e_bullet->GetBulletPosition();
				// A,B�̋���
				Vector3 vecPos = MyMath::lens(posA, posB);
				float dis = MyMath::length(vecPos);
				//
				float radius = e_bullet->GetRadius() + p_bullet->GetRadius();
				//����
				if (dis <= radius) {
					//���e�̃R�[���o�b�N���Ăяo��
					p_bullet->OnCollision();
					//�G�e�̃R�[���o�b�N���Ăяo��
					e_bullet->OnCollision();
				}
			}
		}
	#pragma endregion
}

void GamePlayScene::Finalize(){}