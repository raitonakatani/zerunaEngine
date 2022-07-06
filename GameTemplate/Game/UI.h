#pragma once

class Player;

class UI : public IGameObject
{
public:
	UI() {}
	~UI() {}

	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	Player* m_player;

	SpriteRender			m_statusRender;					//�X�e�[�^�X�摜
	SpriteRender			m_hpRender;						//�q�b�g�|�C���g�摜
	SpriteRender			m_staminaRender;				//�X�^�~�i�|�C���g�摜
	SpriteRender			m_spRender;						//����\�̓|�C���g�摜
	SpriteRender			m_Render;

	float					m_alpha =0.0f;					//UI�̃��l�B
	float					m_criticalAlpha = 0.0f;			//�W�����̉摜�̃��l�B
	float					m_criticalscale = 2.0f;			//�W�����̉摜�B
};

