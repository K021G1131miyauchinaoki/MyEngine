/**
 * @file GamePlayScene.cpp
 * @brief ベースシーンを継承したプレイシーン
 */

#include "GamePlayScene.h"
#include"SceneTransition.h"
#include<SpriteCommon.h>
#include"WinApp.h"
#include"Easing.h"
#include<SceneManager.h>
#include<Vector3.h>
#include<SoundManager.h>

int8_t GamePlayScene::startCount = 0;
bool GamePlayScene::isStart = true;
/// <summary>
/// 矩形の判定
/// </summary>
bool CheckBoxXZ( Vector3 pos1,Vector3 scale1,Vector3 pos2,Vector3 scale2)
{
	float cl1 = pos1.x - scale1.x;
	float cr1 = cl1 + ( scale1.x * 2 );
	float cl2 = pos2.x - scale2.x;
	float cr2 = cl2 + ( scale2.x * 2 );

	if ( cr1 <= cl2 )
	{
		return false;
	}

	if ( cr2 <= cl1 )
	{
		return false;
	}

	float cf1 = pos1.z - scale1.z;
	float cb1 = cf1 + ( scale1.z * 2 );
	float cf2 = pos2.z - scale2.z;
	float cb2 = cf2 + ( scale2.z * 2 );

	if ( cb1 <= cf2 )
	{
		return false;
	}

	if ( cb2 <= cf1 )
	{
		return false;
	}

	return true;
}
/// <summary>
/// 線分の判定
/// </summary>
/// <param name="startA">直線のスタート</param>
/// <param name="endA">直線のエンド</param>
/// <param name="startB">線分のスタート</param>
/// <param name="endB">線分のエンド</param>
/// <returns></returns>
bool HitLine(Vector3 startA,Vector3 endA,Vector3 startB,Vector3 endB,std::string direction)//aが直線、bが線分,_sはstartの略,_lはlastの略
{
	if ( direction == "x" )
	{
		startB.x += 0.5f;
		endB.x += 0.5f;
	}
	else if ( direction == "-x" )
	{
		startB.x -= 0.5f;
		endB.x -= 0.5f;
	}
	else if ( direction == "z" )
	{
		startB.z += 0.5f;
		endB.z += 0.5f;
	}
	else if ( direction == "-z" )
	{
		startB.z -= 0.5f;
		endB.z -= 0.5f;
	}
	float s,t;
	s = ( startA.x - endA.x ) * ( endB.z - startA.z ) - ( startA.z - endA.z ) * ( endB.x - startA.x );
	t = ( startA.x - endA.x ) * ( startB.z - startA.z ) - ( startA.z - endA.z ) * ( startB.x - startA.x );
	if ( s * t > 0 )
	{
		return false;
	}

	s = ( endB.x - startB.x ) * ( startA.z - endB.z ) - ( endB.z - startB.z ) * ( startA.x - endB.x );
	t = ( endB.x - startB.x ) * ( endA.z - endB.z ) - ( endB.z - startB.z ) * ( endA.x - endB.x );
	if ( s * t > 0 )
	{
		return false;
	}
	return	true;
}
/// <summary>
/// 線分の判定
/// </summary>
/// <param name="startA">直線のスタート</param>
/// <param name="endA">直線のエンド</param>
/// <param name="startB">線分のスタート</param>
/// <param name="endB">線分のエンド</param>
/// <param name="direction">方向</param>
/// <returns></returns>
bool HitLine(Vector3 startA,Vector3 endA,Vector3 startB,Vector3 endB)//aが直線、bが線分,_sはstartの略,_lはlastの略
{
	
	float s,t;
	s = ( startA.x - endA.x ) * ( endB.z - startA.z ) - ( startA.z - endA.z ) * ( endB.x - startA.x );
	t = ( startA.x - endA.x ) * ( startB.z - startA.z ) - ( startA.z - endA.z ) * ( startB.x - startA.x );
	if ( s * t > 0 )
	{
		return false;
	}

	s = ( endB.x - startB.x ) * ( startA.z - endB.z ) - ( endB.z - startB.z ) * ( startA.x - endB.x );
	t = ( endB.x - startB.x ) * ( endA.z - endB.z ) - ( endB.z - startB.z ) * ( endA.x - endB.x );
	if ( s * t > 0 )
	{
		return false;
	}
	return	true;
}

