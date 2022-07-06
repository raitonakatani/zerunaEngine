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
	bool					m_isWaitFadeout = false;	//フェード。
	Fade*					m_fade = nullptr;           //フェード。
	SpriteRender			m_spriteRender;				//スプライトレンダー。
	SpriteRender			m_clearRender;				//スプライトレンダ―。
	float					m_currentAlpha = 1.0f;		//現在のα値。ここを変更すると画像が透明になる。

};

