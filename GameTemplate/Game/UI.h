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

	SpriteRender			m_statusRender;					//ステータス画像
	SpriteRender			m_hpRender;						//ヒットポイント画像
	SpriteRender			m_staminaRender;				//スタミナポイント画像
	SpriteRender			m_spRender;						//特殊能力ポイント画像
	SpriteRender			m_Render;

	float					m_alpha =0.0f;					//UIのα値。
	float					m_criticalAlpha = 0.0f;			//集中線の画像のα値。
	float					m_criticalscale = 2.0f;			//集中線の画像。
};