void GamePlayScene::Initialize() {
	//ステージ番号
	StageString();

	//変数
	startCount = 0;
	isStart = true;
	spriteEaseTime = 0.0f;
	spriteWaitTime = 0.0f;
	rPosStartY=WinApp::height+150.0f;
	sPosStartY=-100.0f;
	mPosEndX = 50.0f;
	mPosStartX = -200.0f;

	//キー
	input=Input::GetInstance();
	
	//カメラ
	camera = std::make_unique<Camera>();
	camera->Initialize();
	Object3d::SetCamera(camera.get());

	//ライト
	light.reset(Light::Create());
	light->SetLightColor({ 1.0f,1.0f,1.0f });
	light->SetLightDir(lightDir);
	light->Update();
	Object3d::SetLight(light.get());

	//ジオメトリ
	geo.reset(BillboardParticle::Create());
	geo->SetCamera(camera.get());
	// モデル読み込み
	modelSkydome.reset(Model::LoadFromOBJ("skydome",true));
	cube.reset(Model::LoadFromOBJ("cube"));
	tank.reset(Model::LoadFromOBJ("tank"));
	had.reset(Model::LoadFromOBJ("TankHad"));
	body.reset(Model::LoadFromOBJ("TankBody"));
	modelMap.reset(Model::LoadFromOBJ("map"));
	fixedgun.reset(Model::LoadFromOBJ("fixedgun"));
	parachute.reset(Model::LoadFromOBJ("parachute"));
	wall.reset(Model::LoadFromOBJ("wall"));
	bullet.reset(Model::LoadFromOBJ("bullet"));

	//パーティクル
	particle = std::make_unique <ModelParticleManager>();
	particle->Initialize(cube.get());

	//弾マネージャー
	bulletManager=std::make_unique<BulletManager>();
	//敵
	enemyManager = std::make_unique<EnemyManager>();
	//プレイヤー
	player = std::make_unique<Player>();
	player->PlayInitialeze(had.get(),body.get(),parachute.get(),input,bulletManager.get());
	//壁
	blockManager = std::make_unique<BlockManager>();
	blockManager->Initialize(bulletManager.get());

	//弾マネージャー初期化
	bulletManager->Initialize(bullet.get(),player.get(),geo.get());
	//json読み込み
	jsonLoader = std::make_unique<LevelData>();
	jsonLoader.reset(LevelLoader::LoadJson(stageStr));
	models.insert(std::make_pair("Normal",tank.get()));
	models.insert(std::make_pair("Shotgun",tank.get()));
	models.insert(std::make_pair("block",wall.get()));
	models.insert(std::make_pair("fixedgun",fixedgun.get()));

	// レベルデータからオブジェクトを生成、配置
	for ( auto& objectData : jsonLoader->objects )
	{
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype( models )::iterator it = models.find(objectData.fileName);
		if ( it != models.end() )
		{
			model = it->second;
		}

		//エネミー
		if (objectData.fileName == "Shotgun" || objectData.fileName == "Normal" )
		{
			enemyManager->Add(objectData.fileName,model,parachute.get(),player.get(),objectData.translation,objectData.rotation,bulletManager.get());
		}
		//ブロック
		if ( objectData.fileName == "block" || objectData.fileName == "fixedgun" )
		{
			blockManager->Add(objectData.fileName,model,objectData.translation,objectData.rotation,objectData.scaling);
		}
	}
	//モデルのセット
	Map::StaticInitialize(modelMap.get());


	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelSkydome.get());	
	objSkydome->SetScale({ 200.0f,200.0f,200.0f });



	//マップ
	map = std::make_unique<Map>();
	map->Initialize(true);
	map->LoadCSV(stageStr);



	//スプライト
	rPosEndY = (static_cast<float>( WinApp::height) / 2.0f ) + 50.0f;
	sPosEndY= ( static_cast< float >( WinApp::height ) / 2.0f ) - 50.0f;

	//準備
	ready = std::make_unique<Sprite>();
	ready->Initialize(SpriteCommon::GetInstance(),7);
	ready->SetAnchorPoint({ 0.5f,0.5f });
	ready->SetPosition({ WinApp::width / 2.0f,rPosStartY });

	//ステージ
	stage = std::make_unique<Sprite>();
	stage->Initialize(SpriteCommon::GetInstance(),8);
	stage->SetAnchorPoint({ 0.5f,0.5f });
	stage->SetPosition({ 640.0f,sPosStartY });

	//照準
	sight = std::make_unique<Sprite>();
	sight->Initialize(SpriteCommon::GetInstance(),6);
	sight->SetAnchorPoint({ 0.5f,0.5f });

	//操作UI
	memo = std::make_unique<Sprite>();
	memo->Initialize(SpriteCommon::GetInstance(),11);
	memo->SetAnchorPoint({ 0.0f,1.0f });
	memo->SetPosition({ mPosStartX,( float ) WinApp::height-50.0f });

	//音
	SoundManager::GetInstance()->PlayWave("BGM/play.wav",0.2f,true);
}

