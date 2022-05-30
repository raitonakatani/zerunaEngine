#include "stdafx.h"
#include "Fade.h"
#include "ClearCounter.h"

namespace
{
	const Vector3	SCALE = Vector3(1.9f, 2.4f, 1.0f);
	const Vector3	POSITIOIN = Vector3(-10.0f, 80.0, 0.0f);
}

Fade::Fade()
{
}


Fade::~Fade()
{
}

bool Fade::Start()
{
	m_spriteRender.Init("Assets/sprite/fade4.dds", 1600, 900);
	//m_spriteRender.SetScale(SCALE);
	m_spriteRender.SetPosition(POSITIOIN);
	m_spriteRender2.Init("Assets/sprite/kuro.dds", 1600, 900);
	//m_spriteRender.SetScale(SCALE);
	m_spriteRender2.SetPosition(POSITIOIN);
	//m_spriteRender.Update();

	m_clearRender.Init("Assets/sprite/alert.dds", 1600, 900);
	//clearCounter = FindGO<ClearCounter>("clear");
	return true;
}

void Fade::Update()
{
	switch (m_state) {
	case enState_FadeIn:
		m_currentAlpha -= 0.5f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOut:
		m_currentAlpha += 0.5f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_Idle:

		break;
	}


	//ƒ¿’l‚ð•Ï‰»‚³‚¹‚éB
	if (m_state== enState_FadeOut)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 5.5f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 5.2f;
	}

	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
}

void Fade::Render(RenderContext& rc)
{
	if (m_clear == 0) {
		if (m_currentAlpha > 0.0f) {
			//	m_spriteRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
			m_spriteRender2.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
			m_spriteRender2.Draw(rc);
	//		m_spriteRender.Draw(rc);
		}
	}
	if (m_clear == 1) {
		if (m_currentAlpha > 0.0f) {
			m_clearRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
			m_clearRender.Draw(rc);
		}
	}
}