#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

const int NUM_WEIGHTS = 8;
/// <summary>
/// �u���[�p�̃p�����[�^�[
/// </summary>
struct SBlurParam
{
	float weights[NUM_WEIGHTS];
};

// �֐��錾
void InitRootSignature(RootSignature& rs);


// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{



	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });

	auto game = NewGO<Game>(0);

	RootSignature rs;
	InitRootSignature(rs);

	//�Q�[���V�[����`�悷�郌���_�����O�^�[�Q�b�g���쐬
	g_posteffect.Init();
	g_posteffect.SpriteInit();

	// step-5 �K�E�V�A���u���[��������
	GaussianBlur gaussianBlur;
	gaussianBlur.Init(&g_posteffect.luminnceRenderTarget.GetRenderTargetTexture());
	// step-6 �{�P�摜�����Z��������X�v���C�g��������
	//����������ݒ肷��B
	SpriteInitData finalSpriteInitData;
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
	Sprite finalSprite;
	finalSprite.Init(finalSpriteInitData);
	// step-7 �e�N�X�`����\��t���邽�߂̃X�v���C�g������������
	//�X�v���C�g�̏������I�u�W�F�N�g���쐬����B
	SpriteInitData spriteInitData;
	//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
	spriteInitData.m_textures[0] = &g_posteffect.mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1600;
	spriteInitData.m_height = 900;
	//���m�N���p�̃V�F�[�_�[���w�肷��B
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//�������I�u�W�F�N�g���g���āA�X�v���C�g������������B
	Sprite copyToFrameBufferSprite;
	copyToFrameBufferSprite.Init(spriteInitData);

	g_Light.Init();

	auto& renderContext = g_graphicsEngine->GetRenderContext();
	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();


		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

		//�����_�����O�^�[�Q�b�g��mainRenderTarget�ɕύX����B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetView(g_posteffect.mainRenderTarget);

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

		//mainRenderTarget�Ɋe�탂�f����`�悷��B
		//plModel.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//�P�x���o
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX�B
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.luminnceRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(g_posteffect.luminnceRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetView(g_posteffect.luminnceRenderTarget);
		//�P�x���o���s���B
		g_posteffect.luminanceSprite.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.luminnceRenderTarget);

		//�K�E�V�A���u���[�����s����B
		gaussianBlur.ExecuteOnGPU(renderContext, 20);

		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z�����B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//�ŏI�����B
		finalSprite.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[�B
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);


		if (g_pad[0]->IsPress(enButtonA))
		{
		//	copyToFrameBufferSprite.Draw(renderContext);
		}

		g_renderingEngine.SpriteRenderDraw(renderContext);

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		g_Light.Update();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}

// ���[�g�V�O�l�`���̏�����
void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}