void GamePlayScene::Update(){
	const XMFLOAT3 cameraPos = { player->GetPos().x, 100, player->GetPos().z - 30.0f };
	if ( !player->IsDead() && enemyManager->GetSize()!=0 )
	{
		sight->SetPosition({ input->GetMausePos().x,input->GetMausePos().y });
		sight->Update();
		
		camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
		camera->SetEye(cameraPos);
		camera->Update();
		player->Update();
		bulletManager->Update();
		enemyManager->Update();
		blockManager->Update();
		map->Update();
		objSkydome->SetPosition(player->GetPos());
		objSkydome->Update();
		particle->Update();
		geo->Update();

		if (!isStart)
		{
			CheckAllCollision();
		}
	}
	//シーン遷移のフラグを立てる
	else if ( !SceneTransition::GetInstance()->GetIsFadeOut() &&
		!SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		SceneTransition::GetInstance()->IsFadeOutTrue();
	}
	//画面真っ暗になったら
	else if (!SceneTransition::GetInstance()->GetIsFadeOut()&&
		SceneTransition::GetInstance()->GetIsFadeIn() )
	{
		//弾全削除
		bulletManager->AllBulletDelete();
		//音を止める
		SoundManager::GetInstance()->StopWave("BGM/play.wav");

		//シーンの切り替え
		if (player->IsDead())
		{
			SceneManager::GetInstance()->ChangeScene("GAMEOVER");
		}
		if (enemyManager->GetSize()==0)
		{
			SceneManager::playerHP = player->GetHp();
			SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
		}
	}
	StartStaging();
	MemoDisplay();
}

void GamePlayScene::SpriteDraw() {
	player->SpriteDraw();
	stage->Draw();
	ready->Draw();
	memo->Draw();
	sight->Draw();
}

void GamePlayScene::ObjDraw(){
	enemyManager->Draw();
	blockManager->Draw();
	bulletManager->Draw();
	objSkydome->Draw();
	player->ObjDraw();
	map->Draw();
	particle->Draw();
}

void GamePlayScene::GeometryDraw()
{
	geo->Draw();
}

