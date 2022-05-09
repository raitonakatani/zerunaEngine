#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"
 

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
//	g_camera3D->SetPosition({ 0.0f, 100.0f, -100.0f });
//	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });

//	auto game = NewGO<Game>(0);
    auto title = NewGO<Title>(0);
	NewGO<Fade>(0, "fade");

    	RootSignature rs;
	InitRootSignature(rs);

  //  ゲームシーンを描画するレンダリングターゲットを作成
	g_posteffect.Init();
	g_posteffect.SpriteInit();


    g_Light.Init();




//シャドウマップ描画用のレンダリングターゲットを作成する。
//カラーバッファのクリアカラー
//今回はカラーバッファは真っ白にする。
	float clearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    RenderTarget shadowMap;
    shadowMap.Create(
        1024,//【注目】レンダリングターゲットの横幅
        1024,//【注目】レンダリングターゲットの縦幅
        1,
        1,
        DXGI_FORMAT_R32_FLOAT,
        DXGI_FORMAT_D32_FLOAT,
        clearColor
    );

  // 影描画用のライトカメラを作成する。
    Camera lightCamera;
 //   カメラの位置を設定。これはライトの位置。
    lightCamera.SetPosition(0, 600, 0);
  //  カメラの注視点を設定。これがライトが照らしている場所。
    lightCamera.SetTarget(0, 0, 0);
  //  【注目】上方向を設定。今回はライトが真下を向いているので、X方向を上にしている。
    lightCamera.SetUp(1, 0, 0);
  //  今回のサンプルでは画角を狭めにしておく。
    lightCamera.SetViewAngle(Math::DegToRad(20.0f));
 //   ライトビュープロジェクション行列を計算している。
    lightCamera.Update();

 //   シャドウマップ描画用のモデルを用意する。
    ModelInitData teapotShadowModelInitData;
 //   【注目】シャドウマップ描画用のシェーダーを指定する。
    teapotShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
    teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
    Model teapotShadowModel;
    teapotShadowModel.Init(teapotShadowModelInitData);
    teapotShadowModel.UpdateWorldMatrix(
        { 0, 50, 0 },
        g_quatIdentity,
        g_vec3One
    );

 //    通常描画のティーポットモデルを初期化
    ModelRender teapotModel;
 //   teapotModel.Init("Assets/modelData/unityChan.tkm");


  //   シャドウマップを表示するためのスプライトを初期化する
    SpriteInitData spriteInitData;
    spriteInitData.m_textures[0] = &shadowMap.GetRenderTargetTexture();
    spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    spriteInitData.m_width = 256;
    spriteInitData.m_height = 256;

    Sprite sprite;
    sprite.Init(spriteInitData);

    ModelInitData bgModelInitData;
 //   影が落とされるモデル用のシェーダーを指定する。
    bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
 //   シャドウマップを拡張SRVに設定する。
    bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
 //   ライトビュープロジェクション行列を拡張定数バッファに設定する。
    bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
    bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
    bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

    Model bgModel;
    bgModel.Init(bgModelInitData);



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
		g_posteffect.gaussianBlur.ExecuteOnGPU(renderContext, 20);

        //ボケ画像をメインレンダリングターゲットに加算合成。
        //レンダリングターゲットとして利用できるまで待つ。
		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
        //レンダリングターゲットを設定。
		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
        //最終合成。
		g_posteffect.finalSprite.Draw(renderContext);
        //レンダリングターゲットへの書き込み終了待ち。
		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

        //メインレンダリングターゲットの絵をフレームバッファにコピー。
		renderContext.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

    	g_posteffect.copyToFrameBufferSprite.Draw(renderContext);


		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		g_renderEngine.SpriteRenderDraw(renderContext);

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
