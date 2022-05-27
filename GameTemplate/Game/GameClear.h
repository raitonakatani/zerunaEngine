#pragma once

class Fade;

class GameClear : public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

private:
	//メンバ変数。
	bool					m_isWaitFadeout = false;
	Fade*					m_fade = nullptr;           //フェード。
	SpriteRender			m_spriteRender;				//スプライトレンダー。
	float					m_alpha = 0.0f;				//pressbuttonのα値。
	SpriteRender		m_clearRender;				//スプライトレンダ―。
	float				m_currentAlpha = 1.0f;		//現在のα値。ここを変更すると画像が透明になる。

};