void GamePlayScene::CheckAllCollision() {
		
	
		//自弾リストを取得
		const std::list<std::unique_ptr<Bullet>>& playerBullets = bulletManager->GetPlayerBullets();
		//敵弾リストを取得
		const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = bulletManager->GetEnemyBullets();
		
		//敵弾
		for ( const std::unique_ptr<EnemyBullet>& e_bullet : enemyBullets )
		{
			#pragma	region	自キャラとの当たり判定
			//判定対象AとBの座標
			Vector3 posA,posB;
			//自キャラの座標
			posA = player->GetPos();
			//敵弾の座標
			posB = e_bullet->GetPos();
			//A,Bの距離
			Vector3 vecPos = MyMath::lens(posA,posB);
			float dis = MyMath::Length(vecPos);
			//
			float	radius = player->GetRadius() + e_bullet->GetRadius();
			//判定
			if ( dis <= radius )
			{
				//自キャラのコールバックを呼び出し
				player->OnCollision();
				//敵弾のコールバックを呼び出し
				e_bullet->OnCollision();
				particle->Add("explosion",30,15,player->GetPos(),1.0f,0.0f);
			}
			#pragma	endregion

			#pragma region 自弾との当たり判定
			for ( const std::unique_ptr<Bullet>& p_bullet : playerBullets )
			{
				//自弾の座標
				posA = p_bullet->GetPos();
				
				// A,Bの距離
				vecPos = MyMath::lens(posA,posB);
				dis = MyMath::Length(vecPos);
				//AとBの半径の合計
				radius = e_bullet->GetRadius() + p_bullet->GetRadius();
				//判定
				if ( dis <= radius )
				{
					//自弾のコールバックを呼び出し
					p_bullet->OnCollision();
					//敵弾のコールバックを呼び出し
					e_bullet->OnCollision();
					particle->Add("explosion",5,10,p_bullet->GetPos(),1.0f,0.0f);
					particle->Add("explosion",5,10,e_bullet->GetPos(),1.0f,0.0f);
				}
			}
			#pragma endregion

			#pragma region ブロックとの当たり判定	
			for ( const std::unique_ptr<BaseBlock>& block : blockManager->GetBlocks() )
			{
				//自弾の座標
				posA = e_bullet->GetPos();
				//敵弾の座標
				posB = block->GetPos();

				//判定
				if ( CheckBoxXZ(posA,e_bullet->GetScale(),posB,block->GetScale()) )
				{
					//自弾のコールバックを呼び出し
					e_bullet->OnCollision();
					//パーティクルの呼び出し
					particle->Add("explosion",5,10,e_bullet->GetPos(),1.0f,0.0f);
				}
			}
			#pragma endregion
		}

		//敵
		for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys() )
		{
			Vector3 topLE,topRE,bottomRE,bottomLE;
			#pragma region 自弾との当たり判定
			for ( const std::unique_ptr<Bullet>& p_bullet : playerBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//敵弾の座標
				posA = enemy->GetPos();
				//自弾の座標
				posB = p_bullet->GetPos();
				// A,Bの距離
				Vector3 vecPos = MyMath::lens(posA,posB);
				float dis = MyMath::Length(vecPos);
				//
				float radius = enemy->GetRadius() + p_bullet->GetRadius();
				//判定
				if ( dis <= radius )
				{
					//敵キャラのコールバックを呼び出し
					enemy->OnCollision();
					//自弾のコールバックを呼び出し
					p_bullet->OnCollision();

					particle->Add("explosion",30,15,enemy->GetPos(),1.0f,0.0f);
				}
			}
			#pragma endregion

			#pragma region ブロックとの当たり判定
			for ( const std::unique_ptr<BaseBlock>& block : blockManager->GetBlocks() )
			{
				Vector3 topLB,topRB,bottomRB,bottomLB;
				/*プレイヤー*/
				//左奥
				topLE.x = enemy->GetPos().x - enemy->GetScale().x;
				topLE.z = enemy->GetPos().z + enemy->GetScale().z;
				//右奥
				topRE.x = enemy->GetPos().x + enemy->GetScale().x;
				topRE.z = enemy->GetPos().z + enemy->GetScale().z;

				//左前
				bottomLE.x = enemy->GetPos().x - enemy->GetScale().x;
				bottomLE.z = enemy->GetPos().z - enemy->GetScale().z;
				//右前
				bottomRE.x = enemy->GetPos().x + enemy->GetScale().x;
				bottomRE.z = enemy->GetPos().z - enemy->GetScale().z;

				/*ブロック*/
				//左奥
				topLB.x = block->GetPos().x - block->GetScale().x;
				topLB.z = block->GetPos().z + block->GetScale().z;
				//右奥
				topRB.x = block->GetPos().x + block->GetScale().x;
				topRB.z = block->GetPos().z + block->GetScale().z;

				//左前
				bottomLB.x = block->GetPos().x - block->GetScale().x;
				bottomLB.z = block->GetPos().z - block->GetScale().z;
				//右前
				bottomRB.x = block->GetPos().x + block->GetScale().x;
				bottomRB.z = block->GetPos().z - block->GetScale().z;
				//左辺と（上辺、下辺）の判定
				//右辺と（上辺、下辺）の判定
				if ( HitLine(topLE,bottomLE,bottomLB,bottomRB,"-z") || HitLine(topLE,bottomLE,topLB,topRB,"z")
					|| HitLine(topRE,bottomRE,bottomLB,bottomRB,"-z") || HitLine(topRE,bottomRE,topLB,topRB,"z") )
				{
					enemy->OnCollisionPos("z");

				}
				//上辺と（左辺、右辺）の判定
				//下辺と（左辺、右辺）の判定
				if ( HitLine(topLE,topRE,topLB,bottomLB,"-x") || HitLine(topLE,topRE,topRB,bottomRB,"x")
					|| HitLine(bottomRE,bottomLE,topLB,bottomLB,"-x") || HitLine(bottomRE,bottomLE,topRB,bottomRB,"x") )
				{
					enemy->OnCollisionPos("x");
				}
			}
			#pragma endregion

			#pragma region マップとの当たり判定
			{
				float diameterW = Map::mapScaleW * 2.0f;
				float diameterH = Map::mapScaleH * 2.0f;
				int16_t sizeW,sizeH;
				sizeW = static_cast< int16_t >( ( enemy->GetPos().x + Map::moveLimitW ) / diameterW );
				sizeH = static_cast< int16_t >( ( enemy->GetPos().z + Map::moveLimitH ) / diameterH );
				Vector3 topLB,topRB,bottomRB,bottomLB;
				/*プレイヤー*/
				//左奥
				topLE.x = enemy->GetPos().x - enemy->GetScale().x;
				topLE.z = enemy->GetPos().z + enemy->GetScale().z;
				//右奥
				topRE.x = enemy->GetPos().x + enemy->GetScale().x;
				topRE.z = enemy->GetPos().z + enemy->GetScale().z;

				//左前
				bottomLE.x = enemy->GetPos().x - enemy->GetScale().x;
				bottomLE.z = enemy->GetPos().z - enemy->GetScale().z;
				//右前
				bottomRE.x = enemy->GetPos().x + enemy->GetScale().x;
				bottomRE.z = enemy->GetPos().z - enemy->GetScale().z;

				for ( int16_t h = -1; h <= 1; h++ )
				{
					for ( int16_t w = -1; w <= 1; w++ )
					{
						if ( sizeW + w < Map::width && sizeW + w >= 0
							&& sizeH + h < Map::height && sizeH + h >= 0 )
						{
							BaseBlock& block = map->GetBlocks(sizeW + w,sizeH + h);
							if ( block.GetDrawNum() == 0 )
							{
								/*ブロック*/
								//左奥
								topLB.x = block.GetPos().x - Map::mapScaleW;
								topLB.z = block.GetPos().z + Map::mapScaleH;
								//右奥
								topRB.x = block.GetPos().x + Map::mapScaleW;
								topRB.z = block.GetPos().z + Map::mapScaleH;

								//左前
								bottomLB.x = block.GetPos().x - Map::mapScaleW;
								bottomLB.z = block.GetPos().z - Map::mapScaleH;
								//右前
								bottomRB.x = block.GetPos().x + Map::mapScaleW;
								bottomRB.z = block.GetPos().z - Map::mapScaleH;
								//左辺と（上辺、下辺）の判定
								//右辺と（上辺、下辺）の判定
								if ( HitLine(topLE,bottomLE,bottomLB,bottomRB,"-z") || HitLine(topLE,bottomLE,topLB,topRB,"z")
									|| HitLine(topRE,bottomRE,bottomLB,bottomRB,"-z") || HitLine(topRE,bottomRE,topLB,topRB,"z") )
								{
									enemy->OnCollisionPos("z");

								}
								//上辺と（左辺、右辺）の判定
								//下辺と（左辺、右辺）の判定
								if ( HitLine(topLE,topRE,topLB,bottomLB,"-x") || HitLine(topLE,topRE,topRB,bottomRB,"x")
									|| HitLine(bottomRE,bottomLE,topLB,bottomLB,"-x") || HitLine(bottomRE,bottomLE,topRB,bottomRB,"x") )
								{
									enemy->OnCollisionPos("x");
								}
							}
						}
					}
				}
			}
			#pragma endregion
		}

		//ブロック
		for ( const std::unique_ptr<BaseBlock>& block : blockManager->GetBlocks() )
		{
			
			#pragma region 自弾との当たり判定
			for ( const std::unique_ptr<Bullet>& p_bullet : playerBullets )
			{
				//判定対象AとBの座標
				Vector3 posA,posB;
				//自弾の座標
				posA = p_bullet->GetPos();
				//ブロック
				posB = block->GetPos();				
				
				//判定
				if ( CheckBoxXZ(posA,p_bullet->GetScale(),posB,block->GetScale()) )
				{
					//自弾のコールバックを呼び出し
					p_bullet->OnCollision();
					//パーティクルの呼び出し
					particle->Add("1",5,10,p_bullet->GetPos(),1.0f,0.0f);
				}
			}
			#pragma endregion

			#pragma region 自機との当たり判定
			Vector3 topLP,topRP,bottomRP,bottomLP;
			Vector3 topLB,topRB,bottomRB,bottomLB;
			/*プレイヤー*/
			//左奥
			topLP.x = player->GetPos().x - player->GetScale().x;
			topLP.z = player->GetPos().z + player->GetScale().z;
			//右奥
			topRP.x = player->GetPos().x + player->GetScale().x;
			topRP.z = player->GetPos().z + player->GetScale().z;

			//左前
			bottomLP.x = player->GetPos().x - player->GetScale().x;
			bottomLP.z = player->GetPos().z - player->GetScale().z;
			//右前
			bottomRP.x = player->GetPos().x + player->GetScale().x;
			bottomRP.z = player->GetPos().z - player->GetScale().z;

			/*ブロック*/
			//左奥
			topLB.x = block->GetPos().x - block->GetScale().x;
			topLB.z = block->GetPos().z + block->GetScale().z;
			//右奥
			topRB.x = block->GetPos().x + block->GetScale().x;
			topRB.z = block->GetPos().z + block->GetScale().z;

			//左前
			bottomLB.x = block->GetPos().x - block->GetScale().x;
			bottomLB.z = block->GetPos().z - block->GetScale().z;
			//右前
			bottomRB.x = block->GetPos().x + block->GetScale().x;
			bottomRB.z = block->GetPos().z - block->GetScale().z;
			//左辺と（上辺、下辺）の判定
			//右辺と（上辺、下辺）の判定
			if ( HitLine(topLP,bottomLP,bottomLB,bottomRB,"-z") || HitLine(topLP,bottomLP,topLB,topRB,"z")
				||HitLine(topRP,bottomRP,bottomLB,bottomRB,"-z") || HitLine(topRP,bottomRP,topLB,topRB,"z") )
			{
				player->OnCollisionPos("z");
				
			}
			//上辺と（左辺、右辺）の判定
			//下辺と（左辺、右辺）の判定
			if ( HitLine(topLP,topRP,topLB,bottomLB,"-x") || HitLine(topLP,topRP,topRB,bottomRB,"x")
				|| HitLine(bottomRP,bottomLP,topLB,bottomLB,"-x") || HitLine(bottomRP,bottomLP,topRB,bottomRB,"x") )
			{
				player->OnCollisionPos("x");
			}
			#pragma endregion
			
		}

		//マップ
		{
			float diameterW = Map::mapScaleW * 2.0f;
			float diameterH = Map::mapScaleH * 2.0f;
			int16_t sizeW,sizeH;
			sizeW = static_cast< int16_t >( ( player->GetPos().x + Map::moveLimitW ) / diameterW );
			sizeH = static_cast< int16_t >( ( player->GetPos().z + Map::moveLimitH ) / diameterH );
			Vector3 topLP,topRP,bottomRP,bottomLP;
			Vector3 topLB,topRB,bottomRB,bottomLB;
			/*プレイヤー*/
			//左奥
			topLP.x = player->GetPos().x - player->GetScale().x;
			topLP.z = player->GetPos().z + player->GetScale().z;
			//右奥
			topRP.x = player->GetPos().x + player->GetScale().x;
			topRP.z = player->GetPos().z + player->GetScale().z;

			//左前
			bottomLP.x = player->GetPos().x - player->GetScale().x;
			bottomLP.z = player->GetPos().z - player->GetScale().z;
			//右前
			bottomRP.x = player->GetPos().x + player->GetScale().x;
			bottomRP.z = player->GetPos().z - player->GetScale().z;

			for ( int16_t h = -1; h <= 1; h++ )
			{
				for ( int16_t w = -1; w <= 1; w++ )
				{
					if ( sizeW + w < Map::width &&sizeW + w >= 0
						&&sizeH + h < Map::height&&sizeH + h >= 0 )
					{
						BaseBlock& block = map->GetBlocks(sizeW + w,sizeH + h);
						if ( block.GetDrawNum() == 0 )
						{
							/*ブロック*/
							//左奥
							topLB.x = block.GetPos().x - Map::mapScaleW;
							topLB.z = block.GetPos().z + Map::mapScaleH;
							//右奥
							topRB.x = block.GetPos().x + Map::mapScaleW;
							topRB.z = block.GetPos().z + Map::mapScaleH;

							//左前
							bottomLB.x = block.GetPos().x - Map::mapScaleW;
							bottomLB.z = block.GetPos().z - Map::mapScaleH;
							//右前
							bottomRB.x = block.GetPos().x + Map::mapScaleW;
							bottomRB.z = block.GetPos().z - Map::mapScaleH;
							//左辺と（上辺、下辺）の判定
							//右辺と（上辺、下辺）の判定
							if ( HitLine(topLP,bottomLP,bottomLB,bottomRB,"-z") || HitLine(topLP,bottomLP,topLB,topRB,"z")
								|| HitLine(topRP,bottomRP,bottomLB,bottomRB,"-z") || HitLine(topRP,bottomRP,topLB,topRB,"z") )
							{
								player->OnCollisionPos("z");

							}
							//上辺と（左辺、右辺）の判定
							//下辺と（左辺、右辺）の判定
							if ( HitLine(topLP,topRP,topLB,bottomLB,"-x") || HitLine(topLP,topRP,topRB,bottomRB,"x")
								|| HitLine(bottomRP,bottomLP,topLB,bottomLB,"-x") || HitLine(bottomRP,bottomLP,topRB,bottomRB,"x") )
							{
								player->OnCollisionPos("x");
							}
						}
					}
				}
			}
		}
}

