#pragma once

//ばねカメラを使用したい場合は、SpringCameraをインクルードする。
//#include "camera/SpringCamera.h"

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

private:
	Vector3 toCameraPosOld;
	Vector3 m_toCameraPos = Vector3::One;	//注視点から視点に向かうベクトル。
//	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。
	Player* m_player = nullptr;
	Vector3 m_Pos;	//注視点から視点に向かうベクトル。
//	Vector3 target;



	

private:
	Vector3 toCameraPosOld2;
	Vector3 m_toCameraPos2;	//注視点から視点に向かうベクトル。
//	Vector3 target2;

};