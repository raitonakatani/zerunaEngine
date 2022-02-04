#include "stdafx.h"
#include "system/system.h"
#include "Game.h"



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

	//float4 a = { 1.0f,1.0f, 1.0f, 1.0f, };

	float a[4] = { 1.0f,1.0f,1.0f,0.0f };

	//RenderTarget�N���X�̃I�u�W�F�N�g���`�B
	RenderTarget offscreenRenderTarget;
	//RenderTarget::Create()�𗘗p���āA�����_�����O�^�[�Q�b�g���쐬����B
	offscreenRenderTarget.Create(
		1600,						//�e�N�X�`���̕��B
		900,						//�e�N�X�`���̍����B
		1,							//Mipmap���x���B
		1,							//�e�N�X�`���z��̃T�C�Y�B
		DXGI_FORMAT_R8G8B8A8_UNORM, //�J���[�o�b�t�@�̃t�H�[�}�b�g�B
		DXGI_FORMAT_D32_FLOAT		//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B
		//a
	);

			ModelRender m;

		// �e�탂�f��������������
		// �w�i���f����������
		ModelInitData bgModelInitData;
		bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
		bgModelInitData.m_fxFilePath = "Assets/shader/3D.fx";

		// �w�i���f����������
		Model bgModel;
		bgModel.Init(bgModelInitData);

		// �v���C���[���f����������
		ModelInitData plModelInitData;
		plModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
		plModelInitData.m_fxFilePath = "Assets/shader/3D.fx";
		Model plModel;
		plModel.Init(plModelInitData);


	//�X�v���C�g�̏������I�u�W�F�N�g���쐬����B
	SpriteInitData spriteInitData;
	//�y�d�v�I�I�I�z�e�N�X�`���̓I�t�X�N���[�������_�����O���ꂽ�G�B
	spriteInitData.m_textures[0] = &offscreenRenderTarget.GetRenderTargetTexture();
	//�y�d�v�I�I�I�z�S��ʕ`��Ȃ̂ŃX�v���C�g�̃T�C�Y�̓t���[���o�b�t�@�Ɠ����ɂ���B
	spriteInitData.m_width = 1600;
	spriteInitData.m_height = 900;
	//�y�d�v�I�I�I�z���m�N���p�̃V�F�[�_�[���w�肷��B
	spriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	//�������I�u�W�F�N�g���g���āA�X�v���C�g������������B
	Sprite monochromeSprite;
	monochromeSprite.Init(spriteInitData);

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


		//�����_�����O�^�[�Q�b�g��offscreenRenderTarget�ɕύX����B
		RenderTarget* rtArray[] = { &offscreenRenderTarget };
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargets(1, rtArray);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetViews(1, rtArray);

		//offscreenRenderTarget�ɔw�i�A�v���C���[��`�悷��B
		//�w�i���f�����h���[�B		
		bgModel.Draw(renderContext);
		//�v���C���[���h���[�B
		plModel.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
		renderContext.WaitUntilFinishDrawingToRenderTargets(1, rtArray);

		//��ʂɕ\������郌���_�����O�^�[�Q�b�g�ɖ߂��B
			renderContext.SetRenderTarget(
				g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
				g_graphicsEngine->GetCurrentFrameBuffuerDSV()
			);

		monochromeSprite.Draw(renderContext);

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		g_Light.Update();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
