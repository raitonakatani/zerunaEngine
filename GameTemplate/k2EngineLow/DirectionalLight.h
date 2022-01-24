#pragma once

namespace nsK2EngineLow {

    class DirectionalLight
    {
    public:
        /// <summary>
        /// ���C�g�\���́B
        /// </summary>
       /* struct Light {

            // �f�B���N�V�������C�g�p�̃����o
            Vector3 dirDirection;   // ���C�g�̕���
            float pad0;
            Vector3 dirColor;       // ���C�g�̃J���[
            float pad1;

            // step-1 ���C�g�\���̂Ƀ|�C���g���C�g�p�̃����o�ϐ���ǉ�����
            Vector3 ptPosition;		//�ʒu�B
            float pad2;					//�p�f�B���O�B
            Vector3 ptColor;			//�J���[�B
            float ptRange;				//�e���͈́B

            Vector3 eyePos;         // ���_�̈ʒu
            float pad3;
            Vector3 ambientLight;   // �A���r�G���g���C�g
        };
        */
        
        struct Directionlight {
            Vector3 dirDirection;	//���C�g�̕����B
            float pad0;
            Vector3 dirColor;
            float pad1;
        };
        struct Ambientlight {
            Vector3 eyePos;			//���_�̈ʒu�B
            float pad3;
            Vector3 ambientColor;
        };
        struct Pointlight { 
            Vector3 ptPosition;		//�ʒu�B
            float pad2;				//�p�f�B���O�B
            Vector3 ptColor;		//�J���[�B
            float ptRange;			//�e���͈́B
        };
        struct Spotlight {
            Vector3 spPosition;		//�ʒu�B
            float pad3;				//�p�f�B���O�B
            Vector3 spColor;		//�J���[�B
            float spRange;			//�e���͈́B
            Vector3 spDirection;	//�ˏo�����B
            float spAngle;			//�ˏo�p�x�B
        };
        /// <summary>
        /// ���C�g�\���́B
        /// </summary>
        struct Light {
           // g_PointLight.pointlight;
            Pointlight pointlight;               //�|�C���g���C�g�B
            Ambientlight ambientlight;           //�A���r�G���g���C�g�B(����)
            Directionlight directionLight;		//�f�B���N�V�������C�g�B
            Spotlight spotlight;                //�X�|�b�g���C�g�B
        };
        /**/

    public:
        DirectionalLight();
        ~DirectionalLight();

        /// <summary>
        /// ���C�g�̕����B
        /// </summary>
        /// <param name="light">����</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.directionLight.dirDirection = light;
        }
        /// <summary>
        /// ���C�g�̃J���[�B
        /// </summary>
        /// <param name="light">�J���[</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.directionLight.dirColor = color;
        }

        void SetLigSpot(const Vector3& posi)
        {
            m_light.spotlight.spPosition = posi;
        }


        /// <summary>
         /// ���C�g�̏ꏊ�B
         /// </summary>
         /// <param name="light">�ꏊ</param>
        void SetLigPoint(const Vector3& lightposi)
        {
            m_light.pointlight.ptPosition = lightposi;
        }

        Light& GetDirectionalLight()
        {
            return m_light;
        }

        Light& GetPointLight()
        {
            return m_light;
        }

    private:
        Light               m_light;  //�V�[�����C�g�B
        Light               m_pointlight;  //�V�[�����C�g�B
        ModelRender         m_modelRender;
        ModelInitData       m_InitData;
    };
    //�O���[�o���ϐ��B (extern)
    extern DirectionalLight g_directionalLight;
};