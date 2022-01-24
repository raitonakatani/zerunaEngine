#include "k2EngineLowPreCompile.h"
#include "DirectionalLight.h"

namespace nsK2EngineLow {
	DirectionalLight g_directionalLight;


	DirectionalLight::DirectionalLight()
	{
		// �f�B���N�V�������C�g�̃f�[�^��ݒ肷��
	 // ���C�g�͉E�����瓖�����Ă���
		m_light.directionLight.dirDirection.x = 1.0f;
		m_light.directionLight.dirDirection.y = -1.0f;
		m_light.directionLight.dirDirection.z = -1.0f;
		m_light.directionLight.dirDirection.Normalize();

		// ���C�g�̃J���[�͔�
		m_light.directionLight.dirColor.x = 0.5f;//R ��
		m_light.directionLight.dirColor.y = 0.5f;//G ��
		m_light.directionLight.dirColor.z = 0.5f;//B ��

		//���_�̈ʒu��ݒ肷��B
		m_light.ambientlight.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });//g_camera3D->GetPosition();

		// ����
		m_light.ambientlight.ambientColor.x = 0.3f;
		m_light.ambientlight.ambientColor.y = 0.3f;
		m_light.ambientlight.ambientColor.z = 0.3f;
		
		// step-2 �|�C���g���C�g�̏������W��ݒ肷��
		m_light.pointlight.ptPosition.x = 0.0f;
		m_light.pointlight.ptPosition.y = 50.0f;
		m_light.pointlight.ptPosition.z = 50.0f;
		// step-3 �|�C���g���C�g�̏����J���[��ݒ肷��
		m_light.pointlight.ptColor.x = 0.0f;
		m_light.pointlight.ptColor.y = 0.0f;
		m_light.pointlight.ptColor.z = 0.0f;
		// step-4 �|�C���g���C�g�̉e���͈͂�ݒ肷��
		m_light.pointlight.ptRange = 100.0f;



		m_light.spotlight.spPosition.x = 0.0f;
		m_light.spotlight.spPosition.y = 0.0f;
		m_light.spotlight.spPosition.y = 50.0f;
		//���C�g�̃J���[��ݒ�BR = 10�AG = 10�AB = 10�ɂ���B
		m_light.spotlight.spColor.x = 10.0f;
		m_light.spotlight.spColor.y = 10.0f;
		m_light.spotlight.spColor.z = 10.0f;
		
		//���������͎΂߉��ɂ���B
		m_light.spotlight.spDirection.x = 1.0f;
		m_light.spotlight.spDirection.y = -1.0f;
		m_light.spotlight.spDirection.z = 1.0f;
		//�����f�[�^�Ȃ̂ŁA�傫�����P�ɂ���K�v������̂Ő��K������B
		m_light.spotlight.spDirection.Normalize();
		
		//�ˏo�͈͂�300
		m_light.spotlight.spRange = 300.0f;
		//�ˏo�p�x��25�x�B
		m_light.spotlight.spAngle = Math::DegToRad(25.0f);

	}


	DirectionalLight::~DirectionalLight()
	{
		
	}	
}