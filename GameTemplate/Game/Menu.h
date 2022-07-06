#pragma once

class PAUSE;

class Menu :public IGameObject
{
public:
	Menu() {}
	~Menu() {}

	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

private:

	int					m_menu = 0;				//���j���[��ʂ̔���B
	int					m_audio = 0;			//�����̏����B
	//�摜�B
	SpriteRender		m_skillRender;			//�X�L���B
	SpriteRender		m_configRender;			//������@�B
	SpriteRender		m_audioRender;			//�����B
	SpriteRender		m_audioRender2;			//�����B
	SpriteRender		m_seRender;				//SE�B
	SpriteRender		m_BGMRender;			//BGM�B
	PAUSE* m_pause;				//�|�[�Y�N���X�B
	float				m_timer = 0.0f;			//�^�C�}�[�B
	float				m_se = 1.0f;			//SE�B
	float				m_bgm = 1.0f;			//BGM�B
};

