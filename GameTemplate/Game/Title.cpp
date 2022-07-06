#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "Retry.h"
#include "GameStart.h"

Title::Title()
{

}

Title::~Title()
{
	//BGMを削除する。
	DeleteGO(m_titleBGM);
}

bool Title::Start()
{
	//画像を読み込む。
	m_spriteRender.Init("Assets/sprite/Title/title.dds", 1600, 900);
	m_senseRender.Init("Assets/sprite/Title/sense.dds", 1600, 900);
	m_criticalRender.Init("Assets/sprite/Title/critical.dds", 1600, 900);
	m_pressButton.Init("Assets/sprite/Title/button2.dds", 800, 400);
	m_pressButton.SetPosition(Vector3(0.0f, -200.0f, 0.0f));
	m_pressButton.Update();

	//音を読み込む。
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/2titleBGM.wav");
	//BGMを再生する。
	m_titleBGM = NewGO<SoundSource>(0);
	m_titleBGM->Init(2);
	m_titleBGM->Play(true);
	m_titleBGM->SetVolume(0.3f);

	retryCounter = FindGO<Retry>("retry");
	m_gameStart = FindGO<GameStart>("gameStart");

	if (retryCounter->retryCounter == 0)
	{
		m_fade = FindGO<Fade>("fade");
		m_fade->StartFadeIn();
		m_fade->m_clear = 0;
	}

	NewGO<Game>(0, "game");

	return true;
}

void Title::Update()
{
	if (a == false) {
		m_timer += 0.04f;
	}

	if (retryCounter->retryCounter == 1)
	{
		retryCounter->retryCounter = 0;
		m_gameStart->gameStart = 0;
		m_sense = 0;
	}

	if (retryCounter->retryCounter == 2)
	{
		m_timer += 8.0f;
		m_gameStart->gameStart = 0;
		m_sense = 2;
	}
	if (m_sense != 2) {
		if (g_pad[0]->IsTrigger(enButtonStart)) {
			m_timer = 0.0f;
			m_sense = 2;
			m_isWaitFadeout = true;
			//音を読み込む。
			g_soundEngine->ResistWaveFileBank(0, "Assets/sound/0titlebutton.wav");
			//効果音を再生する。
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(0);
			se->Play(false);
			se->SetVolume(0.5f);
		}
	}
	if (m_sense == 0) {
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_A = true;
			m_sense = 1;
			m_timer = 0.0f;
		}
	}

	if (m_sense == 1 && m_senseA <= 0.0f) {
		if (m_timer >= 10.0f) {
			m_timer = 0.0f;
			m_A = false;
			m_senseA = 0.0f;
			m_B = true;
			m_sense = 2;
		}

	}

	if (m_sense == 2) {
		if (m_isWaitFadeout && m_timer >= 1.0f) {
			retryCounter->retryCounter = 0;
			m_gameStart->gameStart = 1;
			//自身を削除する。
			DeleteGO(this);
		}
		else {
			if (m_timer >= 10.0f && m_senseB<0.1f) {
				m_isWaitFadeout = true;
				if (retryCounter->retryCounter == 0) {
					m_timer = 0.0f;
					//音を読み込む。
					g_soundEngine->ResistWaveFileBank(0, "Assets/sound/0titlebutton.wav");
					//効果音を再生する。
					SoundSource* se = NewGO<SoundSource>(0);
			//		se->Init(0);
			//		se->Play(false);
			//		se->SetVolume(0.5f);
				}
			}
		}

	}

	if (m_A == true && m_senseA >= 0.0f) {
		m_senseA -= 0.01f;
		m_senseB += 0.01f;
	}
	if (m_B == true && m_senseB >= 0.0f) {
		m_senseB -= 0.01f;
		m_critical += 0.01f;
	}

	if (retryCounter->retryCounter == 0) {
		//α値を変化させる。
		if (m_isWaitFadeout)
		{
			m_alpha += g_gameTime->GetFrameDeltaTime() * 20.5f;
		}
		else
		{
			m_alpha += g_gameTime->GetFrameDeltaTime() * 1.2f;
		}
	}

	m_criticalRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_critical))));
	m_senseRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_senseB))));
	m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_senseA))));
	m_pressButton.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

}

void Title::Render(RenderContext& rc)
{
	if (retryCounter->retryCounter == 0) {
		//画像の描画。
		m_criticalRender.Draw(rc);
		m_senseRender.Draw(rc);
		m_spriteRender.Draw(rc);
		if (m_sense == 0) {
			m_pressButton.Draw(rc);
		}
	}

	if (retryCounter->retryCounter == 2) {

		m_spriteRender.Init("Assets/sprite/fade4.dds", 1600, 900);
		m_alpha += g_gameTime->GetFrameDeltaTime() * 5.2f;
		m_spriteRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
		m_spriteRender.Draw(rc);

	}
}