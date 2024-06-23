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
#include<random>

int8_t GamePlayScene::startCount = 0;
bool GamePlayScene::isStart = true;
int8_t GamePlayScene::outCount = 0;
bool GamePlayScene::isOut = false;
bool GamePlayScene::isSlow = false;
int32_t GamePlayScene::clearCount = 0;
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
bool HitObj(Vector3 startA,Vector3 endA,Vector3 startB,Vector3 endB,std::string direction)//aが直線、bが線分,_sはstartの略,_lはlastの略
{
	float plus = 0.5f;
	if ( direction == "x" )
	{
		startB.x += plus;
		endB.x += plus;
	}
	else if ( direction == "-x" )
	{
		startB.x -= plus;
		endB.x -= plus;
	}
	else if ( direction == "z" )
	{
		startB.z += plus;
		endB.z += plus;
	}
	else if ( direction == "-z" )
	{
		startB.z -= plus;
		endB.z -= plus;
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
	modelM = ModelManager::GetInstance();

	//変数
	startCount = 0;
	isStart = true;
	outCount = Serial::None;
	isOut = false;
	isSlow = false;
	spriteEaseTime = 0.0f;
	spriteWaitTime = 0.0f;
	rPosStartY=WinApp::height+150.0f;
	mPosEndX = 50.0f;
	mPosStartX = -200.0f;
	clearCount = 0;
	mSpeed = 1.0f;
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
	billParticle.reset(BillboardParticle::Create());
	billParticle->SetCamera(camera.get());

	//パーティクル
	particle = std::make_unique <ModelParticleManager>();
	particle->Initialize(modelM->GetModel("cube"));

	//弾マネージャー
	bulletManager=std::make_unique<BulletManager>();
	//マップ
	map = std::make_unique<Map>();
	map->Initialize(true,modelM->GetModel("map"));
	map->LoadCSV("1");
	
	//プレイヤー
	player = std::make_unique<Player>();
	player->Initialeze(input,bulletManager.get(),map.get());
	

	//A*
	aStar = std::make_unique<AStar>();
	//敵
	enemyManager = std::make_unique<EnemyManager>();
	enemyManager->Initialize(player.get(),bulletManager.get());
	//壁
	blockManager = std::make_unique<BlockManager>();
	blockManager->Initialize(bulletManager.get(),map.get(),enemyManager.get(),player.get(),particle.get());
	
	//ブロック生成後にポインタを渡す
	aStar->Initialize(blockManager.get());

	//弾マネージャー初期化
	bulletManager->Initialize(modelM->GetModel("bullet"),player.get(),billParticle.get());
	//json読み込み
	jsonLoader.reset(LevelLoader::LoadJson("1"));
	models.insert(std::make_pair("normal",modelM->GetModel("enemy")));
	models.insert(std::make_pair("shotgun",modelM->GetModel("enemy")));
	models.insert(std::make_pair("block",modelM->GetModel("wall")));
	models.insert(std::make_pair("fixedgun",modelM->GetModel("fixedgun")));

	//配置
	PlaceObj();

	//スカイドーム
	objSkydome = std::make_unique<Object3d>();
	objSkydome->Initialize();
	objSkydome->SetModel(modelM->GetModel("skydome"));
	objSkydome->SetScale({ 250.0f,200.0f,250.0f });

	//スプライト
	rPosEndY = (static_cast<float>( WinApp::height) / 2.0f ) + 50.0f;

	//準備
	ready = std::make_unique<Sprite>();
	ready->Initialize(SpriteCommon::GetInstance(),7);
	ready->SetAnchorPoint({ 0.5f,0.5f });
	ready->SetPosition({ WinApp::width / 2.0f,rPosStartY });

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

void GamePlayScene::Update() {
	StartStaging();
	OutStaging();
	MemoDisplay();
	SlowMotion();
	shake = { 0.0f,0.0f,0.0f };
	if(player->IsShake() )
	{
		float num = 0.4f;
		/*シェイク処理*/
		//カメラ位置
		//乱数シード生成器
		std::random_device seed_gen;
		//メルセンヌ・ツイスターの乱数エンジン
		std::mt19937_64 engine(seed_gen());
		//乱数　（回転）
		std::uniform_real_distribution<float> posDist(-num,num);
		//乱数エンジンを渡し、指定範囲かっランダムな数値を得る
		shake = { posDist(engine),0.0f,posDist(engine) };
	}
	cameraPos ={ player->GetPos().x+ shake.x,cameraY,player->GetPos().z - 30.0f+ shake.z };
	if ( !isEnemy )
	{
		camera->SetTarget({ player->GetPos().x, player->GetPos().y, player->GetPos().z });
	}
	else
	{
		for ( std::unique_ptr<BaseEnemy>& enemy : enemyManager->GetEnemys() )
		{
			camera->SetTarget({ enemy->GetPos().x, enemy->GetPos().y, enemy->GetPos().z });
		}
	
	}
	camera->SetEye(cameraPos);
	camera->Update();
	bulletManager->Update();
	player->Update();
	enemyManager->Update();
	blockManager->Update();
	map->Update();
	objSkydome->SetPosition(player->GetPos());
	objSkydome->Update();
	particle->Update();
	billParticle->Update();

	if ( !player->IsDead() )
	{
		sight->SetPosition({ input->GetMausePos().x,input->GetMausePos().y });
		sight->Update();
		if (!isStart&&!isOut&&!isSlow)
		{
			CheckAllCollision();
		}
	}

	BlackOut();

	if ( enemyManager->GetSize() == 0 &&!isOut&&!isSlow)
	{
		isOut = true;
		outCount = Serial::None;
	}
}

void GamePlayScene::SpriteDraw() {
	player->SpriteDraw();
	ready->Draw();
	memo->Draw();
	sight->Draw();
	aStar->Draw();
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
	billParticle->Draw();
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
				//HPが2以上であれば
				if ( player->GetHp() > 1 )
				{
					//自キャラのコールバックを呼び出し
					player->OnCollision();
					//敵弾のコールバックを呼び出し
					e_bullet->OnCollision();
					particle->Add("explosion",30,15,player->GetPos(),1.0f,0.0f);
				}
				else
				{
					eBullet = e_bullet.get();
					isSlow = true;
					isPlayer = true;
				}
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
					//敵弾のコールバックを呼び出し
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
			size_t count = 0;
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
					//２体以上いれば
					if ( enemyManager->GetSize()>=2 )
					{
						//敵キャラのコールバックを呼び出し
						enemy->OnCollision();
						//自弾のコールバックを呼び出し
						p_bullet->OnCollision();

					}
					else//残り1体で
					{
						//HPが2以上であれば
						if ( enemy->GetHP()>1 )
						{
							//敵キャラのコールバックを呼び出し
							enemy->OnCollision();
							//自弾のコールバックを呼び出し
							p_bullet->OnCollision();
						}
						else
						{
							pickEnemy = enemy.get();
							bullet = p_bullet.get();
							isSlow = true;
							isEnemy = true;
						}
					}

					particle->Add("explosion",30,15,enemy->GetPos(),1.0f,0.0f);
				}
			}
			#pragma endregion
			
			for ( const std::unique_ptr<BaseBlock>& block : blockManager->GetBlocks() )
			{
				#pragma region ブロックとの当たり判定
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
				if ( HitObj(topLE,bottomLE,bottomLB,bottomRB,"-z") || HitObj(topLE,bottomLE,topLB,topRB,"z")
					|| HitObj(topRE,bottomRE,bottomLB,bottomRB,"-z") || HitObj(topRE,bottomRE,topLB,topRB,"z") )
				{
					enemy->OnCollisionPos("z");

				}
				//上辺と（左辺、右辺）の判定
				//下辺と（左辺、右辺）の判定
				if ( HitObj(topLE,topRE,topLB,bottomLB,"-x") || HitObj(topLE,topRE,topRB,bottomRB,"x")
					|| HitObj(bottomRE,bottomLE,topLB,bottomLB,"-x") || HitObj(bottomRE,bottomLE,topRB,bottomRB,"x") )
				{
					enemy->OnCollisionPos("x");
				}
				#pragma endregion

				#pragma region 敵と自機の間にブロックがあるかの判定
				Vector3 len;
				float lenght;
				float decisionLen = 70.0f;
				//長さを算出
				len = enemy->GetPos() - player->GetPos();
				lenght = MyMath::Length(len);
				//長さが規定値以下なら
				if ( lenght <= decisionLen )
				{
					float plus = 0.2f;
					//ブロックの位置を代入
					Vector3 startX,endX,startZ,endZ;
					startX = block->GetPos();
					endX = block->GetPos();
					startZ = block->GetPos();
					endZ=block->GetPos();
					//補正
					startX.x-= block->GetScale().x+plus;
					endX.x+= block->GetScale().x + plus;
					startZ.z-= block->GetScale().z + plus;
					endZ.z+=block->GetScale().z + plus;
					if ( !HitLine(startX,endX,player->GetPos(),enemy->GetPos())
						&& !HitLine(startZ,endZ,player->GetPos(),enemy->GetPos()) )
					{
						count++;
					}
				}
				
			}
			if ( count== blockManager->GetSize() )
			{
				enemy->OffCollisionShot();
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

				/*敵*/
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
								if ( HitObj(topLE,bottomLE,bottomLB,bottomRB,"-z") || HitObj(topLE,bottomLE,topLB,topRB,"z")
									|| HitObj(topRE,bottomRE,bottomLB,bottomRB,"-z") || HitObj(topRE,bottomRE,topLB,topRB,"z") )
								{
									enemy->OnCollisionPos("z");

								}
								//上辺と（左辺、右辺）の判定
								//下辺と（左辺、右辺）の判定
								if ( HitObj(topLE,topRE,topLB,bottomLB,"-x") || HitObj(topLE,topRE,topRB,bottomRB,"x")
									|| HitObj(bottomRE,bottomLE,topLB,bottomLB,"-x") || HitObj(bottomRE,bottomLE,topRB,bottomRB,"x") )
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
					//ブロックのコールバックを呼び出し
					block->OnCollision();
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
			if ( HitObj(topLP,bottomLP,bottomLB,bottomRB,"-z") || HitObj(topLP,bottomLP,topLB,topRB,"z")
				||HitObj(topRP,bottomRP,bottomLB,bottomRB,"-z") || HitObj(topRP,bottomRP,topLB,topRB,"z") )
			{
				player->OnCollisionPos("z");
				
			}
			//上辺と（左辺、右辺）の判定
			//下辺と（左辺、右辺）の判定
			if ( HitObj(topLP,topRP,topLB,bottomLB,"-x") || HitObj(topLP,topRP,topRB,bottomRB,"x")
				|| HitObj(bottomRP,bottomLP,topLB,bottomLB,"-x") || HitObj(bottomRP,bottomLP,topRB,bottomRB,"x") )
			{
				player->OnCollisionPos("x");
			}
			#pragma endregion
			
		}

		//マップ
		{
			//float diameterW = Map::mapScaleW * 2.0f;
			//float diameterH = Map::mapScaleH * 2.0f;
			//int16_t sizeW,sizeH;
			//sizeW = static_cast< int16_t >( ( player->GetPos().x + Map::moveLimitW ) / diameterW );
			//sizeH = static_cast< int16_t >( ( player->GetPos().z + Map::moveLimitH ) / diameterH );
			//Vector3 topLP,topRP,bottomRP,bottomLP;
			//Vector3 topLB,topRB,bottomRB,bottomLB;
			///*プレイヤー*/
			////左奥
			//topLP.x = player->GetPos().x - player->GetScale().x;
			//topLP.z = player->GetPos().z + player->GetScale().z;
			////右奥
			//topRP.x = player->GetPos().x + player->GetScale().x;
			//topRP.z = player->GetPos().z + player->GetScale().z;

			////左前
			//bottomLP.x = player->GetPos().x - player->GetScale().x;
			//bottomLP.z = player->GetPos().z - player->GetScale().z;
			////右前
			//bottomRP.x = player->GetPos().x + player->GetScale().x;
			//bottomRP.z = player->GetPos().z - player->GetScale().z;

			//for ( int16_t h = -1; h <= 1; h++ )
			//{
			//	for ( int16_t w = -1; w <= 1; w++ )
			//	{
			//		if ( sizeW + w < Map::width &&sizeW + w >= 0
			//			&&sizeH + h < Map::height&&sizeH + h >= 0 )
			//		{
			//			BaseBlock& block = map->GetBlocks(sizeW + w,sizeH + h);
			//			if ( block.GetDrawNum() == 0 )
			//			{
			//				/*ブロック*/
			//				//左奥
			//				topLB.x = block.GetPos().x - Map::mapScaleW;
			//				topLB.z = block.GetPos().z + Map::mapScaleH;
			//				//右奥
			//				topRB.x = block.GetPos().x + Map::mapScaleW;
			//				topRB.z = block.GetPos().z + Map::mapScaleH;

			//				//左前
			//				bottomLB.x = block.GetPos().x - Map::mapScaleW;
			//				bottomLB.z = block.GetPos().z - Map::mapScaleH;
			//				//右前
			//				bottomRB.x = block.GetPos().x + Map::mapScaleW;
			//				bottomRB.z = block.GetPos().z - Map::mapScaleH;
			//				//左辺と（上辺、下辺）の判定
			//				//右辺と（上辺、下辺）の判定
			//				if ( HitObj(topLP,bottomLP,bottomLB,bottomRB,"-z") || HitObj(topLP,bottomLP,topLB,topRB,"z")
			//					|| HitObj(topRP,bottomRP,bottomLB,bottomRB,"-z") || HitObj(topRP,bottomRP,topLB,topRB,"z") )
			//				{
			//					player->OnCollisionPos("z");

			//				}
			//				//上辺と（左辺、右辺）の判定
			//				//下辺と（左辺、右辺）の判定
			//				if ( HitObj(topLP,topRP,topLB,bottomLB,"-x") || HitObj(topLP,topRP,topRB,bottomRB,"x")
			//					|| HitObj(bottomRP,bottomLP,topLB,bottomLB,"-x") || HitObj(bottomRP,bottomLP,topRB,bottomRB,"x") )
			//				{
			//					player->OnCollisionPos("x");
			//				}
			//			}
			//		}
			//	}
			//}
		}
}

