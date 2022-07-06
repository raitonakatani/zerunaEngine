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
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

private:
	Retry*			retryCounter = nullptr;				//リトライカウンター
	int				m_pause = 0;						//ポーズ
	SpriteRender	m_optionRender;						//オプション画面
	SpriteRender	m_retireRender;						//リトライ画面
	SpriteRender	m_returnRender;						//リターン画面
	Menu*			m_menu;								//メニュー
	Game*			m_game;								//ゲーム
	bool			m_option = false;					//オプション判定
};

