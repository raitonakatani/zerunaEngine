#include "stdafx.h"
#include "PAUSE.h"
#include "Menu.h"
#include "Title.h"
#include "Game.h"
#include "Retry.h"

bool PAUSE::Start()
{
	m_optionRender.Init("Assets/sprite/PAUSE/PAUSE_01.dds", 1600.0f, 900.0f);
	m_retireRender.Init("Assets/sprite/PAUSE/PAUSE_02.dds", 1600.0f, 900.0f);
	m_returnRender.Init("Assets/sprite/PAUSE/PAUSE_03.dds", 1600.0f, 900.0f);
	retryCounter = FindGO<Retry>("retry");
	return true;
}

void PAUSE::Update()
{
	if (m_pause == 0 && m_option == false && g_pad[0]->IsTrigger(enButtonA)) {		//Startボタンが押された。
		m_menu = NewGO<Menu>(0, "Menu");
		DeleteGO(this);
	}
	if (m_pause == 1 && m_option == false && g_pad[0]->IsTrigger(enButtonA)) {		//Startボタンが押された。
		retryCounter->retryCounter = 2;
		DeleteGO(this);
	}
	if (m_pause == 2 && m_option == false && g_pad[0]->IsTrigger(enButtonA)) {		//Startボタンが押された。
		retryCounter->retryCounter = 1;
		DeleteGO(this);
	}

	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		DeleteGO(this);
	}
}

void PAUSE::Render(RenderContext& rc)
{
	//	m_menuRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
	if (m_pause == 0) {
		m_optionRender.Draw(rc);

		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			m_pause = 2;
		}
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			m_pause = 1;
		}
	}
	else if (m_pause == 1) {
		m_retireRender.Draw(rc);
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			m_pause = 0;
		}
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			m_pause = 2;
		}
	}
	else if (m_pause == 2) {
		m_returnRender.Draw(rc);
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			m_pause = 1;
		}
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			m_pause = 0;
		}
	}
}