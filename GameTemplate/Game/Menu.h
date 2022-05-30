#pragma once

class PAUSE;

class Menu :public IGameObject
{
public:
	Menu() {}
	~Menu() {}

	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

private:
	int m_menu = 0;
	int m_audio = 0;
	SpriteRender m_skillRender;
	SpriteRender m_configRender;
	SpriteRender m_audioRender;
	SpriteRender m_audioRender2;
	SpriteRender m_seRender;
	SpriteRender m_BGMRender;
	PAUSE* m_pause;
	float m_timer = 0.0f;
	float m_se = 1.0f;
	float m_bgm = 1.0f;
};

