#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

// �O���錾�B
class Player;
class GameCamera;
class TankEnemy;
class SpeedEnemy;
class Enemy3;
class Background;

class Game : public IGameObject
{
	/*
public:
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	*/
private:

	SpriteRender m_spriteRender;
	SpriteRender m_pressButton;
	
	GameCamera* m_gameCamera = nullptr;		// �J����
	TankEnemy* m_tank = nullptr;			// �G
	SpeedEnemy* m_speed = nullptr;
	Background* m_background = nullptr;		// �w�i
	Enemy3* m_enemy3 = nullptr;				// �G
	Player* m_player = nullptr;				// �v���C���[
public:
	//////////////////////////////////////
	// �����o�֐��B
	//////////////////////////////////////
	//Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	CharacterController m_charaCon;
	ModelRender m_bgModelRendedr;
	ModelRender m_charaRender;
	ModelRender m_targetPointRender;
	TknFile m_tknFile;
	PhysicsStaticObject m_bgObject;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
	Vector3 m_position;
	Vector3 m_targetPointPosition;
};
