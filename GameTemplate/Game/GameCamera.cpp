#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "TankEnemy.h"


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
	m_toCameraPos2.Set(0.0f, 80.0f, -220.0f);
	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");
	m_tank = FindGO<TankEnemy>("TankEmemy");

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(4000.0f);



	return true;
}
void GameCamera::Update()
{
	// TPS
		
		//�J�������X�V�B
		//�����_���v�Z����B
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


/*		if (g_pad[0]->IsPress(enButtonA)) {
			m_camera = 1;
		}
		else {
			m_camera = 0;
		}

		if (m_camera==1)
		{
			m_timer += g_gameTime->GetFrameDeltaTime();

			Vector3 kari;
			m_toCameraPos2 = m_toCameraPos2;
			if (m_timer < 1.0f)
			{
				m_toCameraPos2 += g_camera3D->GetForward() * 3.0f;
				rook = 1;
			}
			//���_���v�Z����B
			Vector3 pos2 = target2 + m_toCameraPos2;

			posi = pos2;
//			posi = posi * 0.8f;
			g_camera3D->SetPosition(posi);
		}
		else {
			m_timer = 0.0f;
			m_toCameraPos2 = m_toCameraPos2;
			if (rook == 1) {
				m_timer += g_gameTime->GetFrameDeltaTime();
				if (m_timer < 1.0f)
				{
					m_toCameraPos2 += g_camera3D->GetForward() * 3.0f;
					rook = 0;
				}
			}
			//���_���v�Z����B
			Vector3 pos2 = target2 + m_toCameraPos2;

			g_camera3D->SetPosition(pos2);
		}
*/		//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B

			//���_���v�Z����B
		Vector3 pos2 = target2 + m_toCameraPos2;

		g_camera3D->SetPosition(pos2);
		g_camera3D->SetTarget(target2);

		//�J�����̍X�V�B
		g_camera3D->Update();
	

		m_forward = Vector3::AxisZ;
		m_rotation.Apply(m_forward);
		Vector3 playerPosition = m_player->GetPosition();
		playerPosition.y += 200.0f;
		Vector3 diff = playerPosition - pos2;
		diff.Normalize();
		float angle = acosf(diff.Dot(m_forward));

		//�v���C���[�����E���ɋ��Ȃ�������B
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
