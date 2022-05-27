#pragma once

//クラス宣言。
class Fade;
class Retry;

/// <summary>
/// タイトル。
/// </summary>
class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	Retry* retryCounter = nullptr;
	SpriteRender			m_spriteRender;					//タイトルの画像。
	SpriteRender			m_senseRender;					//タイトルの画像。
	SpriteRender			m_criticalRender;					//タイトルの画像。
	int m_sense = 0;
	float m_senseA = 1.0f;
	float m_senseB = 0.0f;
	float m_critical = 0.0f;
	bool m_A = false;
	bool m_B = false;
	float					m_alpha = 0.0f;					//pressbuttonのα値。
	SpriteRender			m_pressButton;					//pressbuttonの画像。
	SoundSource* m_titleBGM = nullptr;				//タイトルのBGM。
	Fade* m_fade = nullptr;				//フェード。
	bool					m_isWaitFadeout = false;
};
