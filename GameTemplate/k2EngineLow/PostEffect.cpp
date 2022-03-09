#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
    PostEffect g_posteffect;

    auto& renderContext = g_graphicsEngine->GetRenderContext();

    void PostEffect::Init()
    {
        // step-1 ���C�������_�����O�^�[�Q�b�g���쐬����
 //       RenderTarget mainRenderTarget;
        mainRenderTarget.Create(
            1600,
            900,
            1,
            1,
            //�y���ځz�J���[�o�b�t�@�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���B
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            DXGI_FORMAT_D32_FLOAT
        );

        //�𑜓x�A�~�b�v�}�b�v���x��
        luminnceRenderTarget.Create(
            1600,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
            900,		//�𑜓x�̓��C�������_�����O�^�[�Q�b�g�Ɠ����B
            1,
            1,
            //�y���ځz�J���[�o�b�t�@�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���B
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            DXGI_FORMAT_D32_FLOAT
        );
    }

    void PostEffect::SpriteInit()
    {
        //�P�x���o�p�̃X�v���C�g��������
        //�����������쐬����B
        //�P�x���o�p�̃V�F�[�_�[�̃t�@�C���p�X���w�肷��B
        luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
        //���_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
        luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
        //�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
        luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
        //�X�v���C�g�̕��ƍ�����luminnceRenderTarget�Ɠ����B
        luminanceSpriteInitData.m_width = 1600;
        luminanceSpriteInitData.m_height = 900;
        //�e�N�X�`���̓��C�������_�����O�^�[�Q�b�g�̃J���[�o�b�t�@�B
        luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
        //�`�����ރ����_�����O�^�[�Q�b�g�̃t�H�[�}�b�g���w�肷��B
        luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    
        luminanceSprite.Init(luminanceSpriteInitData);
   
        //�쐬�����������������ƂɃX�v���C�g������������B
       
        luminanceSprite.Init(luminanceSpriteInitData);
        // step-5 �K�E�V�A���u���[��������
        gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());

        // step-6 �{�P�摜�����Z��������X�v���C�g��������
        //����������ݒ肷��B
        finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
        //�𑜓x��mainRenderTarget�̕��ƍ����B
        finalSpriteInitData.m_width = 1600;
        finalSpriteInitData.m_height = 900;
        //�ڂ������摜���A�ʏ��2D�Ƃ��ă��C�������_�����O�^�[�Q�b�g�ɕ`�悷��̂ŁA
        //2D�p�̃V�F�[�_�[���g�p����B
        finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
        //�������A���Z�����ŕ`�悷��̂ŁA�A���t�@�u�����f�B���O���[�h�����Z�ɂ���B
        finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
        //�J���[�o�b�t�@�̃t�H�[�}�b�g�͗�ɂ���āA32�r�b�g���������_�o�b�t�@�B
        finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

        //�������������ɉ��Z�����p�̃X�v���C�g������������B
        finalSprite.Init(finalSpriteInitData);
        // step-7 �e�N�X�`����\��t���邽�߂̃X�v���C�g������������
        //�X�v���C�g�̏������I�u�W�F�N�g���쐬����B
        //�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
        spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
        spriteInitData.m_width = 1600;
        spriteInitData.m_height = 900;
        //���m�N���p�̃V�F�[�_�[���w�肷��B
        spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
        //�������I�u�W�F�N�g���g���āA�X�v���C�g������������B
        copyToFrameBufferSprite.Init(spriteInitData);
    }
    
    void PostEffect::maint()
    {

        //�����_�����O�^�[�Q�b�g��mainRenderTarget�ɕύX����B
          //�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
        renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        //�����_�����O�^�[�Q�b�g��ݒ�B
        renderContext.SetRenderTargetAndViewport(mainRenderTarget);
        //�����_�����O�^�[�Q�b�g���N���A�B
        renderContext.ClearRenderTargetView(mainRenderTarget);
    }

    void PostEffect::Render()
    {

        //mainRenderTarget�Ɋe�탂�f����`�悷��B
        //plModel.Draw(renderContext);
        //�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
        renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

        //�P�x���o
        //�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX�B
        renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
        //�����_�����O�^�[�Q�b�g��ݒ�B
        renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);
        //�����_�����O�^�[�Q�b�g���N���A�B
        renderContext.ClearRenderTargetView(luminnceRenderTarget);
        //�P�x���o���s���B
        luminanceSprite.Draw(renderContext);
        //�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
        renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);

        //�K�E�V�A���u���[�����s����B
        gaussianBlur.ExecuteOnGPU(renderContext, 20);

        //�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z�����B
        //�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
        renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        //�����_�����O�^�[�Q�b�g��ݒ�B
        renderContext.SetRenderTargetAndViewport(mainRenderTarget);
        //�ŏI�����B
        finalSprite.Draw(renderContext);
        //�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
        renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

        //���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[�B
        renderContext.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );


        if (g_pad[0]->IsPress(enButtonA))
        {
            	copyToFrameBufferSprite.Draw(renderContext);
        }
    }
}