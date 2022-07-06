#pragma once


#include "LevelRender.h"

// 前方宣言。
class Player;
class GameCamera;
class TankEnemy;
class SpeedEnemy;
class Background;
class Floor;
class Fade;
class index;
class Retry;
class Title;
class GameStart;
class UI;

class Game : public IGameObject
{

public:
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);


	bool Bgmspeed = false;
	float m_Ambient = 0.15f;
	float m_Direction = 0.35f;
	int start = 0;


	/// <summary>
	/// 敵の数を取得する。
	/// </summary>
	/// <returns>敵の数。</returns>
	const int GettankSize() const
	{
		return m_tankSize;
	}

	void PlustankSize()
	{
		m_tankSize++;
	}

private:
	LevelRender					m_levelRender;					//レベルレンダー。 
	SpriteRender				m_spriteRender;					//フェードの画像。
	SpriteRender				m_gameoverRender;				//ゲームオーバーの画像。
	SoundSource*				GameBGM;						//ゲームBGM
	UI*							m_ui;							//UI
	GameCamera*					m_gameCamera = nullptr;			//カメラ
	TankEnemy*					m_tank = nullptr;				//敵（タンク）
	SpeedEnemy*					m_speed = nullptr;				//敵（スピード）
	Background*					m_background = nullptr;			//背景（ステージ）
	Floor*						m_floor = nullptr;				//背景（床）
	index*						m_index = nullptr;				//目的物
	Player*						m_player = nullptr;				//プレイヤー
	GameStart*					m_gameStart;					//ゲームのスタート判定
	Retry*						retryCounter = nullptr;			//リトライカウンター
	Fade*						m_fade = nullptr;				//フェード
	bool						m_isWaitFadeout = false;		//フェード
	bool						m_deathse = false;				//死亡判定
	float						siboutimer = 0.0f;				//死亡タイマー
	float						m_alpha = 0.0f;					//ゲームオーバー画像のα値。
	float						m_currentAlpha = 1.0f;			//現在のα値。ここを変更すると画像が透明になる。
	int							m_tankSize = 0;					//「タンク」ナンバー
};
