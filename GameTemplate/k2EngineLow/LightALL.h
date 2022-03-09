#pragma once

namespace nsK2EngineLow {


	class LightALL
	{
        /// <summary>
       /// ���C�g�\���́B
       /// </summary>
    public:

        struct Directionlight {
            Vector3 dirDirection;	//���C�g�̕����B
            float pad0;
            Vector3 dirColor;
            float pad1;
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
        struct Ambientlight {
            Vector3 ambientLight;
            float pad;
        };

        struct Light {

            Pointlight pointlight; //�|�C���g���C�g�B
            Ambientlight ambientlight; //�A���r�G���g���C�g�B(����)
            Directionlight directionlight; //�f�B���N�V�������C�g�B
            Spotlight spotlight; //�X�|�b�g���C�g�B
            Vector3 eyePos;			//���_�̈ʒu�B
        };


    public:
        LightALL() {}
        ~LightALL();

        void Update();

      //       void InitDirectionLight(Directionlight& m_light);
      //       void InitPointLight(Pointlight& m_light);
      //       void InitAmbientLight(Ambientlight& m_light);
        void Init();

               /// <summary>
               /// �f�B���N�V�������C�g�̕����B
               /// </summary>
               /// <param name="light">����</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.directionlight.dirDirection = light;
        }
        /// <summary>
        /// �f�B���N�V�������C�g�̃J���[�B
        /// </summary>
        /// <param name="light">�J���[</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.directionlight.dirColor = color;
        }



        /// <summary>
         /// �|�C���g���C�g�̏ꏊ�B
         /// </summary>
         /// <param name="light">�ꏊ</param>
        void SetLigPoint(const Vector3& lightposi)
        {
            m_light.pointlight.ptPosition = lightposi;
        }

        /// <summary>
         /// �|�C���g���C�g�̃J���[�B
         /// </summary>
         /// <param name="light">�J���[</param>
        void SetLigPointColor(const Vector3& lightcola)
        {
            m_light.pointlight.ptColor = lightcola;
        }
        /// <summary>
        /// �͈͂�ݒ�B
        /// </summary>
        /// <param name="range"></param>
        void SetPointRange(float range)
        {
            m_light.pointlight.ptRange = range;
        }

        /// <summary>
         /// �X�|�b�g���C�g�̉�]�B
         /// </summary>
         /// <param name="light">��]</param>
        void SetLigSpotRot(const Vector3& lightrot)
        {
            m_light.spotlight.spDirection = lightrot;
        }

        /// <summary>
        /// �X�|�b�g���C�g�̏ꏊ�B
        /// </summary>
        /// <param name="light">�ꏊ</param>
        void SetLigSpot(const Vector3& posi)
        {
            m_light.spotlight.spPosition = posi;
        }

        

        Light& GetDirectionalLight()
        {
            return m_light;
        }



    private:
        Light               m_light;  //�V�[�����C�g�B
   
        float posi = 0.0f;

        ModelRender         m_modelRender;
        ModelInitData       m_InitData;
    };
    //�O���[�o���ϐ��B (extern)
    extern LightALL g_Light;
};

