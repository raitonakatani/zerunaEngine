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

private:
	Player*					m_player = nullptr;				//プレイヤー
	Vector3					m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル。
	Vector3					m_forward;						//前方向のベクトル	
	Vector3					pos2;							//カメラの位置(視点)
	Vector3					toCameraPosOld2;				//注視点から視点に向かうベクトル。
	Vector3					m_toCameraPos2;					//注視点から視点に向かうベクトル。
	Quaternion              m_rotation;						//回転
	int						m_camera = 0;					//カメラの切り替え判定。
	float					m_timer = 0.0f;					//タイマー。

};