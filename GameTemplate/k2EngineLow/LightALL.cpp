#include "k2EngineLowPreCompile.h"
#include "LightALL.h"

namespace nsK2EngineLow {
	LightALL g_Light;


	void LightALL::Init()
	{
		// ���C�g�̃f�[�^���쐬����
/*
		// �f�B���N�V�������C�g������������
		InitDirectionLight(m_light);
		// �|�C���g���C�g������������
		InitPointLight(m_light);
		// �A���r�G���g���C�g������������
		InitAmbientLight(m_light);
*/
		// ���C�g�͉E�����瓖�����Ă���
		m_light.directionlight.dirDirection.x = 1.0f;
		m_light.directionlight.dirDirection.y = -1.0f;
		m_light.directionlight.dirDirection.z = 1.0f;
		m_light.directionlight.dirDirection.Normalize();

		// ���C�g�̃J���[��(*��֥)/ʰ�
		m_light.directionlight.dirColor.x = 0.5f;
		m_light.directionlight.dirColor.y = 0.5f;
		m_light.directionlight.dirColor.z = 0.5f;

		// ���_�̈ʒu��ݒ肷��
		m_light.eyePos =g_camera3D->GetPosition(); 

		// �|�C���g���C�g�̍��W��ݒ肷��
		m_light.pointlight.ptPosition.x = 0.0f;
		m_light.pointlight.ptPosition.y = 50.0f;
		m_light.pointlight.ptPosition.z = 50.0f;

		// �|�C���g���C�g�̃J���[��ݒ肷��
		m_light.pointlight.ptColor.x = 0.5f;
		m_light.pointlight.ptColor.y = 0.5f;
		m_light.pointlight.ptColor.z = 0.5f;

		// �|�C���g���C�g�̉e���͈͂�ݒ肷��
		m_light.pointlight.ptRange = 100.0f;

		// �A���r�G���g���C�g
		m_light.ambientlight.ambientLight.x = 0.3f;
		m_light.ambientlight.ambientLight.y = 0.3f;
		m_light.ambientlight.ambientLight.z = 0.3f;


		// ���_�̈ʒu��ݒ肷��
		m_light.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });

		// step-2 �X�|�b�g���C�g�̃f�[�^������������
	  //�������W��X = 0�AY = 50�AZ = 0�ɂ���B
		m_light.spotlight.spPosition.x = 0.0f;
		m_light.spotlight.spPosition.y = 100.0f;
		m_light.spotlight.spPosition.z = 50.0f;
		//���C�g�̃J���[��ݒ�BR = 10�AG = 10�AB = 10�ɂ���B
		m_light.spotlight.spColor.x = 10.0f;
		m_light.spotlight.spColor.y = 1.0f;
		m_light.spotlight.spColor.z = 1.0f;

		//���������͎΂߉��ɂ���B
		m_light.spotlight.spDirection.x = 0.0f;
		m_light.spotlight.spDirection.y = -0.5f;
		m_light.spotlight.spDirection.z = -0.5f;
		//�����f�[�^�Ȃ̂ŁA�傫�����P�ɂ���K�v������̂Ő��K������B
		m_light.spotlight.spDirection.Normalize();

		//�ˏo�͈͂�300
		m_light.spotlight.spRange = 500.0f;
		//�ˏo�p�x��25�x�B
		m_light.spotlight.spAngle = Math::DegToRad(25.0f);

	}


	LightALL::~LightALL()
	{

	}

	void LightALL::Update()
	{


	}

/*
	void LightALL::InitDirectionLight(Directionlight& m_light)
	{
		// ���C�g�͉E�����瓖�����Ă���
		m_light.dirDirection.x = 0.0f;
		m_light.dirDirection.y = -0.3f;
		m_light.dirDirection.z = 1.0f;
		m_light.dirDirection.Normalize();

		// ���C�g�̃J���[��(*��֥)/ʰ�
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

	
	}

	void LightALL::InitPointLight(Pointlight& m_light)
	{
		// �|�C���g���C�g�̍��W��ݒ肷��
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 0.0f;

		// �|�C���g���C�g�̃J���[��ݒ肷��
		m_light.ptColor.x = 0.5f;
		m_light.ptColor.y = 0.5f;
		m_light.ptColor.z = 0.5f;

		// �|�C���g���C�g�̉e���͈͂�ݒ肷��
		m_light.ptRange = 100.0f;
	}			

	void LightALL::InitAmbientLight(Ambientlight& m_light)
	{
		// �A���r�G���g���C�g
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;
	}
	*/

}