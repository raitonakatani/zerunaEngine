#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

/*
struct DirectionLight {
	Vector3 ligDirection;	//ライトの方向。
	float pad;
	Vector3 ligColor;
};
*/

// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 100.0f, -200.0f });
	g_camera3D->SetTarget({ 0.0f, 50.0f, 0.0f });

	auto game = NewGO<Game>(0);

/*	//step-2 ディレクションライトのデータを作成する。
	DirectionLight directionLig;
	//ライトは斜め上からあたっている。
	directionLig.ligDirection.x = 1.0f;
	directionLig.ligDirection.y = 0.0f;
	directionLig.ligDirection.z = 0.0f;
	//正規化する。
	directionLig.ligDirection.Normalize();
	//ライトのカラーは赤色。
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
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

	//	model.Draw(renderContext);

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}

