#include "stdafx.h"
#include "Player.h"

void Player::InitAnimation()
{
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
}

bool Player::Start()
{

	//�A�j���[�V�������������B
	InitAnimation();

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClipArray, enAnimClip_Num, enModelUpAxisY);

	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);

	m_rotation.SetRotationDegY(180.0f);

	return true;
}


void Player::Update()
{
	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * 120.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * 120.0f;

	// �E�X�e�B�b�N(�L�[�{�[�h�F�㉺���E)�ŉ�]�B
	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
//	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);

	// �㉺���E�L�[(�L�[�{�[�h�F2, 4, 6, 8)�Ŋg��
	if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}

	// �A�j���[�V�����̐؂�ւ��B
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
	//�h���[�B
	m_modelRender.Draw(rc);
}