void GamePlayScene::StartStaging() {
	if ( startCount==start::Wait )
	{
		waitTime++;
		if ( waitTime >= waitTimer )
		{
			startCount++;
		}
	}
	else if ( startCount== start::Redy )
	{		
		XMFLOAT2 readyPos = ready->GetPosition();
		XMFLOAT2 memoPos = memo->GetPosition();

		//イージングを一時的に止める
		if ( spriteEaseTime == spriteEaseTimer )
		{
			//一番最初のスタート演出
			if ( !isOut )
			{
				spriteWaitTime++;
			}
			//落ちて上がってを繰り返す方の演出
			else
			{
				if ( outCount < Serial::FallEnemy )
				{
					//イージングのタイマーを動かさないようにするために1.0fを代入
					spriteWaitTime = 1.0f;
				}
				else
				{
					//敵の落ち始めてからタイマーを動かす
					spriteWaitTime++;
				}
			}

		}
		//イージング
		if ( spriteWaitTime >= spriteWaitTimer || spriteWaitTime == 0.0f )
		{
			spriteEaseTime++;
			readyPos.y = rPosStartY + ( rPosEndY - rPosStartY ) * Easing::easeOutCirc(spriteEaseTime / spriteEaseTimer);
			//操作説明の画像のイージング
			if ( spriteEaseTime <= spriteEaseTimer&&!isOut )
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
		memo->SetPosition(memoPos);
	}
	else if ( startCount >= start::Go)
	{
		isStart = false;
	}
	ready->Update();
	memo->Update();
}

void GamePlayScene::OutStaging()
{
	if ( isOut )
	{
		if ( outCount==Serial::None )
		{
			outCount++;
			map->CenterMapChip(player->GetPos());
			player->LocationMapChip(map.get());
		}
		else if ( outCount == Serial::Create )
		{
			clearCount++;
			startCount = start::Redy;
			spriteEaseTime = 0.0f;

			//生成と配置
			UseJson();

			aStar->Initialize(blockManager.get());
			outCount++;
		}
		else if ( outCount>=Serial::Max&& startCount >= start::Go )
		{
			isOut = false;
			
		}

	}
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

void GamePlayScene::UseJson()
{
	int32_t num = 3;
	int32_t max = 2;
	//int32_t stage = 1;
	bool flag = false;
	//３回クリアするごとに新しいものを追加
	if ( clearCount % num == 0  )
	{
		flag = true;
		//stage++;
	}
	if ( flag && clearCount / num <=max)
	{
		//データをクリア
		jsonLoader->objects.clear();
		//読み込みと配置
		map->LoadCSV("1");
		//プレイヤーの初期化とマップの生成後にマップチップの中心を算出
		jsonLoader.reset(LevelLoader::LoadJson("1"));
		PlaceObj();
		map->CenterMapChip(player->GetPos());

	}
	else
	{
		//ランダム生成、配置
		map->RandomCreate();
		player->RandomDeployment(map.get());
		//プレイヤーの初期化とマップの生成後にマップチップの中心を算出
		map->CenterMapChip(player->GetPos());
		enemyManager->RandomCreate(map.get());
		blockManager->RandomCreate();
	}
}

void GamePlayScene::PlaceObj()
{
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
		//自機
		if ( objectData.fileName == "player" )
		{
			player->SetParameter(objectData.translation,objectData.rotation);
			//マップの中心を算出
			map->CenterMapChip(player->GetPos());
		}

		//エネミー
		if ( objectData.fileName == "shotgun" || objectData.fileName == "normal" )
		{
			enemyManager->Add(objectData.fileName,model,
				objectData.translation,objectData.rotation);
		}
		//ブロック
		if ( objectData.fileName == "block" || objectData.fileName == "fixedgun" )
		{
			blockManager->Add(objectData.fileName,model,objectData.translation,objectData.rotation,objectData.scaling);
		}
	}
}

void GamePlayScene::SlowMotion()
{
	if ( isSlow )
	{
		mSpeed = 0.0f;
		player->SetMotionSpeed(mSpeed);
		enemyManager->SetMotionSpeed(mSpeed);
		bulletManager->SetMotionSpeed(mSpeed);
		//イージングを一時的に止める
		if ( slowTime == slowTimer )
		{
			slowWaitTime++;
			//止めているタイマーが半分になったら
			if ( slowWaitTime == slowWaitTimer /2.0f )
			{
				if ( isEnemy )
				{
					particle->Add("explosion",30,15,pickEnemy->GetPos(),1.0f,0.0f);
					pickEnemy->OnCollision();
					bullet->OnCollision();
				}
				if ( isPlayer )
				{
					particle->Add("explosion",30,15,player->GetPos(),1.0f,0.0f);
					player->OnCollision();
					eBullet->OnCollision();
				}
			}
		}
		if ( slowWaitTime >= slowWaitTimer || slowWaitTime == 0.0f )
		{
			slowTime++;
			float fovAngle;
			//固定
			if ( isPlayer && slowTime >= slowTimer )fovAngle = endFovAngle;
			//画角のイージング
			else fovAngle = startFovAngle + ( endFovAngle - startFovAngle ) * Easing::easeOutCirc(slowTime / slowTimer);
			
			camera->SetFovAngle(fovAngle);
			//画角が元に戻ったら
			if ( slowTime == ( slowTimer * 2.0f ) )
			{
				slowWaitTime = 0;
				slowTime = 0.0f;
				isSlow = false;
				isEnemy = false;
				isPlayer = false;
				mSpeed = 1.0f;
				player->SetMotionSpeed(mSpeed);
				enemyManager->SetMotionSpeed(mSpeed);
				bulletManager->SetMotionSpeed(mSpeed);
			}
		}

	}
}

void GamePlayScene::BlackOut()
{
	if ( player->IsDead() )
	{
	//シーン遷移のフラグを立てる
		if ( !SceneTransition::GetInstance()->GetIsFadeOut() &&
			!SceneTransition::GetInstance()->GetIsFadeIn()
			&& !isSlow )
		{
			SceneTransition::GetInstance()->IsFadeOutTrue();
		}
		//画面真っ暗になったら
		else if ( !SceneTransition::GetInstance()->GetIsFadeOut() &&
			SceneTransition::GetInstance()->GetIsFadeIn() )
		{
			//弾全削除
			bulletManager->AllBulletDelete();
			//音を止める
			SoundManager::GetInstance()->StopWave("BGM/play.wav");

			//シーンの切り替え
			if ( player->IsDead() )
			{
				SceneManager::GetInstance()->ChangeScene("GAMEOVER");
			}
			/*if (enemyManager->GetSize()==0)
			{
				SceneManager::playerHP = player->GetHp();
				SceneManager::GetInstance()->ChangeScene("GAMECLEAR");
			}*/
		}
	}
}

void GamePlayScene::Finalize(){}

GamePlayScene::GamePlayScene() {}
GamePlayScene::~GamePlayScene() {
	Finalize();
}