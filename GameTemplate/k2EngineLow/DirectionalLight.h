#pragma once

namespace nsK2EngineLow {

	class DirectionalLight
    {
    public:
        struct Directionlight {
           Vector3 ligDirection;	//���C�g�̕����B
           float pad0;
           Vector3 ligColor;
           float pad1;
       };
        struct Ambientlight {
            Vector3 ambientColor;
            float pad;
        };
       /// <summary>
       /// ���C�g�\���́B
       /// </summary>
       struct Light {
           Ambientlight ambientlight;
           Directionlight directionLight;		//�f�B���N�V�������C�g�B
           Vector3 eyePos;						//���_�̈ʒu�B
       };

    public:
        DirectionalLight();
        ~DirectionalLight();
    
        void Render(RenderContext& rc);

        /// <summary>
        /// ���C�g�̕����B
        /// </summary>
        /// <param name="light">����</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.directionLight.ligDirection = light;
        }

        /// <summary>
        /// ���C�g�̃J���[�B
        /// </summary>
        /// <param name="light">�J���[</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.directionLight.ligColor= color;
        }

        Light& GetDirectionalLight()
        {
            return m_light;
        }

    private:
        Light               m_light;  //�V�[�����C�g�B
        Model               model;
        ModelInitData       m_InitData;
    };

    //�O���[�o���ϐ��B (extern)
    extern DirectionalLight g_directionalLight;
}