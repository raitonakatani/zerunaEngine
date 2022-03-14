#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

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
	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 1600, 900);
	m_spriteRender.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
	m_pressButton.Init("Assets/sprite/button.dds", 800, 400);
	m_pressButton.SetPosition(Vector3(-30.0f, -80.0f, 0.0f));



//	m_fade = FindGO<Fade>("fade");
//	m_fade->StartFadeIn();
	return true;
}

void Title::Update()
{
	if (g_pad[0]->IsTrigger(enButtonA)) {
		NewGO<Game>(0, "game");
		//���g���폜����B
		DeleteGO(this);
	}
/*	if (m_isWaitFadeout) {
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
*/
}

void Title::Render(RenderContext& rc)
{
	//�摜�̕`��B
	m_spriteRender.Draw(rc);
	m_pressButton.Draw(rc);
}