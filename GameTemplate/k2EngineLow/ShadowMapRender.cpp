#include "k2EngineLowPreCompile.h"
#include "ShadowMapRender.h"

namespace nsK2EngineLow {



	void ShadowMapRender::Init()
	{


        // �V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����
        float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        shadowMap.Create(
            4096,
            4096,
            1,
            1,
            DXGI_FORMAT_R32_FLOAT,
            DXGI_FORMAT_D32_FLOAT,
            clearColor
        );
	}

    void ShadowMapRender::Render(RenderContext& rc, Vector3 ligDirection, std::vector< IRenderer* >& renderObjects)
    {
        /*
        // �e�`��p�̃��C�g�J�������쐬����
        Camera lightCamera;
        */
        //�J�����̈ʒu��ݒ�B����̓��C�g�̒u�B
        m_lightCamera.SetPosition({ g_camera3D->GetTarget().x- 500.0f,g_camera3D->GetTarget().y + 600.0f,g_camera3D->GetTarget().z -500.0f});
        //�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
        m_lightCamera.SetTarget(g_camera3D->GetTarget());
        //�y���ځz�������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���B
        m_lightCamera.SetUp(1, 0, 0);
        m_lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
        m_lightCamera.SetWidth(3000.0f);
        m_lightCamera.SetHeight(3000.0f);
        //����̃T���v���ł͉�p�����߂ɂ��Ă����B
        //m_lightCamera.SetViewAngle(Math::DegToRad(20.0f));
        //���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
        m_lightCamera.Update();


        rc.WaitUntilToPossibleSetRenderTarget(shadowMap);
        rc.SetRenderTargetAndViewport(shadowMap);
        rc.ClearRenderTargetView(shadowMap);

        for (auto& model : renderObjects)
        {
            model->OnRenderShadowMap(rc, m_lightCamera.GetViewProjectionMatrix());
        }

        rc.WaitUntilFinishDrawingToRenderTarget(shadowMap);
        // �ʏ탌���_�����O
        // �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂�
        rc.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );
        rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
    }
}