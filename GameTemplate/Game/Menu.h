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

	int					m_menu = 0;				//メニュー画面の判定。
	int					m_audio = 0;			//音声の処理。
	//画像。
	SpriteRender		m_skillRender;			//スキル。
	SpriteRender		m_configRender;			//操作方法。
	SpriteRender		m_audioRender;			//音声。
	SpriteRender		m_audioRender2;			//音声。
	SpriteRender		m_seRender;				//SE。
	SpriteRender		m_BGMRender;			//BGM。
	PAUSE* m_pause;				//ポーズクラス。
	float				m_timer = 0.0f;			//タイマー。
	float				m_se = 1.0f;			//SE。
	float				m_bgm = 1.0f;			//BGM。
};

