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
	m_modelRender.Init("Assets/modelData/Portion/Portion.tkm");
	//m_modelRender.SetScale(100.0f, 100.0f, 100.0f);
	//Portion
	//�v���C���[�̃I�u�W�F�N�g��T���Ď����Ă���B
	m_player = FindGO<Player>("player");

	return(true);
}

void index::Update()
{

	//��]�����B
	Rotation();

	//�G�`������ɍ��W��������B
	m_modelRender.SetPosition(m_position);

	//�G�`������̍X�V�����B
	m_modelRender.Update();


	//�v���C���[���灙�Ɍ������x�N�g�����v�Z�B
	Vector3 diff = m_player->GetPosition() - m_position;
	//�x�N�g���̒�����120.0f��菬����������B
	if (diff.Length() <= 120.0f)
	{
		//�J�E���g��+1����B
		m_player->Plusindex();

		g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
		//���ʉ����Đ�����B
		SoundSource* Getse = NewGO<SoundSource>(0);
		Getse->Init(4);
		Getse->Play(false);
		Getse->SetVolume(0.8f);

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
}