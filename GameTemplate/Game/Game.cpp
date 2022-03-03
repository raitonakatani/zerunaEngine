#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Background.h"
#include "TankEnemy.h"
#include "Enemy3.h"

Game::~Game()
{
//	DeleteGO(m_player);
//	DeleteGO(m_background);
}
bool Game::Start()
{
	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });

	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

//	m_tank = NewGO<TankEnemy>(0);
//	m_tank->SetPosition({ 100.0f,0.0f,-100.0f });
	m_enemy3 = NewGO<Enemy3>(0);
	m_enemy3->SetPosition({ 0.0f,0.0f,-100.0f });
	m_background = NewGO<Background>(0);


	//画像を読み込む。
	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 800, 450, AlphaBlendMode_None);
	m_pressButton.Init("Assets/sprite/button.dds", 400, 225, AlphaBlendMode_Trans);

	return true;
}

void Game::Update()
{
	//画像の更新。
//	m_spriteRender.Update();
//	m_pressButton.Update();
}

void Game::Render(RenderContext& rc)
{
	//画像の描画。
	//m_spriteRender.Draw(rc);
	//m_pressButton.Draw(rc);
}