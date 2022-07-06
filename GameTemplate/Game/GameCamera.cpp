#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
	//�C���X�^���X���폜����B
	DeleteGO(this);
}
bool GameCamera::Start()
{

	m_toCameraPos.Set(0.0f, 80.0f, 400.0f);
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos2.Set(0.0f, 80.0f, -220.0f);
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(4000.0f);



	return true;
}
void GameCamera::Update()
{

	//���o�J����
	//�J�������X�V�B
	//�����_���v�Z����B
	Vector3 target = m_player->GetPosition();
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target.y += 150.0f;

	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 2.0f * x);
	qRot.Apply(m_toCameraPos);





	//�ʏ�J����
	Vector3 target2 = m_player->GetPosition();
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	target2.y += 150.0f;
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

	m_timer += 0.01f;

	//�N���e�B�J���U�����̃J�����̏����B
	if (m_camera == 1 || m_player->prok == true)
	{
		pos2 = target + m_toCameraPos;
		m_camera = 1;

		if (m_timer >= 1.5f) {
			m_player->prok = false;
			m_camera = 0;
		}
	}
	else {
		m_timer = 0.0f;
		//���_���v�Z����B
		pos2 = target2 + m_toCameraPos2;
	}
	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	g_camera3D->SetPosition(pos2);
	g_camera3D->SetTarget(target2);

	//�J�����̍X�V�B
	g_camera3D->Update();
}

void GameCamera::Render(RenderContext& rc)
{

}
