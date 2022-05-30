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

	//���̃��f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/treasure/treasure.tkm",false);
	m_modelRender.SetScale({ 6.0f,6.0f,6.0f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	//�ÓI�����I�u�W�F�N�g�̏����������B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//Portion
	//�v���C���[�̃I�u�W�F�N�g��T���Ď����Ă���B
	m_player = FindGO<Player>("player");

	m_sprite.Init("Assets/sprite/Title/button.dds", 800, 400);
	m_sprite.SetPosition(Vector3(0.0f, -150.0f, 0.0f));
	m_sprite.Update();

	return(true);
}

void index::Update()
{

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);

	//�G�`������̍X�V�����B
	m_modelRender.Update();


	//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_player->GetPosition() - m_position;
	//�x�N�g���̒�����150.0f��菬����������B
	if (diff.Length() < 2000.0f && m_player->index <4)
	{
		if (light >= 0.15f) {
			light -= 0.0025f;
			light2 -= 0.0025f;
			light3 += 0.15f;
		}

		m_player->index = 1;
		g_Light.SetLigSpot({ m_position.x,200.0f ,m_position.z });
		g_Light.SetLigSpotColor({light3,light3,light3 });
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
	}
	if(diff.Length() >= 2000.0f){
			if (light < 0.4f) {
			light += 0.005f;
			light2 += 0.005f;
			light3 -= 0.15f;
		}
			if (light >= 0.4f) {
				m_player->index = 0;
			}
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
		g_Light.SetLigSpotColor({ light3,light3,light3 });
	}

	if (diff.Length() <= 1000.0f && m_player->index == 1)
	{
		m_player->index = 2;
	}

	if (diff.Length() <= 200.0f && m_player->index == 2)
	{
		m_player->index = 3;
	}

	//wchar_t wcsbuf[256];
	//swprintf_s(wcsbuf, 256, L"%d", float(light));
	//m_font.SetText(wcsbuf);
	////m_font.SetText(L"Death");
	////�\��������W��ݒ肷��B
	//m_font.SetPosition(Vector3{ 0.0f,0.0f,0.0f });
	////�����̑傫����ς���B
	//m_font.SetScale(3.0f);

}

void index::Rotation()
{
	//��]�����Z����B
	rotation.AddRotationDegY(2.0f);

	//�G�`������ɉ�]��������B
	m_modelRender.SetRotation(rotation);
}

void index::Render(RenderContext& rc)
{
	m_font.Draw(rc);
	//����`�悷��B
	m_modelRender.Draw(rc);
	/*if (win == 1) {
		m_sprite.Draw(rc);
	}*/
}