#pragma once

class Menu;
class Game;
class Retry;

class PAUSE :public IGameObject
{
public:
	PAUSE() {}
	~PAUSE() {}

	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

private:
	Retry*			retryCounter = nullptr;				//���g���C�J�E���^�[
	int				m_pause = 0;						//�|�[�Y
	SpriteRender	m_optionRender;						//�I�v�V�������
	SpriteRender	m_retireRender;						//���g���C���
	SpriteRender	m_returnRender;						//���^�[�����
	Menu*			m_menu;								//���j���[
	Game*			m_game;								//�Q�[��
	bool			m_option = false;					//�I�v�V��������
};

