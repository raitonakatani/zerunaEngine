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
	/*
//	m_light.pointlight.ptPosition.x -= g_pad[0]->GetLStickXF();

	g_directionalLight.SetLigPoint({ posi,50.0f,50.0f, });
*/
	if (g_pad[0]->IsPress(enButtonRight)) {
		posi--;
	}
	else if(g_pad[0]->IsPress(enButtonLeft)){
		posi++;
	}

	g_directionalLight.SetLigSpot({ posi,0.0f,50.0f, });

}
