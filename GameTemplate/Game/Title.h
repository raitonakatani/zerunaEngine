#pragma once

//�N���X�錾�B
class Fade;
class Retry;

/// <summary>
/// �^�C�g���B
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
	SpriteRender			m_spriteRender;					//�^�C�g���̉摜�B
	SpriteRender			m_senseRender;					//�^�C�g���̉摜�B
	SpriteRender			m_criticalRender;					//�^�C�g���̉摜�B
	int m_sense = 0;
	float m_senseA = 1.0f;
	float m_senseB = 0.0f;
	float m_critical = 0.0f;
	bool m_A = false;
	bool m_B = false;
	float					m_alpha = 0.0f;					//pressbutton�̃��l�B
	SpriteRender			m_pressButton;					//pressbutton�̉摜�B
	SoundSource* m_titleBGM = nullptr;				//�^�C�g����BGM�B
	Fade* m_fade = nullptr;				//�t�F�[�h�B
	bool					m_isWaitFadeout = false;
};
