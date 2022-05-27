#pragma once


#include "LevelRender.h"

// �O���錾�B
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

	float						m_hp = 100.0f;								//�v���C���[��HP�B
	float						life = 100.0f;								//�t��HP�o�[

	SoundSource*				GameBGM;
	SpriteRender				m_spriteRender;
	SpriteRender				m_pressButton;
	float					m_alpha = 0.0f;					//pressbutton�̃��l�B
	SpriteRender				m_targetRender;
	
	GameCamera*					m_gameCamera = nullptr;		// �J����
	TankEnemy*					m_tank = nullptr;			// �G
	TankEnemy2*					m_tank2 = nullptr;			// �G
	SpeedEnemy*					m_speed = nullptr;			//�G
	Background*					m_background = nullptr;		// �w�i
	Floor*						m_floor = nullptr;
	Enemy3*						m_enemy3 = nullptr;			// �G
	index*						m_index = nullptr;			//�ړI��
	Player*						m_player = nullptr;			// �v���C���[
	float						m_sibou = 0;				//���S�֐�
	bool m_deathse = false;
	float						siboutimer = 0.0f;			//
	bool						m_isWaitFadeout = false;	//�t�F�[�h
	Fade*						m_fade = nullptr;			//�t�F�[�h
	Retry* retryCounter = nullptr;
	FontRender					m_fontRender;					//�����B
	float						alpha = 0.0f;
	SoundSource* m_SE;
	LevelRender					m_levelRender;				//���x�������_�[�B 

};
