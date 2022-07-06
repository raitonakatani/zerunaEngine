#include "stdafx.h"
#include "index.h"
#include "Player.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

index::index()
{

}

index::~index()
{

}

bool index::Start()
{
	m_position.y = -150.0f;

	//�󔠂̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/treasure/treasure.tkm", false);
	m_modelRender.SetScale({ 6.0f,6.0f,6.0f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	//�ÓI�����I�u�W�F�N�g�̏����������B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//�v���C���[�̃I�u�W�F�N�g��T���Ď����Ă���B
	m_player = FindGO<Player>("player");

	return(true);
}

void index::Update()
{

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);

	//�G�`������̍X�V�����B
	m_modelRender.Update();


	//�v���C���[����󔠂Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_player->GetPosition() - m_position;
	//�x�N�g���̒�����150.0f��菬����������B
	if (diff.Length() < 2000.0f && m_player->GetPlayerHaveIndex() < 4)
	{
		//���C�g�̒����B
		if (light >= 0.15f) {
			light -= 0.0025f;
			light2 -= 0.0025f;
			light3 += 0.2f;
		}
		//������Â�����B
		//�󔠂ɃX�|�b�g���C�g�𓖂Ă�B
		m_player->SetPlayerHaveIndex(1);
		g_Light.SetLigSpot({ m_position.x,200.0f ,m_position.z });
		g_Light.SetLigSpotColor({ light3,light3,light3 });
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
	}
	if (diff.Length() >= 2000.0f) {
		if (g_pad[0]->IsPress(enButtonY)) {
			return;
		}
		//���C�g�̒����B
		if (light < 0.4f) {
			light += 0.0025f;
			light2 += 0.0025f;
			light3 -= 0.2f;
		}
		if (light >= 0.4f) {
			m_player->SetPlayerHaveIndex(0);
		}
		//����𖾂邭����B
		//�X�|�b�g���C�g�������B
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
		g_Light.SetLigSpotColor({ light3,light3,light3 });
	}

	if (diff.Length() <= 1500.0f && m_player->GetPlayerHaveIndex() == 1)
	{
		m_player->SetPlayerHaveIndex(2);
	}

	if (diff.Length() <= 200.0f && m_player->GetPlayerHaveIndex() == 2)
	{
		m_player->SetPlayerHaveIndex(3);
	}
}

void index::Render(RenderContext& rc)
{
	//�󔠂�`�悷��B
	m_modelRender.Draw(rc);
}