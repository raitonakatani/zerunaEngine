#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "Title.h"
#include "Fade.h"

/// <summary>
/// ディレクショナルライト
/// </summary>
struct DirectionalLight
{
	Vector3  color;
	float pad0;         // パディング
	Vector3  direction;
	float pad1;         // パディング
	Vector3 eyePos; // 視点
	float specPow;  // スペキュラの絞り
};

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
	g_camera3D->SetPosition({ 0.0f, 100.0f, -100.0f });
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });

//	auto game = NewGO<Game>(0);
auto title = NewGO<Title>(0);
	NewGO<Fade>(0, "fade");

	RootSignature rs;
	InitRootSignature(rs);

	//ゲームシーンを描画するレンダリングターゲットを作成
	g_posteffect.Init();
	g_posteffect.SpriteInit();


	g_Light.Init();




	//シャドウマップ描画用のレンダリングターゲットを作成する。
	//カラーバッファのクリアカラー
	//今回はカラーバッファは真っ白にする。
/*	float clearColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
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

	//影描画用のライトカメラを作成する。
	Camera lightCamera;
	//カメラの位置を設定。これはライトの位置。
	lightCamera.SetPosition(0, 600, 0);
	//カメラの注視点を設定。これがライトが照らしている場所。
	lightCamera.SetTarget(0, 0, 0);
	//【注目】上方向を設定。今回はライトが真下を向いているので、X方向を上にしている。
	lightCamera.SetUp(1, 0, 0);
	//今回のサンプルでは画角を狭めにしておく。
	lightCamera.SetViewAngle(Math::DegToRad(20.0f));
	//ライトビュープロジェクション行列を計算している。
	lightCamera.Update();

	//シャドウマップ描画用のモデルを用意する。
	ModelInitData teapotShadowModelInitData;
	//【注目】シャドウマップ描画用のシェーダーを指定する。
	teapotShadowModelInitData.m_fxFilePath = "Assets/shader/sampleDrawShadowMap.fx";
	teapotShadowModelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	Model teapotShadowModel;
	teapotShadowModel.Init(teapotShadowModelInitData);
	teapotShadowModel.UpdateWorldMatrix(
		{ 0, 50, 0 },
		g_quatIdentity,
		g_vec3One
	);

	// 通常描画のティーポットモデルを初期化
	ModelRender teapotModel;
	teapotModel.Init("Assets/modelData/unityChan.tkm");
	

	// シャドウマップを表示するためのスプライトを初期化する
	SpriteInitData spriteInitData;
	spriteInitData.m_textures[0] = &shadowMap.GetRenderTargetTexture();
	spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	spriteInitData.m_width = 256;
	spriteInitData.m_height = 256;

	Sprite sprite;
	sprite.Init(spriteInitData);

	ModelInitData bgModelInitData;
	//影が落とされるモデル用のシェーダーを指定する。
	bgModelInitData.m_fxFilePath = "Assets/shader/sampleShadowReciever.fx";
	//シャドウマップを拡張SRVに設定する。
	bgModelInitData.m_expandShaderResoruceView[0] = &shadowMap.GetRenderTargetTexture();
	//ライトビュープロジェクション行列を拡張定数バッファに設定する。
	bgModelInitData.m_expandConstantBuffer = (void*)&lightCamera.GetViewProjectionMatrix();
	bgModelInitData.m_expandConstantBufferSize = sizeof(lightCamera.GetViewProjectionMatrix());
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";

	Model bgModel;
	bgModel.Init(bgModelInitData);

*/

	//ステックの入力量を取得
	float lStick_x = 0.0f;
	float lStick_y = 0.0f;



	// ルートシグネチャを作成
	RootSignature rootSignature;
	InitRootSignature(rootSignature);


	// ディレクションライト
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


	// モデルを初期化
	ModelInitData modelInitData;
	// 人型モデルを初期化
	modelInitData.m_tkmFilePath = "Assets/modelData/player/player2.tkm";
	modelInitData.m_fxFilePath = "Assets/shader/model1.fx";
	Model humanModel;
	humanModel.Init(modelInitData);
	humanModel.UpdateWorldMatrix({ 0.0f, 0.0f, 0.0f }, g_quatIdentity, g_vec3One);

	// 背景モデルを初期化
	modelInitData.m_tkmFilePath = "Assets/modelData/karisute/stage3.tkm";
	Model bgModel;
	bgModel.Init(modelInitData);


	RenderTarget albedRT;
	RenderTarget normalRT;
	RenderTarget worldPosRT;
	SpriteInitData spriteInitData;
	Sprite defferdLightinSpr;

	// G-Bufferを作成
	// アルベドカラー書き込み用のレンダリングターゲット
	albedRT.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
	// 法線書き込み用のレンダリングターゲット
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
		DXGI_FORMAT_R32G32B32A32_FLOAT, // ワールド座標を記録するので、32ビット浮動小数点バッファーを利用する
		DXGI_FORMAT_UNKNOWN
	);

	// ポストエフェクト的にディファードライティングを行うためのスプライトを初期化
	// 画面全体にレンダリングするので幅と高さはフレームバッファーの幅と高さと同じ
	spriteInitData.m_width = FRAME_BUFFER_W;
	spriteInitData.m_height = FRAME_BUFFER_H;
	// ディファードライティングで使用するテクスチャを設定
	spriteInitData.m_textures[0] = &albedRT.GetRenderTargetTexture();
	spriteInitData.m_textures[1] = &normalRT.GetRenderTargetTexture();

	// ディファードライティングで使用するテクスチャにワールド座標テクスチャを追加
	spriteInitData.m_textures[2] = &worldPosRT.GetRenderTargetTexture();

	spriteInitData.m_fxFilePath = "Assets/shader/sprite1.fx";
	spriteInitData.m_expandConstantBuffer = &light;
	spriteInitData.m_expandConstantBufferSize = sizeof(light);
	// 初期化データを使ってスプライトを作成
	defferdLightinSpr.Init(spriteInitData);



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

		g_Light.Update();


	/*	if (g_pad[0]->IsPress(enButtonA))
		{
			model.modelUpdate();

			// レンダリングターゲットをG-Bufferに変更して書き込む
			RenderTarget* rts[] = {
				&albedRT, // 0番目のレンダリングターゲット
				&normalRT, // 1番目のレンダリングターゲット
				&worldPosRT // 2番目のレンダリングターゲット
			};

			// まず、レンダリングターゲットとして設定できるようになるまで待つ
			renderContext.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
			// レンダリングターゲットを設定
			renderContext.SetRenderTargets(ARRAYSIZE(rts), rts);
			// レンダリングターゲットをクリア
			renderContext.ClearRenderTargetViews(ARRAYSIZE(rts), rts);


			humanModel.Draw(renderContext);
			bgModel.Draw(renderContext);


			// レンダリングターゲットへの書き込み待ち
			renderContext.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

			// レンダリング先をフレームバッファーに戻してスプライトをレンダリングする
			g_graphicsEngine->ChangeRenderTargetToFrameBuffer(renderContext);
			// G-Bufferの内容を元にしてディファードライティング
			defferdLightinSpr.Draw(renderContext);

			// step-2 深度ステンシルビューをG-Bufferを作成したときのものに変更する
			//深度ステンシルビューはG-Bufferを作成したときのものに変更する。
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
		// 影を生成したいモデルをシャドウマップに描画する。
		//レンダリングターゲットをシャドウマップに変更する。
	//	renderContext.WaitUntilToPossibleSetRenderTarget(shadowMap);
	//	renderContext.SetRenderTargetAndViewport(shadowMap);
	//	renderContext.ClearRenderTargetView(shadowMap);

		//影モデルを描画。
	//	teapotShadowModel.Draw(renderContext, lightCamera);

		//書き込み完了待ち。
	//	renderContext.WaitUntilFinishDrawingToRenderTarget(shadowMap);


		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
	//	g_k2EngineLow->ExecuteRender();

		//レンダリングターゲットをmainRenderTargetに変更する。
		//レンダリングターゲットとして利用できるまで待つ。
//		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//レンダリングターゲットを設定。
//		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//レンダリングターゲットをクリア。
//		renderContext.ClearRenderTargetView(g_posteffect.mainRenderTarget);

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
	//	g_k2EngineLow->ExecuteRender();

		//mainRenderTargetに各種モデルを描画する。
		//plModel.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
//		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//輝度抽出
		//輝度抽出用のレンダリングターゲットに変更。
//		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.luminnceRenderTarget);
		//レンダリングターゲットを設定。
//		renderContext.SetRenderTargetAndViewport(g_posteffect.luminnceRenderTarget);
		//レンダリングターゲットをクリア。
//		renderContext.ClearRenderTargetView(g_posteffect.luminnceRenderTarget);
		//輝度抽出を行う。
//		g_posteffect.luminanceSprite.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
//		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.luminnceRenderTarget);

		//ガウシアンブラーを実行する。
///		g_posteffect.gaussianBlur.ExecuteOnGPU(renderContext, 20);

		//ボケ画像をメインレンダリングターゲットに加算合成。
		//レンダリングターゲットとして利用できるまで待つ。
//		renderContext.WaitUntilToPossibleSetRenderTarget(g_posteffect.mainRenderTarget);
		//レンダリングターゲットを設定。
//		renderContext.SetRenderTargetAndViewport(g_posteffect.mainRenderTarget);
		//最終合成。
//		g_posteffect.finalSprite.Draw(renderContext);
		//レンダリングターゲットへの書き込み終了待ち。
//		renderContext.WaitUntilFinishDrawingToRenderTarget(g_posteffect.mainRenderTarget);

		//メインレンダリングターゲットの絵をフレームバッファにコピー。
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
