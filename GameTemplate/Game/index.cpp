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
	if (diff.Length() <= 150.0f) 
	{
		win = 1;
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�J�E���g��+1����B
			m_player->Plusindex();
			win = 2;
			g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
			//���ʉ����Đ�����B
			SoundSource* Getse = NewGO<SoundSource>(0);
			Getse->Init(4);
			Getse->Play(false);
			Getse->SetVolume(0.4f);
		}
	}
	else {
		win = 0;
	}

	if (m_player->Getindex() == 2)
	{
		//���M���폜����B
		DeleteGO(this);
	}
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
	//����`�悷��B
	m_modelRender.Draw(rc);
	if (win == 1) {
		m_sprite.Draw(rc);
	}
}