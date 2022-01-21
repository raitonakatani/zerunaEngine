#pragma once

// 前方宣言。
class Player;
class Background;

class Game : public IGameObject
{
public:
	~Game();
	bool Start();
	void Update();
private:

	//DirectionalLight directionalLight;

	Player* m_player = nullptr;	// プレイヤー
	Background* m_background = nullptr; // 背景
};
