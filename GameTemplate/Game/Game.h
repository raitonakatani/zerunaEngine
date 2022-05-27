#pragma once


#include "LevelRender.h"

// 前方宣言。
class Player;
class GameCamera;
class TankEnemy;
class TankEnemy2;
class SpeedEnemy;
class Enemy3;
class Background;
class Floor;
class Fade;
class index;
class Retry;

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

private:

	float						m_hp = 100.0f;								//プレイヤーのHP。
	float						life = 100.0f;								//フルHPバー

	SoundSource*				GameBGM;
	SpriteRender				m_spriteRender;
	SpriteRender				m_pressButton;
	float					m_alpha = 0.0f;					//pressbuttonのα値。
	SpriteRender				m_targetRender;
	
	GameCamera*					m_gameCamera = nullptr;		// カメラ
	TankEnemy*					m_tank = nullptr;			// 敵
	TankEnemy2*					m_tank2 = nullptr;			// 敵
	SpeedEnemy*					m_speed = nullptr;			//敵
	Background*					m_background = nullptr;		// 背景
	Floor*						m_floor = nullptr;
	Enemy3*						m_enemy3 = nullptr;			// 敵
	index*						m_index = nullptr;			//目的物
	Player*						m_player = nullptr;			// プレイヤー
	float						m_sibou = 0;				//死亡関数
	bool m_deathse = false;
	float						siboutimer = 0.0f;			//
	bool						m_isWaitFadeout = false;	//フェード
	Fade*						m_fade = nullptr;			//フェード
	Retry* retryCounter = nullptr;
	FontRender					m_fontRender;					//文字。
	float						alpha = 0.0f;
	SoundSource* m_SE;
	LevelRender					m_levelRender;				//レベルレンダー。 

};
