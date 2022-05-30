#include "stdafx.h"
#include "Menu.h"
#include "PAUSE.h"

bool Menu::Start()
{

	m_skillRender.Init("Assets/sprite/OPTION/2_SKILL/SKILL.dds", 1600.0f, 900.0f);
	m_configRender.Init("Assets/sprite/OPTION/3_CONFIG/CONFIG.dds", 1600.0f, 900.0f);
	m_audioRender.Init("Assets/sprite/OPTION/4_AUDIO/AUDIO_01.dds", 1600.0f, 900.0f);
	m_audioRender2.Init("Assets/sprite/OPTION/4_AUDIO/AUDIO_02.dds", 1600.0f, 900.0f);
	m_seRender.Init("Assets/sprite/OPTION/4_AUDIO/AUDIO.dds", 585.0f, 40.0f);
	m_seRender.SetPivot({ 0.0f,0.5f });
	m_seRender.SetPosition({-530.0f,199.0f,0.0f});
	m_seRender.Update();
	m_BGMRender.Init("Assets/sprite/OPTION/4_AUDIO/AUDIO.dds", 585.0f, 40.0f);
	m_BGMRender.SetPivot({ 0.0f,0.5f });
	m_BGMRender.SetPosition({ -530.0f,131.0f,0.0f });
	m_BGMRender.Update();


	return true;
}

void Menu::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB)) {		//Startƒ{ƒ^ƒ“‚ª‰Ÿ‚³‚ê‚½B
		m_pause = NewGO<PAUSE>(0, "PAUSE");
		DeleteGO(this);
	}
	m_timer += g_gameTime->GetFrameDeltaTime();
}

void Menu::Render(RenderContext& rc)
{
	
	if (m_menu == 0 && m_timer >= 0.001f) {
		m_skillRender.Draw(rc);
		if (g_pad[0]->IsTrigger(enButtonRight))
		{
			m_menu = 1;
			m_timer = 0.0f;
		}
	}
	else if (m_menu == 1 && m_timer >= 0.001f) {
		m_configRender.Draw(rc);
		if (g_pad[0]->IsTrigger(enButtonRight))
		{
			m_menu = 2;
			m_timer = 0.0f;
		}
		if (g_pad[0]->IsTrigger(enButtonLeft))
		{
			m_menu = 0;
			m_timer = 0.0f;
		}
	}
	else if (m_menu == 2 && m_timer >= 0.001f) {

		if (m_audio == 0) {
			m_audioRender.Draw(rc);
			if (g_pad[0]->IsTrigger(enButtonDown))
			{
				m_audio = 1;
			}
			if (g_pad[0]->IsTrigger(enButtonLB1))
			{
				m_se -= 0.05f;
			}
			else if (g_pad[0]->IsTrigger(enButtonRB1))
			{
				m_se += 0.05f;
			}
		}
		else if (m_audio == 1) {
			m_audioRender2.Draw(rc);
			if (g_pad[0]->IsTrigger(enButtonUp))
			{
				m_audio = 0;
			}
			if (g_pad[0]->IsTrigger(enButtonLB1))
			{
				m_bgm -= 0.05f;
			}
			else if (g_pad[0]->IsTrigger(enButtonRB1))
			{
				m_bgm += 0.05f;
			}
		}

		m_seRender.SetScale({ m_se,1.0f,0.0f });
		m_seRender.Update();
		m_seRender.Draw(rc);
		m_BGMRender.SetScale({ m_bgm,1.0f,0.0f });
		m_BGMRender.Update();
		m_BGMRender.Draw(rc);
		if (g_pad[0]->IsTrigger(enButtonLeft))
		{
			m_menu = 1;
			m_timer = 0.0f;
		}
	}
}