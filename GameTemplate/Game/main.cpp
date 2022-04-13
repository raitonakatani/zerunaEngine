#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"

/// <summary>
/// �f�B���N�V���i�����C�g
/// </summary>
struct DirectionalLight
{
	Vector3  color;
	float pad0;         // �p�f�B���O
	Vector3  direction;
	float pad1;         // �p�f�B���O
	Vector3 eyePos; // ���_
	float specPow;  // �X�y�L�����̍i��
};

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
	g_camera3D->SetPosition({ 0.0f, 100.0f, -100.0f });
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });

//	auto game = NewGO<Game>(0);
auto title = NewGO<Title>(0);
	NewGO<Fade>(0, "fade");

	RootSignature rs;
	InitRootSignature(rs);

	//�Q�[���V�[����`�悷�郌���_�����O�^�[�Q�b�g���쐬
	g_posteffect.Init();
	g_posteffect.SpriteInit();


	g_Light.Init();




	//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����B
	//�J���[�o�b�t�@�̃N���A�J���[
	//����̓J���[�o�b�t�@�͐^�����ɂ���B
/*	float clearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	RenderTarget shadowMap;
	shadowMap.Create(
		1024,//�y���ځz�����_�����O�^�[�Q�b�g�̉���
		1024,//�y���ځz�����_�����O�^�[�Q�b�g�̏c��
		1,
		1,
		DXGI_FORMAT_R32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);

	//�e�`��p�̃��C�g�J�������쐬����B
	Camera lightCamera;
	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	lightCamera.SetPosition(0, 600, 0);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	lightCamera.SetTarget(0, 0, 0);
	//�y���ځz�������ݒ�B����̓��C�g���^���������Ă���̂ŁAX��������ɂ��Ă���B
	lightCamera.SetUp(1, 0, 0);
	//����̃T���v���ł͉�p�����߂ɂ��Ă����B
	lightCamera.SetViewAngle(Math::DegToRad(20.0f));
	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	lightCamera.Update();

	//�V���h�E�}�b�v�`��p�̃��f����p�ӂ���B
	ModelInitData teapotShadowModelInitData;
	//�y���ځz�V���h�E�}�b�v�`��p�̃V�F�[�_�[���w�肷��B
	teapotShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	Model teapotShadowModel;
	teapotShadowModel.Init(teapotShadowModelInitData);
	teapotShadowModel.UpdateWorldMatrix(
		{ 0, 50, 0 },
		g_quatIdentity,
		g_vec3One
	);

	// �ʏ�`��̃e�B�[�|�b�g���f����������
	ModelRender teapotModel;
	teapotModel.Init("Assets/modelData/unityChan.tkm");
	

	// �V���h�E�}�b�v��\�����邽�߂̃X�v���C�g������������
	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &shadowMap.GetRenderTargetTexture();
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData.m_width = 256;
	spriteInitData.m_height = 256;

	Sprite sprite;
	sprite.Init(spriteInitData);

	ModelInitData bgModelInitData;
	//�e�����Ƃ���郂�f���p�̃V�F�[�_�[���w�肷��B
	bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
	//�V���h�E�}�b�v���g��SRV�ɐݒ肷��B
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
	//���C�g�r���[�v���W�F�N�V�����s����g���萔�o�b�t�@�ɐݒ肷��B
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model bgModel;
	bgModel.Init(bgModelInitData);

*/

	//�X�e�b�N�̓��͗ʂ��擾
	float lStick_x = 0.0f;
	float lStick_y = 0.0f;



	// ���[�g�V�O�l�`�����쐬
	RootSignature rootSignature;
	InitRootSignature(rootSignature);


	// �f�B���N�V�������C�g
	DirectionalLight light;
	light.direction.x = 1.0f;
	light.direction.y = -1.0f;
	light.direction.z = -1.0f;
	light.direction.Normalize();

	light.color.x = 1.0f;
	light.color.y = 1.0f;
	light.color.z = 1.0f;
	light.eyePos = g_camera3D->GetPosition();

	ModelRender model;
	model.InitModel("Assets/modelData/player/player2.tkm");


	// ���f����������
	ModelInitData modelInitData;
	// �l�^���f����������
	modelInitData.m_tkmFilePath = "Assets/modelData/player/player2.tkm";
	modelInitData.m_fxFilePath = "Assets/shader/model1.fx";
	Model humanModel;
	humanModel.Init(modelInitData);
	humanModel.UpdateWorldMatrix({ 0.0f, 0.0f, 0.0f }, g_quatIdentity, g_vec3One);

	// �w�i���f����������
	modelInitData.m_tkmFilePath = "Assets/modelData/karisute/stage3.tkm";
	Model bgModel;
	bgModel.Init(modelInitData);


	RenderTarget albedRT;
	RenderTarget normalRT;
	RenderTarget worldPosRT;
	SpriteInitData spriteInitData;
	Sprite defferdLightinSpr;

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

	spriteInitData.m_fxFilePath = "Assets/shader/sprite1.fx";
	spriteInitData.m_expandConstantBuffer = &light;
	spriteInitData.m_expandConstantBufferSize = sizeof(light);
	// �������f�[�^���g���ăX�v���C�g���쐬
	defferdLightinSpr.Init(spriteInitData);



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

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		g_Light.Update();


	/*	if (g_pad[0]->IsPress(enButtonA))
		{
			model.modelUpdate();

			// �����_�����O�^�[�Q�b�g��G-Buffer�ɕύX���ď�������
			RenderTarget* rts[] = {
				&albedRT, // 0�Ԗڂ̃����_�����O�^�[�Q�b�g
				&normalRT, // 1�Ԗڂ̃����_�����O�^�[�Q�b�g
				&worldPosRT // 2�Ԗڂ̃����_�����O�^�[�Q�b�g
			};

			// �܂��A�����_�����O�^�[�Q�b�g�Ƃ��Đݒ�ł���悤�ɂȂ�܂ő҂�
			renderContext.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
			// �����_�����O�^�[�Q�b�g��ݒ�
			renderContext.SetRenderTargets(ARRAYSIZE(rts), rts);
			// �����_�����O�^�[�Q�b�g���N���A
			renderContext.ClearRenderTargetViews(ARRAYSIZE(rts), rts);


			humanModel.Draw(renderContext);
			bgModel.Draw(renderContext);


			// �����_�����O�^�[�Q�b�g�ւ̏������ݑ҂�
			renderContext.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

			// �����_�����O����t���[���o�b�t�@�[�ɖ߂��ăX�v���C�g�������_�����O����
			g_graphicsEngine->ChangeRenderTargetToFrameBuffer(renderContext);
			// G-Buffer�̓��e�����ɂ��ăf�B�t�@�[�h���C�e�B���O
			defferdLightinSpr.Draw(renderContext);

			// step-2 �[�x�X�e���V���r���[��G-Buffer���쐬�����Ƃ��̂��̂ɕύX����
			//�[�x�X�e���V���r���[��G-Buffer���쐬�����Ƃ��̂��̂ɕύX����B
			renderContext.SetRenderTarget(
				g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
				rts[0]->GetDSVCpuDescriptorHandle()
			);

			model.Draw(renderContext);
		}
		else {
*/			g_renderEngine.SpriteRenderDraw(renderContext);
	//	}

