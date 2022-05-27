#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
	DeleteGO(this);
}
bool GameCamera::Start()
{

	m_toCameraPos.Set(0.0f, 80.0f, 400.0f);
	//注視点から視点までのベクトルを設定。
	m_toCameraPos2.Set(0.0f, 80.0f, -220.0f);
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(4000.0f);



	return true;
}
void GameCamera::Update()
{

	//演出カメラ
		//カメラを更新。
		//注視点を計算する。
	Vector3 target = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 150.0f;

	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(m_toCameraPos);





	//通常カメラ
	Vector3 target2 = m_player->GetPosition();
	//プレイヤの足元からちょっと上を注視点とする。
	target2.y += 150.0f;
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


	//if (g_pad[0]->IsPress(enButtonA)) {
	//	m_camera = 1;
	//	m_player->prok = true;
	//	m_player->m_prokcamera = 1;
	//}
	//else {
	//	m_camera = 0;
	//	m_player->prok = false;
	//	m_player->m_prokcamera = 0;
	//}

	if (m_camera == 1 || m_player->prok == true)
	{
		pos2 = target + m_toCameraPos;
//		Vector3 position = pos2;
//		g_camera3D->SetPosition(position);
	}
	else {
		//視点を計算する。
		pos2 = target2 + m_toCameraPos2;
//		g_camera3D->SetPosition(pos2);
	}
	//メインカメラに注視点と視点を設定する。

		//視点を計算する。

	g_camera3D->SetPosition(pos2);
	g_camera3D->SetTarget(target2);

	//カメラの更新。
	g_camera3D->Update();


	Vector3 pos = target2 + m_toCameraPos2;
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	Vector3 playerPosition = m_player->GetPosition();
	playerPosition.y += 200.0f;
	Vector3 diff = playerPosition - pos;
	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));

	//プレイヤーが視界内に居なかったら。
	if (Math::PI * 1.0f >= fabsf(angle))
	{
		drow = 1;
	}
	else {
		drow = 0;
	}
}

void GameCamera::Render(RenderContext& rc)
{
	
}
