#include "k2EngineLowPreCompile.h"
#include "touka.h"

namespace nsK2EngineLow {
 //   touka g_touka;

	void touka::Init(){
       

        // G-Buffer���쐬
        // �A���x�h�J���[�������ݗp�̃����_�����O�^�[�Q�b�g
        albedRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
        // �@���������ݗp�̃����_�����O�^�[�Q�b�g
        normalRT.Create(
            FRAME_BUFFER_W,
            FRAME_BUFFER_H,
            1,
            1,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            DXGI_FORMAT_UNKNOWN
        );
        worldPosRT.Create(
            FRAME_BUFFER_W,
            FRAME_BUFFER_H,
            1,
            1,
            DXGI_FORMAT_R32G32B32A32_FLOAT, // ���[���h���W���L�^����̂ŁA32�r�b�g���������_�o�b�t�@�[�𗘗p����
            DXGI_FORMAT_UNKNOWN
        );

        // �|�X�g�G�t�F�N�g�I�Ƀf�B�t�@�[�h���C�e�B���O���s�����߂̃X�v���C�g��������
        // ��ʑS�̂Ƀ����_�����O����̂ŕ��ƍ����̓t���[���o�b�t�@�[�̕��ƍ����Ɠ���
        spriteInitData.m_width = FRAME_BUFFER_W;
        spriteInitData.m_height = FRAME_BUFFER_H;
        // �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`����ݒ�
        spriteInitData.m_textures[0] = &albedRT.GetRenderTargetTexture();
        spriteInitData.m_textures[1] = &normalRT.GetRenderTargetTexture();

        // �f�B�t�@�[�h���C�e�B���O�Ŏg�p����e�N�X�`���Ƀ��[���h���W�e�N�X�`����ǉ�
        spriteInitData.m_textures[2] = &worldPosRT.GetRenderTargetTexture();

        spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
        spriteInitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
        spriteInitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());
        // �������f�[�^���g���ăX�v���C�g���쐬
        defferdLightinSpr.Init(spriteInitData);
	}

    void touka::Draw(RenderContext& rc) {
        // �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX���ď�������
        RenderTarget* rts[] = {
            &albedRT, // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
            &normalRT, // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
            &worldPosRT // 2�Ԗڂ̃����_�����O�^�[�Q�b�g
        };

        // �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
        rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
        // �����_�����O�^�[�Q�b�g��ݒ�
        rc.SetRenderTargets(ARRAYSIZE(rts), rts);
        // �����_�����O�^�[�Q�b�g���N���A
        rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);


        //humanModel.Draw(rc);
        //bgModel.Draw(rc);


        // �����_�����O�^�[�Q�b�g�ւ̏������ݑ҂�
        rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

        // �����_�����O����t���[���o�b�t�@�[�ɖ߂��ăX�v���C�g�������_�����O����
        g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
        // G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
        defferdLightinSpr.Draw(rc);

        // step-2 �[�x�X�e���V���r���[��G-Buffer���쐬�����Ƃ��̂��̂ɕύX����
        //�[�x�X�e���V���r���[��G-Buffer���쐬�����Ƃ��̂��̂ɕύX����B
        rc.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            rts[0]->GetDSVCpuDescriptorHandle()
        );
    }



}