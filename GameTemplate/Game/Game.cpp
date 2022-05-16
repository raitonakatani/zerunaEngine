#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Title.h"
#include "Background.h"
#include "Floor.h"

#include "TankEnemy.h"
#include "TankEnemy2.h"
#include "SpeedEnemy.h"
#include "Enemy3.h"

#include "Fade.h"
#include "index.h"
#include "GameClear.h"
#include "Retry.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"


Game::~Game()
{
	const auto& indexs = FindGOs<index>("index");
	for (auto index : indexs)
	{
		DeleteGO(index);
	}
	const auto& enemys = FindGOs<TankEnemy>("TankEnemy");
	for (auto enemy : enemys)
	{
		DeleteGO(enemy);
	}
	const auto& tkenemys = FindGOs<TankEnemy2>("TankEnemy2");
	for (auto tkenemy : tkenemys)
	{
		DeleteGO(tkenemy);
	}
	const auto& enemys2 = FindGOs<SpeedEnemy>("m_enemy2");
	for (auto enemy2 : enemys2)
	{
		DeleteGO(enemy2);
	}
	const auto& enemys3 = FindGOs<Enemy3>("m_enemy3");
	for (auto enemy3 : enemys3)
	{
		DeleteGO(enemy3);
	}

	DeleteGO(m_gameCamera);
	DeleteGO(m_player);
	DeleteGO(m_background);
	DeleteGO(m_floor);
	DeleteGO(GameBGM);
}
bool Game::Start()
{
//	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });

	
//	m_bgModelRendedr.Init("Assets/modelData/karisute/stage.yuka.tkm");
//	m_bgObject.CreateFromModel(m_bgModelRendedr.GetModel(), m_bgModelRendedr.GetWorldMatrix(0));

	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	m_floor = NewGO<Floor>(0, "floor");
	m_floor->SetPosition({1.0f,1.0f,1.0f});
	m_floor->SetRotation({ 0.0f,0.0f,0.0f ,0.0f});
	m_floor->SetScale({ 1.0f,1.0f,1.0f });


	//レベルを構築する。
	m_levelRender.Init("Assets/Level/BackGround2.tkl", [&](LevelObjectData& objData) {
		
		if (objData.EqualObjectName(L"stage") == true) {

			m_background = NewGO<Background>(0, "background");
			m_background->SetPosition(objData.position);
			m_background->SetRotation(objData.rotation);
			m_background->SetScale(objData.scale);
			//trueにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}
		
		if (objData.EqualObjectName(L"index") == true) {

			m_index = NewGO<index>(0, "index");
			m_index->SetPosition(objData.position);
			m_index->SetRotation(objData.rotation);
			m_index->SetScale(objData.scale);
			//trueにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		if (objData.ForwardMatchName(L"tank") == true) {
			//エネミーのインスタンスを生成する。
			m_tank = NewGO<TankEnemy>(0, "TankEnemy");
			m_tank->SetPosition({ objData.position });
			m_tank->SetRotation(objData.rotation);
			m_tank->SetScale(objData.scale);
			//番号を設定する。
			m_tank->SettankNumber(objData.number);
			//falseにすると、レベルの方でモデルが読み込まれない。
			return true;
		}

		if (objData.ForwardMatchName(L"secondtank") == true) {
			//エネミーのインスタンスを生成する。
			m_tank2 = NewGO<TankEnemy2>(0, "TankEnemy2");
			m_tank2->SetPosition( { objData.position });
			m_tank2->SetRotation(objData.rotation);
			m_tank2->SetScale(objData.scale);
			//番号を設定する。
			m_tank2->SettankNumber(objData.number);
			//falseにすると、レベルの方でモデルが読み込まれない。
			return true;
		}

		if (objData.ForwardMatchName(L"enemy") == true) {
			//エネミーのインスタンスを生成する。
			m_enemy3 = NewGO<Enemy3>(0,"m_enemy3");
			m_enemy3->SetPosition({ objData.position });
			m_enemy3->SetRotation(objData.rotation);
			m_enemy3->SetScale(objData.scale);
			//番号を設定する。
			m_enemy3->SetenemyNumber(objData.number);
			return true;
		}

		if (objData.ForwardMatchName(L"speed") == true) {
			//エネミーのインスタンスを生成する。
			m_speed = NewGO<SpeedEnemy>(0,"m_enemy2");
			m_speed->SetPosition({ objData.position });
			m_speed->SetRotation(objData.rotation);
			m_speed->SetScale(objData.scale);
			//番号を設定する。
			m_speed->SetspeedNumber(objData.number);
			return true;
		}
		return true;
	});

	retryCounter = FindGO<Retry>("retry");

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

//	SkyCube* sky = NewGO<SkyCube>(0);
//	sky->SetLuminance(0.2f);
	
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/6GameBGM.wav");

	//効果音を再生する。
	GameBGM = NewGO<SoundSource>(0);
	GameBGM->Init(6);
	GameBGM->Play(true);
	GameBGM->SetVolume(0.5f);


	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

void Game::Update()
{
	if (m_isWaitFadeout && m_player->Getindex() == 1) {
		if (!m_fade->IsFade()) {
			NewGO<GameClear>(0, "gameclear");
			DeleteGO(this);
		}
	}
	else {
		if (m_player->Getindex() == 1)
		{
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}

	if (retryCounter->retryCounter == 1)
	{
		m_player->m_hp = 0;
		retryCounter->retryCounter = 0;
		siboutimer += 2.0f;
	}
	if (retryCounter->retryCounter == 2)
	{
		m_player->m_hp = 0;
	//	retryCounter->retryCounter = 2;
		siboutimer += 2.0f;
	}
	if (m_player->m_hp <= 0)
	{
/*		sibou = true;
	}
	else {
		sibou = false;
	}
	if (sibou == true)
	{
*/		siboutimer += g_gameTime->GetFrameDeltaTime();
		/*	m_fontRender.SetText(L"Death");
			//表示する座標を設定する。
			m_fontRender.SetPosition(Vector3{ -300.0f,0.0f,0.0f });
			//文字の大きさを変える。
			m_fontRender.SetScale(5.0f);
			if (siboutimer <= 1.5f) {
				alpha = alpha + 0.02f;
				if (alpha > 1.0f) {
					alpha = 1.0f;
				}
			}

			else if (siboutimer > 1.5f) {
				alpha = alpha - 0.02f;
				if (alpha < 0.0f) {
					alpha = 0.0f;
				}
			}
			//m_modelRender.Update();
	*/


		if (m_isWaitFadeout && siboutimer >= 3.0f) {
			if (!m_fade->IsFade()) {
				NewGO<Title>(0, "title");
				DeleteGO(this);
			}
		}
		else {
			if (siboutimer >= 3.0f)
			{
				m_isWaitFadeout = true;
				m_fade->StartFadeOut();
			}
		}
	}
	else {
		return;
	}


}

void Game::Render(RenderContext& rc)
{

	if (m_fps == true)
	{
		m_targetRender.Draw(rc);
	}
	else {
		return;
	}
}
