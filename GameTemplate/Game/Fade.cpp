#include "stdafx.h"
#include "Fade.h"
#include "ClearCounter.h"

namespace
{
	const float PLUSALPHA = 5.2f;			//��Z����郿�l
}

Fade::Fade()
{
}


Fade::~Fade()
{
}

bool Fade::Start()
{
	//�摜�̓ǂݍ��݁B
	m_spriteRender.Init("Assets/sprite/fade4.dds");
	m_spriteRender2.Init("Assets/sprite/kuro.dds");

	m_clearRender.Init("Assets/sprite/alert.dds");
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


	//���l��ω�������B
	if (m_state == enState_FadeOut)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * PLUSALPHA;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * PLUSALPHA;
	}

	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
}

void Fade::Render(RenderContext& rc)
{
	//�摜�̐؂�ւ��B
	//���f���̕`��B
	if (m_clear == 0) {
		if (m_currentAlpha > 0.0f) {
			m_spriteRender2.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
			m_spriteRender2.Draw(rc);
		}
	}
	if (m_clear == 1) {
		if (m_currentAlpha > 0.0f) {
			m_clearRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
			m_clearRender.Draw(rc);
		}
	}
}