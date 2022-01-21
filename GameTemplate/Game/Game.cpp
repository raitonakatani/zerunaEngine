#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Background.h"


Game::~Game()
{
//	DeleteGO(m_player);
//	DeleteGO(m_background);
}
bool Game::Start()
{
	m_player = NewGO<Player>(0);
	m_background = NewGO<Background>(0);

	return true;
}

void Game::Update()
{
}
