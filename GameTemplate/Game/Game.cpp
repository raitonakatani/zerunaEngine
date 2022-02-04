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
		posi -= 0.01f;
	}
	if (g_pad[0]->IsPress(enButtonY))
	{
		posi += 0.01f;
	}

	if (posi >= 1.0f)
	{
		posi = 1.0f;
	}
	if (posi <= -1.0f)
	{
		posi = -1.0f;
	}

//	g_Light.SetLigDirection({ posi, -1.0f, 1.0f });



}
