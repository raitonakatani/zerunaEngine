#pragma once


class Player;

//ゲーム中のカメラを制御する。
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
	
	bool FPS = false;
	Vector3 target;
	int m_camera = 0;
	Vector3 posi;
	float m_timer = 0.0f;
	int rook = 0;
	int drow = 0;


private:
	Vector3					m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル。
	Player*					m_player = nullptr;				//プレイヤー
	Vector3					m_forward;						//前方向のベクトル	
	Quaternion              m_rotation;						//回転
	Vector3					pos2;							//カメラの位置(視点)
	Vector3					toCameraPosOld2;				//注視点から視点に向かうベクトル。
	Vector3					m_toCameraPos2;					//注視点から視点に向かうベクトル。

};