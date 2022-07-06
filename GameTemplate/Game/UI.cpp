#include "stdafx.h"
#include "UI.h"
#include "Player.h"

namespace
{
	//Vector3
	const Vector3 HP_SPRITE_POSITION = { -748.0f,420.0f ,0.0f };
	const Vector3 SUTAMINA_SPRITE_POSITION = { -748.0f, 372.0f, 0.0f };
	const Vector3 SP_SPRITE_POSITION = { -748.0f, 325.0f, 0.0f };
	// Vector2
	const Vector2 SPRITE_PIVOT_POSITION = { 0.0f,0.5f };	//�X�v���C�g�����_�[�̃s�{�b�g�B
	// float
	const float SPRITE_WIDTH = 500.0f;						//�X�v���C�g�����_�[�̉��T�C�Y�B
	const float SPRITE_HEIGHT = 27.0f;						//�X�v���C�g�����_�[�̏c�T�C�Y�B

}

bool UI::Start()
{

	// �摜��ǂݍ��ށB
	m_statusRender.Init("Assets/sprite/hutimatome.dds");
	// �\��������W��ݒ肷��B
	m_statusRender.SetPosition(Vector3::Zero);
	// �X�V����B
	m_statusRender.Update();

	// �摜��ǂݍ��ށB
	m_hpRender.Init("Assets/sprite/HP.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
	// �摜�̃s�{�b�g��ݒ肷��B
	m_hpRender.SetPivot(SPRITE_PIVOT_POSITION);
	// �\��������W��ݒ肷��B
	m_hpRender.SetPosition(Vector3::Zero);
	// �X�V����B
	m_hpRender.Update();

	// �摜��ǂݍ��ށB
	m_staminaRender.Init("Assets/sprite/stmn.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
	// �摜�̃s�{�b�g��ݒ肷��B
	m_staminaRender.SetPivot(SPRITE_PIVOT_POSITION);
	// �\��������W��ݒ肷��B
	m_staminaRender.SetPosition(Vector3::Zero);
	// �X�V����B
	m_staminaRender.Update();

	// �摜��ǂݍ��ށB
	m_spRender.Init("Assets/sprite/sp.dds", SPRITE_WIDTH, SPRITE_HEIGHT);
	// �摜�̃s�{�b�g��ݒ肷��B
	m_spRender.SetPivot(SPRITE_PIVOT_POSITION);
	// �\��������W��ݒ肷��B
	m_spRender.SetPosition(Vector3::Zero);
	// �X�V����B
	m_spRender.Update();


	m_Render.Init("Assets/sprite/syutyu3.dds");
	m_Render.SetScale({ m_criticalscale,m_criticalscale,0.0f });
	m_Render.Update();

	m_player = FindGO<Player>("player");

	return true;
}

void UI::Update()
{

	m_hpRender.SetScale({ m_player->GetMaxHP(), 1.0f, 0.0f });
	m_hpRender.SetPosition(HP_SPRITE_POSITION);
	m_hpRender.Update();

	m_staminaRender.SetScale({ m_player->GetMaxStamina(),1.0f,0.0f });
	m_staminaRender.SetPosition(SUTAMINA_SPRITE_POSITION);
	m_staminaRender.Update();

	m_spRender.SetScale({ m_player->GetMaxSP(),1.0f,0.0f });
	m_spRender.SetPosition(SP_SPRITE_POSITION);
	m_spRender.Update();

}

void UI::Render(RenderContext& rc)
{
	if (m_player->GetStartTimer()>=2.5f) {
		
		if (m_alpha < 1.0f && m_player->m_down == false && m_player->GetPlayerHaveIndex() <= 1) {
			m_alpha += 0.01f;
		}
		else if (m_alpha > 0.0f && m_player->m_down == true ||
			m_alpha > 0.0f && m_player->GetPlayerHaveIndex() >= 2) {
			m_alpha -= 0.003f;
		}
		
		if (m_player->IsOpenMenu() == false) {

			m_hpRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_hpRender.Draw(rc);
			//�X�^�~�i
			m_staminaRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_staminaRender.Draw(rc);
			//SP
			m_spRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_spRender.Draw(rc);

			m_statusRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_statusRender.Draw(rc);


			if (m_player->GetHitCritical() == 1) {
				if (m_criticalAlpha <= 1.0f) {
					m_criticalAlpha += 0.005;

					if (m_criticalAlpha >= 0.4f && m_criticalAlpha <= 0.5f) {
						m_criticalscale -= 0.05;
						m_Render.SetScale({ m_criticalscale ,m_criticalscale ,0.0f });
						m_Render.Update();
						m_Render.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_criticalAlpha))));
						m_Render.Draw(rc);
					}
				}
			}
			else {
				if (m_criticalAlpha >= 0.0f) {
					m_criticalAlpha -= 0.005;
					m_Render.SetScale({ m_criticalscale ,m_criticalscale ,0.0f });
					m_Render.Update();
					m_Render.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_criticalAlpha))));
					m_Render.Draw(rc);
				}
				if (m_criticalscale <= 1.995f) {
					m_criticalscale += 0.005;
				}
			}
		}
	}
}
