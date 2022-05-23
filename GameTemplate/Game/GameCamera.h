#pragma once

//ばねカメラを使用したい場合は、SpringCameraをインクルードする。
//#include "camera/SpringCamera.h"

class Player;
class TankEnemy;

//ゲーム中のカメラを制御する。
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
/// 座標を設定。
/// </summary>
/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_Pos = position;
	}
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
	Vector3 toCameraPosOld;
	Vector3 m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル。
//	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。
	Player* m_player = nullptr;
	Vector3 m_Pos;	//注視点から視点に向かうベクトル。
//	Vector3 target;
	TankEnemy* m_tank;
	Vector3 m_position;
	Vector3 m_forward;
	Quaternion              m_rotation;                                 //回転
	Vector3 pos2;

	

private:
	Vector3 toCameraPosOld2;
	Vector3 m_toCameraPos2;	//注視点から視点に向かうベクトル。
//	Vector3 target2;

};