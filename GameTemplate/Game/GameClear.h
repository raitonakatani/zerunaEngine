#pragma once

class Fade;

class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

private:
	//�����o�ϐ��B
	bool					m_isWaitFadeout = false;
	Fade*					m_fade = nullptr;           //�t�F�[�h�B
	SpriteRender			m_spriteRender;				//�X�v���C�g�����_�[�B
	float					m_alpha = 0.0f;				//pressbutton�̃��l�B
};

