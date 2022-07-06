#pragma once


#include "LevelRender.h"

// �O���錾�B
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
	/// �G�̐����擾����B
	/// </summary>
	/// <returns>�G�̐��B</returns>
	const int GettankSize() const
	{
		return m_tankSize;
	}

	void PlustankSize()
	{
		m_tankSize++;
	}

private:
	LevelRender					m_levelRender;					//���x�������_�[�B 
	SpriteRender				m_spriteRender;					//�t�F�[�h�̉摜�B
	SpriteRender				m_gameoverRender;				//�Q�[���I�[�o�[�̉摜�B
	SoundSource*				GameBGM;						//�Q�[��BGM
	UI*							m_ui;							//UI
	GameCamera*					m_gameCamera = nullptr;			//�J����
	TankEnemy*					m_tank = nullptr;				//�G�i�^���N�j
	SpeedEnemy*					m_speed = nullptr;				//�G�i�X�s�[�h�j
	Background*					m_background = nullptr;			//�w�i�i�X�e�[�W�j
	Floor*						m_floor = nullptr;				//�w�i�i���j
	index*						m_index = nullptr;				//�ړI��
	Player*						m_player = nullptr;				//�v���C���[
	GameStart*					m_gameStart;					//�Q�[���̃X�^�[�g����
	Retry*						retryCounter = nullptr;			//���g���C�J�E���^�[
	Fade*						m_fade = nullptr;				//�t�F�[�h
	bool						m_isWaitFadeout = false;		//�t�F�[�h
	bool						m_deathse = false;				//���S����
	float						siboutimer = 0.0f;				//���S�^�C�}�[
	float						m_alpha = 0.0f;					//�Q�[���I�[�o�[�摜�̃��l�B
	float						m_currentAlpha = 1.0f;			//���݂̃��l�B������ύX����Ɖ摜�������ɂȂ�B
	int							m_tankSize = 0;					//�u�^���N�v�i���o�[
};
