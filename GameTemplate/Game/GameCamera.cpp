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
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(0.0f, 100.0f, -150.0f);
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(10000.0f);

	return true;
}
void GameCamera::Update()
{
/*	if (g_pad[0]->IsPress(enButtonLB1))
	{
		//�J�������X�V�B
		//�����_���v�Z����B
		Vector3 target = m_player->GetPosition();
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
		target.y += 120.0f;
		target.x += 300.0f;
		//�O�����̃x�N�g��
		Vector3 m_forward = g_camera3D->GetForward();;

		target += m_forward * 500.0f;

//		toCameraPosOld = m_player->GetPosition();
//		toCameraPosOld.y += 120.0f;
		m_toCameraPos.Set(m_player->GetPosition());
		m_toCameraPos.y += 100.0f;

	}
	else {
*/		//�J�������X�V�B
		//�����_���v�Z����B
		Vector3 target = m_player->GetPosition();
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
		target.y += 120.0f;
		toCameraPosOld = m_toCameraPos;
//	}
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.8f) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	Vector3 pos = target + m_toCameraPos;
	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//�J�����̍X�V�B
	g_camera3D->Update();
}