#pragma once
namespace nsK2EngineLow {


    class PointLight
    {
    public:
        struct Pointlight {
            // step-1 ���C�g�\���̂Ƀ|�C���g���C�g�p�̃����o�ϐ���ǉ�����
            Vector3 ptPosition;		//�ʒu�B
            float pad2;					//�p�f�B���O�B
            Vector3 ptColor;			//�J���[�B
            float ptRange;				//�e���͈́B
        };
        Pointlight pointlight;



    };

    //�O���[�o���ϐ��B (extern)
    extern PointLight g_PointLight;
};