#include "stdafx.h"
#include "ENE.h"

bool ENE::Start()
{
	m_bgObject.CreateFromModel(m_bgModelRendedr.GetModel(), m_bgModelRendedr.GetWorldMatrix(0));

	m_charaRender.Init("Assets/modelData/unityChan.tkm");
	m_charaRender.SetScale(1.2f, 1.2f, 1.2f);
	m_targetPointRender.Init("Assets/modelData/light.tkm");
	m_targetPointRender.SetScale(4.0f, 4.0f, 4.0f);
//	m_targetPointRender.SetShadowCasterFlag(false);
	m_charaCon.Init(50.0f, 50.0f, m_targetPointPosition);
	m_targetPointPointLight.Init();

	// �i�r���b�V�����\�z�B
	m_nvmMesh.Init("Assets/nvm/test.tkn");

	return true;
}

void ENE::Update()
{

	bool isEnd;
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// �p�X����
		m_pathFiding.Execute(
			m_path,							// �\�z���ꂽ�p�X�̊i�[��
			m_nvmMesh,						// �i�r���b�V��
			m_position,						// �J�n���W
			m_targetPointPosition,			// �ړ��ڕW���W
			PhysicsWorld::GetInstance(),	// �����G���W��	
			50.0f,							// AI�G�[�W�F���g�̔��a
			200.0f							// AI�G�[�W�F���g�̍����B
		);
	}
	// �p�X����ړ�����B
	m_position = m_path.Move(
		m_position,
		10.0f,
		isEnd
	);

	m_charaRender.SetPosition(m_position);
	m_charaRender.Update();

	Vector3 move;
	move.x = -g_pad[0]->GetLStickXF() * 10.0f;
	move.y = -10.0f;
	move.z = -g_pad[0]->GetLStickYF() * 10.0f;
	m_targetPointPosition = m_charaCon.Execute(move, 1.0f);
	m_targetPointRender.SetPosition(
		{ m_targetPointPosition.x, m_targetPointPosition.y + 100.0f, m_targetPointPosition.z }
	);
	m_targetPointPointLight.SetLigPoint(
		{ m_targetPointPosition.x, m_targetPointPosition.y + 100.0f, m_targetPointPosition.z }
	);
	m_targetPointPointLight.SetLigPointColor({ 100.5f, 100.5f, 100.5f });
	m_targetPointPointLight.SetPointRange(130.0f);
	m_targetPointPointLight.Update();

	m_targetPointRender.Update();
}

void ENE::Render(RenderContext& rc)
{
	m_charaRender.Draw(rc);
	m_targetPointRender.Draw(rc);
}