void GamePlayScene::StartStaging() {
	if ( startCount >= start::Go)
	{
		isStart = false;
	}
	if ( startCount== start::Redy )
	{
		waitTime++;
		if ( waitTime >= waitTimer )
		{
			XMFLOAT2 readyPos = ready->GetPosition();
			XMFLOAT2 stagePos = stage->GetPosition();
			XMFLOAT2 memoPos = memo->GetPosition();

			//イージングを一時的に止める
			if ( spriteEaseTime == spriteEaseTimer )
			{
				spriteWaitTime++;
			}
			//イージング
			if ( spriteWaitTime >= spriteWaitTimer || spriteWaitTime == 0.0f )
			{
				spriteEaseTime++;
				readyPos.y = rPosStartY + ( rPosEndY - rPosStartY ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
				stagePos.y = sPosStartY + ( sPosEndY - sPosStartY ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
				if ( spriteEaseTime <= spriteEaseTimer )
				{
					memoPos.x = mPosStartX + ( mPosEndX - mPosStartX ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
				}
				//スプライトが画面外にいく時間になったら
				if ( spriteEaseTime == ( spriteEaseTimer * 2.0f ) )
				{
					startCount++;
					spriteWaitTime = 0;
					spriteEaseTime = spriteEaseTimer;
					isDisplay = true;
				}
			}			
			ready->SetPosition(readyPos);
			stage->SetPosition(stagePos);
			memo->SetPosition(memoPos);
		}
	}
	stage->Update();
	ready->Update();
	memo->Update();
}

void GamePlayScene::MemoDisplay()
{
	if ( !isStart )
	{
		XMFLOAT2 memoPos = memo->GetPosition();
		//押したら
		if ( input->TriggerKey(DIK_TAB) && !isSlide )
		{
			isDisplay ^= 1;
			isSlide = true;
		}

		if ( isSlide )
		{
			if ( isDisplay )
			{
				spriteEaseTime++;
			}
			else
			{
				spriteEaseTime--;
			}
			memoPos.x = mPosStartX + ( mPosEndX - mPosStartX ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
			if ( spriteEaseTime> spriteEaseTimer||spriteEaseTime<0.0f)
			{
				isSlide = false;
			}
			memo->SetPosition(memoPos);
			memo->Update();
		}
	}
}

void GamePlayScene::Finalize(){}

GamePlayScene::GamePlayScene() {}
GamePlayScene::~GamePlayScene() {
	Finalize();
}

void GamePlayScene::StageString()
{
	if ( SceneManager::stage == 0 )
	{
		stageStr = "1";
	}
	else if ( SceneManager::stage == 1 )
	{
		stageStr = "2";
	}
	else
	{
		stageStr = "1";
	}
}
