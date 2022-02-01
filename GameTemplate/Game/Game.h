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

	float posi = 0.0f;
	float posi2 = 0.0f;
	float posi3 = 1.0f;

	Player* m_player = nullptr;	// プレイヤー
	Background* m_background = nullptr; // 背景
};
