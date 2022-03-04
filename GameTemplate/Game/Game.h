#pragma once

// �O���錾�B
class Player;
class GameCamera;
class TankEnemy;
class Enemy3;
class Background;
class SpeedEnemy;

class Game : public IGameObject
{
public:
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:

	SpriteRender m_spriteRender;
	SpriteRender m_pressButton;
	Player* m_player = nullptr;				// �v���C���[
	GameCamera* m_gameCamera = nullptr;		// �J����
	TankEnemy* m_tank = nullptr;			// �G
	Enemy3* m_enemy3 = nullptr;				// �G
	Background* m_background = nullptr;		// �w�i
	SpeedEnemy* m_speed = nullptr;
};
