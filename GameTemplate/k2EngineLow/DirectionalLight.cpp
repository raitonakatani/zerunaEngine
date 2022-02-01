#include "k2EngineLowPreCompile.h"
#include "DirectionalLight.h"

namespace nsK2EngineLow {
	DirectionalLight g_directionalLight;


	DirectionalLight::DirectionalLight()
	{
			// ���C�g�̃f�[�^���쐬����

/*		// �f�B���N�V�������C�g������������
		InitDirectionLight(m_light);
		// �|�C���g���C�g������������
		InitPointLight(m_light);
		// �A���r�G���g���C�g������������
		InitAmbientLight(m_light);
*/
		// ���C�g�͉E�����瓖�����Ă���
		m_light.dirDirection.x = 1.0f;
		m_light.dirDirection.y = -1.0f;
		m_light.dirDirection.z = -1.0f;
		m_light.dirDirection.Normalize();

		// ���C�g�̃J���[�͔�
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

		// ���_�̈ʒu��ݒ肷��
		m_light.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });

		// �|�C���g���C�g�̍��W��ݒ肷��
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;

		// �|�C���g���C�g�̃J���[��ݒ肷��
		m_light.ptColor.x = 10.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;

		// �|�C���g���C�g�̉e���͈͂�ݒ肷��
		m_light.ptRange = 100.0f;

		// �A���r�G���g���C�g
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;


		// step-2 �X�|�b�g���C�g�̃f�[�^������������
	  //�������W��X = 0�AY = 50�AZ = 0�ɂ���B
		m_light.spPosition.x = 0.0f;
		m_light.spPosition.y = 50.0f;
		m_light.spPosition.z = 0.0f;
		//���C�g�̃J���[��ݒ�BR = 10�AG = 10�AB = 10�ɂ���B
		m_light.spColor.x = 10.0f;
		m_light.spColor.y = 10.0f;
		m_light.spColor.z = 10.0f;

		//���������͎΂߉��ɂ���B
		m_light.spDirection.x = 1.0f;
		m_light.spDirection.y = -1.0f;
		m_light.spDirection.z = 1.0f;
		//�����f�[�^�Ȃ̂ŁA�傫�����P�ɂ���K�v������̂Ő��K������B
		m_light.spDirection.Normalize();

		//�ˏo�͈͂�300
		m_light.spRange = 300.0f;
		//�ˏo�p�x��25�x�B
		m_light.spAngle = Math::DegToRad(25.0f);
	}


	DirectionalLight::~DirectionalLight()
	{

	}

	/*
	void DirectionalLight::InitDirectionLight(Light& m_light)
	{
		// ���C�g�͉E�����瓖�����Ă���
		m_light.dirDirection.x = 1.0f;
		m_light.dirDirection.y = -1.0f;
		m_light.dirDirection.z = -1.0f;
		m_light.dirDirection.Normalize();

		// ���C�g�̃J���[�͔�
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

		// ���_�̈ʒu��ݒ肷��
		m_light.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });
	}

	void DirectionalLight::InitPointLight(Light& m_light)
	{
		// �|�C���g���C�g�̍��W��ݒ肷��
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;

		// �|�C���g���C�g�̃J���[��ݒ肷��
		m_light.ptColor.x = 0.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;

		// �|�C���g���C�g�̉e���͈͂�ݒ肷��
		m_light.ptRange = 100.0f;
	}

	void DirectionalLight::InitAmbientLight(Light& m_light)
	{
		// �A���r�G���g���C�g
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;
	}
	*/
}