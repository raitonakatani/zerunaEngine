#include "stdafx.h"
#include "Player.h"

void Player::InitAnimation()
{
	//アニメーションクリップをロードする。
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
}

bool Player::Start()
{

	//アニメーションを初期化。
	InitAnimation();

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClipArray, enAnimClip_Num, enModelUpAxisY);

	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);

	//m_rotation.SetRotationDegY(180.0f);

	return true;
}


void Player::Update()
{
	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * -120.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * -120.0f;

	// 右スティック(キーボード：上下左右)で回転。
//	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);

	// アニメーションの切り替え。
	if (g_pad[0]->IsPress(enButtonA)) {
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.2f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
	}

	m_position = m_charaCon.Execute(moveSpeed, g_gameTime->GetFrameDeltaTime());

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);

	m_modelRender.Update();
}

void Player::Render(RenderContext& rc)
{
	//ドロー。
	m_modelRender.Draw(rc);
}
