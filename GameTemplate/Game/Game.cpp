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


	/// <summary>
	/// (*•É÷•)/ ∞≤
	/// </summary>
	/// <returns>(*•É÷•)/ ∞≤</returns>
	Vector3 iro = { 0.5f,0.5f,0.5f };


	return true;
}

void Game::Update()
{

	if (g_pad[0]->IsPress(enButtonX))
	{
		posi--;
	}
	if (g_pad[0]->IsPress(enButtonY))
	{
		posi++;
	}

	

	g_Light.SetLigPointColor({ posi, 10.0f, 3.0f });



}
