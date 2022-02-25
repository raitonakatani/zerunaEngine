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
	m_player->SetPosition({ 0.0f,0.0f,0.0f });
	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");

	m_tank = NewGO<TankEnemy>(0);
	m_tank->SetPosition({ 100.0f,0.0f,-100.0f });
	m_enemy3 = NewGO<Enemy3>(0);
	m_enemy3->SetPosition({ 0.0f,0.0f,-100.0f });
	m_background = NewGO<Background>(0);

	return true;
}

void Game::Update()
{

}
