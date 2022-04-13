#pragma once


#include "LevelRender.h"

// �O���錾�B
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

	float						m_hp = 100.0f;								//�v���C���[��HP�B
	float						life = 100.0f;								//�t��HP�o�[

	SoundSource*				GameBGM;
	SpriteRender				m_spriteRender;
	SpriteRender				m_pressButton;
	SpriteRender				m_targetRender;
	
	GameCamera*					m_gameCamera = nullptr;		// �J����
	TankEnemy*					m_tank = nullptr;			// �G
	SpeedEnemy*					m_speed = nullptr;			//�G
	Background*					m_background = nullptr;		// �w�i
	Enemy3*						m_enemy3 = nullptr;			// �G
	index*						m_index = nullptr;			//�ړI��
	Player*						m_player = nullptr;			// �v���C���[
	bool						m_fps = false;				//���_	bool						sibou = false;				//���S�֐�
	float						siboutimer = 0.0f;			//
	bool						m_isWaitFadeout = false;	//�t�F�[�h
	Fade*						m_fade = nullptr;			//�t�F�[�h

	LevelRender					m_levelRender;				//���x�������_�[�B
};
