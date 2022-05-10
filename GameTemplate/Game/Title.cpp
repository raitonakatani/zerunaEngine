#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "Retry.h"

Title::Title()
{

}

Title::~Title()
{
	//BGM���폜����B
	DeleteGO(m_titleBGM);
}

bool Title::Start()
{
	//�摜��ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/Title/title.dds", 1600, 900);
	m_pressButton.Init("Assets/sprite/Title/button.dds", 800, 400);
	m_pressButton.SetPosition(Vector3(0.0f, -150.0f, 0.0f));
	m_pressButton.Update();

	//����ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/2titleBGM.wav");
	//BGM���Đ�����B
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(2);
	m_titleBGM->Play(true);
	m_titleBGM->SetVolume(0.3f);

	retryCounter = FindGO<Retry>("retry");
	if (retryCounter->retryCounter == 0)
	{
		m_fade = FindGO<Fade>("fade");
		m_fade->StartFadeIn();
	}
	return true;
}

void Title::Update()
{
	if (retryCounter->retryCounter == 2)
	{
		retryCounter->retryCounter = 0;
		NewGO<Game>(0, "game");
		//���g���폜����B
		DeleteGO(this);
	}

	if (m_isWaitFadeout) {
		if (!m_fade->IsFade()) {
			NewGO<Game>(0, "game");
			//���g���폜����B
			DeleteGO(this);
		}
	}
	else {
		//A�{�^������������B
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();

			//����ǂݍ��ށB
			g_soundEngine->ResistWaveFileBank(0, "Assets/sound/0titlebutton.wav");
			//���ʉ����Đ�����B
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(0);
			se->Play(false);
			se->SetVolume(0.5f);

		}
	}

	//���l��ω�������B
	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
	}
	else
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
	}

	m_pressButton.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

}

void Title::Render(RenderContext& rc)
{
	//�摜�̕`��B
	m_spriteRender.Draw(rc);
	m_pressButton.Draw(rc);
}