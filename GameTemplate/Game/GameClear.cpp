#include "stdafx.h"
#include "GameClear.h"
#include "Title.h"
#include "Fade.h"
#include "Game.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{

}

bool GameClear::Start()
{
	//�Q�[���N���A�̉摜��ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/Title/result.dds");

	m_clearRender.Init("Assets/sprite/alert.dds");


	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();

	return true;
}

//�X�V�����B
void GameClear::Update()
{

		if (m_isWaitFadeout) {
			if (!m_fade->IsFade()) {
				NewGO<Title>(0, "title");
				//���g���폜����B
				DeleteGO(this);
			}
		}
		else {
			//A�{�^������������B
			if (g_pad[0]->IsTrigger(enButtonA)) {
				m_isWaitFadeout = true;
				m_fade->StartFadeOut();
			}
		}

		m_currentAlpha -= 0.005f;
}

//�`�揈���B
void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);

	if (m_currentAlpha > 0.0f) {
		m_clearRender.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
		m_clearRender.Draw(rc);
	}

}
