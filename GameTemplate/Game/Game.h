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

private:

	float						m_hp = 100.0f;								//プレイヤーのHP。
	float						life = 100.0f;								//フルHPバー

	SoundSource*				GameBGM;
	SpriteRender				m_spriteRender;
	SpriteRender				m_spriteRender2;				//スプライトレンダ―。
	SpriteRender				m_spriteRender3;				//スプライトレンダ―。
	SpriteRender				m_pressButton;
	float						m_alpha = 0.0f;					//pressbuttonのα値。
	SpriteRender				m_targetRender;
	
	Title* m_title;
	GameCamera*					m_gameCamera = nullptr;		// カメラ
	TankEnemy*					m_tank = nullptr;			// 敵
	SpeedEnemy*					m_speed = nullptr;			//敵
	Background*					m_background = nullptr;		// 背景
	Floor*						m_floor = nullptr;
	index*						m_index = nullptr;			//目的物
	Player*						m_player = nullptr;			// プレイヤー
	GameStart*					m_gameStart;
	float						m_sibou = 0;				//死亡関数
	bool						m_deathse = false;
	float						siboutimer = 0.0f;			//
	bool						m_isWaitFadeout = false;	//フェード
	Fade*						m_fade = nullptr;			//フェード
	Retry*						retryCounter = nullptr;
	FontRender					m_fontRender;					//文字。
	float						alpha = 0.0f;
	SoundSource*				m_SE;
	LevelRender					m_levelRender;				//レベルレンダー。 
	float				m_currentAlpha = 1.0f;		//現在のα値。ここを変更すると画像が透明になる。
	int gameover = 0;
};
