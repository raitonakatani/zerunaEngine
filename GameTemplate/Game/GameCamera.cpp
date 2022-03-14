#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}
bool GameCamera::Start()
{

//	m_toCameraPos.Set(1.0f, 1.0f, 1.0f);
	//注視点から視点までのベクトルを設定。
	m_toCameraPos2.Set(0.0f, 100.0f, -150.0f);
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(5000.0f);



	return true;
}
void GameCamera::Update()
{
	///FPS

	//カメラを更新。
	//注視点を計算する。
		target = m_player->GetPosition();
		//プレイヤの足元からちょっと上を注視点とする。
		target.y += 100.0f;
		target += g_camera3D->GetForward() * 20.0f;
	//	target += m_player->m_forward * 20.0f;

//		Vector3 toCameraPosOld = m_player->GetPosition();
		//パッドの入力を使ってカメラを回す。
		float x = g_pad[0]->GetRStickXF();
		float y = g_pad[0]->GetRStickYF();
		//Y軸周りの回転
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
		qRot.Apply(m_toCameraPos);
		//X軸周りの回転。
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, 2.0f * y);
		qRot.Apply(m_toCameraPos);
		//カメラの回転の上限をチェックする。
		//注視点から視点までのベクトルを正規化する。
		//正規化すると、ベクトルの大きさが１になる。
		//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
		Vector3 toPosDir = m_toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.8f) {
			//カメラが上向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.8f) {
			//カメラが下向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}

		//視点を計算する。
		Vector3 pos = target + m_toCameraPos;
	//		pos = m_player->GetPosition();
	//	pos.y += 120.0f;
	//	pos += g_camera3D->GetForward() * 20.0f;
		
	
	// TPS
		
		//カメラを更新。
		//注視点を計算する。
		Vector3 target2 = m_player->GetPosition();
		//プレイヤの足元からちょっと上を注視点とする。
		target2.y += 120.0f;
		toCameraPosOld2 = m_toCameraPos2;

		//パッドの入力を使ってカメラを回す。
		float x2 = g_pad[0]->GetRStickXF();
		float y2 = g_pad[0]->GetRStickYF();
		//Y軸周りの回転
		Quaternion qRot2;
		qRot2.SetRotationDeg(Vector3::AxisY, 2.0f * x2);
		qRot2.Apply(m_toCameraPos2);
		//X軸周りの回転。
		Vector3 axisX2;
		axisX2.Cross(Vector3::AxisY, m_toCameraPos2);
		axisX2.Normalize();
		qRot2.SetRotationDeg(axisX2, 2.0f * y2);
		qRot2.Apply(m_toCameraPos2);
		//カメラの回転の上限をチェックする。
		//注視点から視点までのベクトルを正規化する。
		//正規化すると、ベクトルの大きさが１になる。
		//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
		Vector3 toPosDir2 = m_toCameraPos2;
		toPosDir2.Normalize();
		if (toPosDir2.y < -0.8f) {
			//カメラが上向きすぎ。
			m_toCameraPos2 = toCameraPosOld2;
		}
		else if (toPosDir2.y > 0.8f) {
			//カメラが下向きすぎ。
			m_toCameraPos2 = toCameraPosOld2;
		}

		//視点を計算する。
		Vector3 pos2 = target2 + m_toCameraPos2;
	
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			FPS = true;
		}
		else
		{
			FPS = false;
		}
		if (FPS == true)
		{
			//座標と注視点を設定。
			g_camera3D->SetTarget(target);
			g_camera3D->SetPosition(pos);
		}
		else {
			//メインカメラに注視点と視点を設定する。
			g_camera3D->SetTarget(target2);
			g_camera3D->SetPosition(pos2);
		}
		//カメラの更新。
		g_camera3D->Update();
	
}

void GameCamera::Render(RenderContext& rc)
{
	
}
