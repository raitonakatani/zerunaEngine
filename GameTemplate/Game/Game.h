#pragma once

// �O���錾�B
class Player;
class Background;

class Game : public IGameObject
{
public:
	~Game();
	bool Start();
	void Update();
private:
	Player* m_player = nullptr;	// �v���C���[
	Background* m_background = nullptr; // �w�i
};