/*
		if (g_pad[0]->IsPress(enButtonA))
		{
			lStick_y++;
		}
		if (g_pad[0]->IsPress(enButtonB))
		{
			lStick_y--;
		}
		if (g_pad[0]->IsPress(enButtonX))
		{
			lStick_x++;
		}
		if (g_pad[0]->IsPress(enButtonY))
		{
			lStick_x--;
		}
		teapotModel.SetPosition({ lStick_x ,lStick_y ,0.0f });
		teapotModel.Update();
		teapotShadowModel.UpdateWorldMatrix(
			{ lStick_x ,lStick_y ,0.0f },
			g_quatIdentity,
			g_vec3One);

*/
		// �e�𐶐����������f�����V���h�E�}�b�v�ɕ`�悷��B
		//�����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����B
	//	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	//	renderContext.SetRenderTargetAndViewport(shadowMap);
	//	renderContext.ClearRenderTargetView(shadowMap);

		//�e���f����`��B
	//	teapotShadowModel.Draw(renderContext, lightCamera);

		//�������݊����҂��B
	//	renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);


		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
	//	g_k2EngineLow->ExecuteRender();

		//�����_�����O�^�[�Q�b�g��mainRenderTarget�ɕύX����B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
//		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
//		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
//		renderContext.ClearRenderTargetView(g_posteffect.mainRenderTarget);

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
	//	g_k2EngineLow->ExecuteRender();

		//mainRenderTarget�Ɋe�탂�f����`�悷��B
		//plModel.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
//		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//�P�x���o
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX�B
//		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.luminnceRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
//		renderContext.SetRenderTargetAndViewport(g_posteffect.luminnceRenderTarget);
		//�����_�����O�^�[�Q�b�g���N���A�B
//		renderContext.ClearRenderTargetView(g_posteffect.luminnceRenderTarget);
		//�P�x���o���s���B
//		g_posteffect.luminanceSprite.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
//		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.luminnceRenderTarget);

		//�K�E�V�A���u���[�����s����B
///		g_posteffect.gaussianBlur.ExecuteOnGPU(renderContext, 20);

		//�{�P�摜�����C�������_�����O�^�[�Q�b�g�ɉ��Z�����B
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
//		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//�����_�����O�^�[�Q�b�g��ݒ�B
//		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//�ŏI�����B
//		g_posteffect.finalSprite.Draw(renderContext);
		//�����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂��B
//		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�ɃR�s�[�B
//		renderContext.SetRenderTarget(
//			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
//			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
//		);


//		if (g_pad[0]->IsPress(enButtonA))
//		{
	//		g_posteffect.copyToFrameBufferSprite.Draw(renderContext);
//		}

	





	//	teapotModel.Draw(renderContext);
	//	bgModel.Draw(renderContext);

	//	sprite.Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });
	//	sprite.Draw(renderContext);

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
