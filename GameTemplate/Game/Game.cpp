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

#include "Fade.h"
#include "index.h"
#include "GameClear.h"
#include "Retry.h"
#include "GameStart.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"


#include "box.h"

Game::~Game()
{
	const auto& m_gameCameras = FindGOs<GameCamera>("gameCamera");
	for (auto m_gameCamera : m_gameCameras)
	{
		DeleteGO(m_gameCamera);
	}

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
	const auto& enemy2s = FindGOs<SpeedEnemy>("m_enemy2");
	for (auto enemy2 : enemy2s)
	{
		DeleteGO(enemy2);
	}


	DeleteGO(m_player);
	DeleteGO(m_background);
	DeleteGO(m_floor);
	DeleteGO(GameBGM);
}
bool Game::Start()
{
//	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });

	//NewGO<box>(0);
	
//	m_bgModelRendedr.Init("Assets/modelData/karisute/stage.yuka.tkm");
//	m_bgObject.CreateFromModel(m_bgModelRendedr.GetModel(), m_bgModelRendedr.GetWorldMatrix(0));

	m_player = NewGO<Player>(0, "player");
	m_player->SetPosition({ 0.0f,0.0f,-400.0f });

	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	m_floor = NewGO<Floor>(0, "floor");
	m_floor->SetPosition({0.0f,-1.0f,0.0f});
	m_floor->SetRotation({ 0.0f,0.0f,0.0f ,0.0f});
	m_floor->SetScale({ 1.0f,1.0f,1.0f });

	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/6GameBGM.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/10houkou.wav");
	g_soundEngine->ResistWaveFileBank(16, "Assets/sound/16mituketa.wav");

	//レベルを構築する。
	m_levelRender.Init("Assets/Level/BackGround5.tkl", [&](LevelObjectData& objData) {
		
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

			if (m_tank->GettankNumber() == 4) {
				m_tank->tankPosi = objData.position;
			}
			
			//falseにすると、レベルの方でモデルが読み込まれない。
			return true;
		}

		if (objData.ForwardMatchName(L"secondtank") == true) {
			//エネミーのインスタンスを生成する。
			m_tank2 = NewGO<TankEnemy2>(0, "TankEnemy2");
			m_tank2->SetPosition({  objData.position});
			m_tank2->SetRotation(objData.rotation);
			m_tank2->SetScale(objData.scale);
			//番号を設定する。
			m_tank2->SettankNumber(objData.number);
			//falseにすると、レベルの方でモデルが読み込まれない。
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

		if (objData.EqualObjectName(L"stage") == true) {

			m_background = NewGO<Background>(0, "background");
			m_background->SetPosition(objData.position);
			m_background->SetRotation(objData.rotation);
			m_background->SetScale(objData.scale);
			//trueにすると、レベルの方でモデルが読み込まれて配置される。
			return true;
		}

		return true;
	});

	retryCounter = FindGO<Retry>("retry");

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

//	SkyCube* sky = NewGO<SkyCube>(0);
//	sky->SetLuminance(0.2f);


	m_pressButton.Init("Assets/sprite/RETIRE/Gameover.dds", 1600, 900);
	m_spriteRender.Init("Assets/sprite/fade4.dds", 1600, 900);
	m_spriteRender2.Init("Assets/sprite/kuro.dds", 1600, 900);

	//効果音を再生する。
	GameBGM = NewGO<SoundSource>(0);
	GameBGM->Init(6);


	m_fade = FindGO<Fade>("fade");
	m_title = FindGO<Title>("title");
	m_gameStart = FindGO<GameStart>("gameStart");

//	m_fade->StartFadeIn();

	return true;
}

void Game::Update()
{

	if (m_gameStart->gameStart == 0) {
		return;
	}

	if (m_gameStart->gameStart == 1) {
	/*	GameBGM->Play(true);
		GameBGM->SetVolume(0.5f);*/
		start = 1;
		m_gameStart->gameStart = 2;
	}

	if (m_player->index == 4) {
		m_Ambient += 0.015f;
		m_Direction += 0.015f;
		g_Light.SetAmbientLight({m_Ambient,m_Ambient ,m_Ambient });
		g_Light.SetLigColor({m_Direction,m_Direction ,m_Direction });
	}
	
	/*	if (Bgmspeed == true)
		{
			GameBGM->SetVolume(0.5f);
			GameBGM->SetFrequencyRatio(4);
		}
		else
		{
			GameBGM->SetVolume(0.5f);
			GameBGM->SetFrequencyRatio(1);
		}*/
	

	if (m_isWaitFadeout && m_player->index == 4) {
		if (!m_fade->IsFade()) {
			NewGO<GameClear>(0, "gameclear");
			DeleteGO(this);
		}
	}
	else {
		if (m_Ambient >=2.0f)
		{
			m_fade->m_clear = 1;
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}

	if (retryCounter->retryCounter == 1)
	{
		m_player->m_down = true;
		//retryCounter->retryCounter = 0;
		siboutimer += 2.0f;
	}
	if (retryCounter->retryCounter == 2)
	{
		m_player->m_down = true;
	//	retryCounter->retryCounter = 2;
		siboutimer += 2.0f;
	}
	if (m_player->m_down == true)
	{
		m_deathse = true;
		if (retryCounter->retryCounter == 0) {
			retryCounter->retryCounter = 2;
		}
		if (gameover ==0) {
			gameover = 1;
			SoundSource* SE;
			SE = NewGO<SoundSource>(0);
			SE->Init(10);
			SE->SetVolume(1.5f);
			SE->Play(false);
			SE->SetFrequencyRatio(0.5);
		}

	}
	else {
		m_fade->StartFadeIn();
		m_deathse = false;
	}
	if (m_deathse == true)
	{
		siboutimer = siboutimer + 0.0000000001f;
		
		if (siboutimer >= 100.0f) {
			m_alpha += g_gameTime->GetFrameDeltaTime() * 0.2f;
		}

		if (m_isWaitFadeout && siboutimer >= 150.0f) {
			if (!m_fade->IsFade()) {
				NewGO<Title>(0, "title");
				DeleteGO(this);
			}
		}
		else {
			if (siboutimer >= 150.0f)
			{
				m_isWaitFadeout = true;
				m_fade->StartFadeOut();
			}
		}
	}
	else {
		return;
	}

	m_pressButton.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
}

void Game::Render(RenderContext& rc)
{
	if (m_gameStart->gameStart == 2) {

		m_currentAlpha -= 0.2f * g_gameTime->GetFrameDeltaTime();
		m_spriteRender2.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
		m_spriteRender2.Draw(rc);
	}
	if (m_deathse == true) {
		m_pressButton.Draw(rc);
	}
}