#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

/*
struct DirectionLight {
	Vector3 ligDirection;	//���C�g�̕����B
	float pad;
	Vector3 ligColor;
};
*/

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
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	auto game = NewGO<Game>(0);

/*	//step-2 �f�B���N�V�������C�g�̃f�[�^���쐬����B
	DirectionLight directionLig;
	//���C�g�͎΂ߏォ�炠�����Ă���B
	directionLig.ligDirection.x = 1.0f;
	directionLig.ligDirection.y = 0.0f;
	directionLig.ligDirection.z = 0.0f;
	//���K������B
	directionLig.ligDirection.Normalize();
	//���C�g�̃J���[�͐ԐF�B
	directionLig.ligColor.x = 0.5f;
	directionLig.ligColor.y = 0.0f;
	directionLig.ligColor.z = 0.0f;

	
	ModelInitData m_InitData;
	m_InitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	m_InitData.m_fxFilePath = "Assets/shader/model.fx";
	m_InitData.m_expandConstantBuffer = &directionLig;
	m_InitData.m_expandConstantBufferSize = sizeof(directionLig);

	ModelRender model;
	model.InitRendering(m_InitData);
*/
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

	//	model.Draw(renderContext);

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}

