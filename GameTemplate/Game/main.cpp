#include "stdafx.h"
#include "system/system.h"
#include "Game.h"



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
	g_camera3D->SetPosition({ 0.0f, 100.0f, 300.0f });
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });

	

	auto game = NewGO<Game>(0);

	//float4 a = { 1.0f,1.0f, 1.0f, 1.0f, };

	float a[4] = { 1.0f,1.0f,1.0f,0.0f };

	//RenderTargetクラスのオブジェクトを定義。
	RenderTarget offscreenRenderTarget;
	//RenderTarget::Create()を利用して、レンダリングターゲットを作成する。
	offscreenRenderTarget.Create(
		1600,						//テクスチャの幅。
		900,						//テクスチャの高さ。
		1,							//Mipmapレベル。
		1,							//テクスチャ配列のサイズ。
		DXGI_FORMAT_R8G8B8A8_UNORM, //カラーバッファのフォーマット。
		DXGI_FORMAT_D32_FLOAT		//デプスステンシルバッファのフォーマット。
		//a
	);

			ModelRender m;

		// 各種モデルを初期化する
		// 背景モデルを初期化
		ModelInitData bgModelInitData;
		bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
		bgModelInitData.m_fxFilePath = "Assets/shader/3D.fx";

		// 背景モデルを初期化
		Model bgModel;
		bgModel.Init(bgModelInitData);

		// プレイヤーモデルを初期化
		ModelInitData plModelInitData;
		plModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
		plModelInitData.m_fxFilePath = "Assets/shader/3D.fx";
		Model plModel;
		plModel.Init(plModelInitData);


	//スプライトの初期化オブジェクトを作成する。
	SpriteInitData spriteInitData;
	//【重要！！！】テクスチャはオフスクリーンレンダリングされた絵。
	spriteInitData.m_textures[0] = &offscreenRenderTarget.GetRenderTargetTexture();
	//【重要！！！】全画面描画なのでスプライトのサイズはフレームバッファと同じにする。
	spriteInitData.m_width = 1600;
	spriteInitData.m_height = 900;
	//【重要！！！】モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
	//初期化オブジェクトを使って、スプライトを初期化する。
	Sprite monochromeSprite;
	monochromeSprite.Init(spriteInitData);

	g_Light.Init();

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


		//レンダリングターゲットをoffscreenRenderTargetに変更する。
		RenderTarget* rtArray[] = { &offscreenRenderTarget };
		//レンダリングターゲットとして利用できるまで待つ。
		renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargets(1, rtArray);
		//レンダリングターゲットをクリア。
		renderContext.ClearRenderTargetViews(1, rtArray);

		//offscreenRenderTargetに背景、プレイヤーを描画する。
		//背景モデルをドロー。		
		bgModel.Draw(renderContext);
		//プレイヤーをドロー。
		plModel.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTargets(1, rtArray);

		//画面に表示されるレンダリングターゲットに戻す。
			renderContext.SetRenderTarget(
				g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
				g_graphicsEngine->GetCurrentFrameBuffuerDSV()
			);

		monochromeSprite.Draw(renderContext);

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		g_Light.Update();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
