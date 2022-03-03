#include "stdafx.h"
#include "system/system.h"
#include "Game.h"

const int NUM_WEIGHTS = 8;
/// <summary>
/// ブラー用のパラメーター
/// </summary>
struct SBlurParam
{
	float weights[NUM_WEIGHTS];
};

// 関数宣言
void InitRootSignature(RootSignature& rs);


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

	RootSignature rs;
	InitRootSignature(rs);

	//ゲームシーンを描画するレンダリングターゲットを作成
	g_posteffect.Init();
	g_posteffect.SpriteInit();

	// step-5 ガウシアンブラーを初期化
	GaussianBlur gaussianBlur;
	gaussianBlur.Init(&g_posteffect.luminnceRenderTarget.GetRenderTargetTexture());
	// step-6 ボケ画像を加算合成するスプライトを初期化
	//初期化情報を設定する。
	SpriteInitData finalSpriteInitData;
	finalSpriteInitData.m_textures[0] = &gaussianBlur.GetBokeTexture();
	//解像度はmainRenderTargetの幅と高さ。
	finalSpriteInitData.m_width = 1600;
	finalSpriteInitData.m_height = 900;
	//ぼかした画像を、通常の2Dとしてメインレンダリングターゲットに描画するので、
	//2D用のシェーダーを使用する。
	finalSpriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
	//カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
	finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

	//初期化情報を元に加算合成用のスプライトを初期化する。
	Sprite finalSprite;
	finalSprite.Init(finalSpriteInitData);
	// step-7 テクスチャを貼り付けるためのスプライトを初期化する
	//スプライトの初期化オブジェクトを作成する。
	SpriteInitData spriteInitData;
	//テクスチャはmainRenderTargetのカラーバッファ。
	spriteInitData.m_textures[0] = &g_posteffect.mainRenderTarget.GetRenderTargetTexture();
	spriteInitData.m_width = 1600;
	spriteInitData.m_height = 900;
	//モノクロ用のシェーダーを指定する。
	spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
	//初期化オブジェクトを使って、スプライトを初期化する。
	Sprite copyToFrameBufferSprite;
	copyToFrameBufferSprite.Init(spriteInitData);

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

		//レンダリングターゲットをmainRenderTargetに変更する。
		//レンダリングターゲットとして利用できるまで待つ。
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//レンダリングターゲットをクリア。
		renderContext.ClearRenderTargetView(g_posteffect.mainRenderTarget);

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		//mainRenderTargetに各種モデルを描画する。
		//plModel.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//輝度抽出
		//輝度抽出用のレンダリングターゲットに変更。
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.luminnceRenderTarget);
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(g_posteffect.luminnceRenderTarget);
		//レンダリングターゲットをクリア。
		renderContext.ClearRenderTargetView(g_posteffect.luminnceRenderTarget);
		//輝度抽出を行う。
		g_posteffect.luminanceSprite.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.luminnceRenderTarget);

		//ガウシアンブラーを実行する。
		gaussianBlur.ExecuteOnGPU(renderContext, 20);

		//ボケ画像をメインレンダリングターゲットに加算合成。
		//レンダリングターゲットとして利用できるまで待つ。
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//最終合成。
		finalSprite.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//メインレンダリングターゲットの絵をフレームバッファにコピー。
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);


		if (g_pad[0]->IsPress(enButtonA))
		{
		//	copyToFrameBufferSprite.Draw(renderContext);
		}

		g_renderingEngine.SpriteRenderDraw(renderContext);

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		g_Light.Update();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}

// ルートシグネチャの初期化
void InitRootSignature(RootSignature& rs)
{
	rs.Init(D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);
}
