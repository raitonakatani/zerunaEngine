#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

	PostEffect g_postEffect;

	void PostEffect::Init()
	{
		
		//step-1 オフスクリーン描画用のレンダリングターゲットを作成。
		//RenderTargetクラスのオブジェクトを定義。
		
		//RenderTarget::Create()を利用して、レンダリングターゲットを作成する。
		mainRenderTarget.Create(
			1600,												//テクスチャの幅。
			900,												//テクスチャの高さ。
			1,													//Mipmapレベル。
			1,													//テクスチャ配列のサイズ。
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT				//デプスステンシルバッファのフォーマット。
		);

		//解像度、ミップマップレベル
		luminnceRenderTarget.Create(
			1600,		//解像度はメインレンダリングターゲットと同じ。
			900,		//解像度はメインレンダリングターゲットと同じ。
			1,
			1,
			//【注目】カラーバッファのフォーマットを32bit浮動小数点にしている。
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

	}

	void PostEffect::Render(RenderContext& rc)
	{
		// step-3 レンダリングターゲットを変更する
		

		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(g_postEffect.mainRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTarget(g_postEffect.mainRenderTarget);

		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(g_postEffect.mainRenderTarget);

		g_engine->ExecuteRender();

		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(g_postEffect.mainRenderTarget);

		

		// レンダリングターゲットとして利用できるまで待つ
		rc.WaitUntilToPossibleSetRenderTarget(g_postEffect.luminnceRenderTarget);

		// レンダリングターゲットを設定
		rc.SetRenderTarget(g_postEffect.luminnceRenderTarget);
		// レンダリングターゲットをクリア
		rc.ClearRenderTargetView(g_postEffect.luminnceRenderTarget);

		g_bloom.LuminanceSpriteDraw(rc);

		// レンダリングターゲットへの書き込み終了待ち
		rc.WaitUntilFinishDrawingToRenderTarget(g_postEffect.luminnceRenderTarget);

		g_bloom.Blur(rc);

		g_bloom.Render(rc, mainRenderTarget);
		// step-5 画面に表示されるレンダリングターゲットに戻す
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		g_bloom.Draw(rc);
	}
}