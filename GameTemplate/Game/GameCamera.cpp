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
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos2.Set(0.0f, 100.0f, -150.0f);
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(5000.0f);



	return true;
}
void GameCamera::Update()
{
	///FPS

	//�J�������X�V�B
	//�����_���v�Z����B
		target = m_player->GetPosition();
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
		target.y += 100.0f;
		target += g_camera3D->GetForward() * 20.0f;
	//	target += m_player->m_forward * 20.0f;

//		Vector3 toCameraPosOld = m_player->GetPosition();
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
	//		pos = m_player->GetPosition();
	//	pos.y += 120.0f;
	//	pos += g_camera3D->GetForward() * 20.0f;
		
	
	// TPS
		
		//�J�������X�V�B
		//�����_���v�Z����B
		Vector3 target2 = m_player->GetPosition();
		//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
		target2.y += 120.0f;
		toCameraPosOld2 = m_toCameraPos2;

		//�p�b�h�̓��͂��g���ăJ�������񂷁B
		float x2 = g_pad[0]->GetRStickXF();
		float y2 = g_pad[0]->GetRStickYF();
		//Y������̉�]
		Quaternion qRot2;
		qRot2.SetRotationDeg(Vector3::AxisY, 2.0f * x2);
		qRot2.Apply(m_toCameraPos2);
		//X������̉�]�B
		Vector3 axisX2;
		axisX2.Cross(Vector3::AxisY, m_toCameraPos2);
		axisX2.Normalize();
		qRot2.SetRotationDeg(axisX2, 2.0f * y2);
		qRot2.Apply(m_toCameraPos2);
		//�J�����̉�]�̏�����`�F�b�N����B
		//�����_���王�_�܂ł̃x�N�g���𐳋K������B
		//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
		//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
		Vector3 toPosDir2 = m_toCameraPos2;
		toPosDir2.Normalize();
		if (toPosDir2.y < -0.8f) {
			//�J����������������B
			m_toCameraPos2 = toCameraPosOld2;
		}
		else if (toPosDir2.y > 0.8f) {
			//�J�����������������B
			m_toCameraPos2 = toCameraPosOld2;
		}

		//���_���v�Z����B
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
			//���W�ƒ����_��ݒ�B
			g_camera3D->SetTarget(target);
			g_camera3D->SetPosition(pos);
		}
		else {
			//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
			g_camera3D->SetTarget(target2);
			g_camera3D->SetPosition(pos2);
		}
		//�J�����̍X�V�B
		g_camera3D->Update();
	
}

void GameCamera::Render(RenderContext& rc)
{
	
}
