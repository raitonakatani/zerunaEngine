#pragma once


#include "LevelRender.h"

// 前方宣言。
class Player;
class GameCamera;
class TankEnemy;
class SpeedEnemy;
class Enemy3;
class Background;
class Fade;
class index;

class Game : public IGameObject
{

public:
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);


private:

	float						m_hp = 100.0f;								//プレイヤーのHP。
	float						life = 100.0f;								//フルHPバー

	SoundSource*				GameBGM;
	SpriteRender				m_spriteRender;
	SpriteRender				m_pressButton;
	SpriteRender				m_targetRender;
	
	GameCamera*					m_gameCamera = nullptr;		// カメラ
	TankEnemy*					m_tank = nullptr;			// 敵
	SpeedEnemy*					m_speed = nullptr;			//敵
	Background*					m_background = nullptr;		// 背景
	Enemy3*						m_enemy3 = nullptr;			// 敵
	index*						m_index = nullptr;			//目的物
	Player*						m_player = nullptr;			// プレイヤー
	bool						m_fps = false;				//視点	bool						sibou = false;				//死亡関数
	float						siboutimer = 0.0f;			//
	bool						m_isWaitFadeout = false;	//フェード
	Fade*						m_fade = nullptr;			//フェード

	LevelRender					m_levelRender;				//レベルレンダー。
};
