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
	Retry* retryCounter = nullptr;
	int m_pause =0;
	SpriteRender m_optionRender;
	SpriteRender m_retireRender;
	SpriteRender m_returnRender;
	Menu* m_menu;
	Game* m_game;
	bool m_option = false;
};

