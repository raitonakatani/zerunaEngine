#include "k2EngineLowPreCompile.h"
#include "Bloom.h"

namespace nsK2EngineLow {

	Bloom g_bloom;



	void Bloom::Init()
	{
		InitLuminanceSprite();
		InitBlur();
		InitFinalSprite();
		InitSprite();

	}
	void Bloom::InitSprite()
	{
		//スプライトの初期化オブジェクトを作成する。
		SpriteInitData spriteInitData;
		//テクスチャはmainRenderTargetのカラーバッファ。
		spriteInitData.m_textures[0] = &g_postEffect.mainRenderTarget.GetRenderTargetTexture();
		spriteInitData.m_width = 1600;
		spriteInitData.m_height = 900;
		//モノクロ用のシェーダーを指定する。
		spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		//初期化オブジェクトを使って、スプライトを初期化する。
		
		copyToFrameBufferSprite.Init(spriteInitData);
	}

	void Bloom::Draw(RenderContext& rc)
	{
		copyToFrameBufferSprite.Draw(rc);
	}
	void Bloom::InitLuminanceSprite()
	{
		//step-4 輝度抽出用のスプライトを初期化。
		//初期化情報を作成する。
		SpriteInitData luminanceSpriteInitData;
		//輝度抽出用のシェーダーのファイルパスを指定する。
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
		//頂点シェーダーのエントリーポイントを指定する。
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		//ピクセルシェーダーのエントリーポイントを指定する。
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		//スプライトの幅と高さはluminnceRenderTargetと同じ。
		luminanceSpriteInitData.m_width = 1600;
		luminanceSpriteInitData.m_height = 900;
		//テクスチャはメインレンダリングターゲットのカラーバッファ。
		luminanceSpriteInitData.m_textures[0] = &g_postEffect.mainRenderTarget.GetRenderTargetTexture();
		//描き込むレンダリングターゲットのフォーマットを指定する。
		luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//作成した初期化情報をもとにスプライトを初期化する。
		
		luminanceSprite.Init(luminanceSpriteInitData);
	}

	void Bloom::InitBlur()
	{
		//step-1 ガウシアンブラーを初期化。
		
		//gaussianBlur[0]は輝度テクスチャにガウシアンブラーをかける。
		gaussianBlur[0].Init(&g_postEffect.luminnceRenderTarget.GetRenderTargetTexture());
		//gaussianBlur[1]はgaussianBlur[0]のテクスチャにガウシアンブラーをかける。
		gaussianBlur[1].Init(&gaussianBlur[0].GetBokeTexture());
		//gaussianBlur[2]はgaussianBlur[1]のテクスチャにガウシアンブラーをかける。
		gaussianBlur[2].Init(&gaussianBlur[1].GetBokeTexture());
		//gaussianBlur[3]はgaussianBlur[2]のテクスチャにガウシアンブラーをかける。
		gaussianBlur[3].Init(&gaussianBlur[2].GetBokeTexture());
	}
	void Bloom::InitFinalSprite()
	{
		//step-2 ボケ画像を合成して書き込むためのスプライトを初期化。
		//初期化情報を設定する。
		SpriteInitData finalSpriteInitData;
		//【注目】ボケテクスチャを4枚指定する。
		finalSpriteInitData.m_textures[0] = &gaussianBlur[0].GetBokeTexture();
		finalSpriteInitData.m_textures[1] = &gaussianBlur[1].GetBokeTexture();
		finalSpriteInitData.m_textures[2] = &gaussianBlur[2].GetBokeTexture();
		finalSpriteInitData.m_textures[3] = &gaussianBlur[3].GetBokeTexture();
		//解像度はmainRenderTargetの幅と高さ。
		finalSpriteInitData.m_width = 1600;
		finalSpriteInitData.m_height = 900;
		//【注目】ボケ画像を合成する必要があるので、2D用のシェーダーではなく、専用シェーダーを指定。
		finalSpriteInitData.m_fxFilePath = "Assets/shader/PostEffect.fx";
		finalSpriteInitData.m_psEntryPoinFunc = "PSBloomFinal";

		//ただし、加算合成で描画するので、アルファブレンディングモードを加算にする。
		finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		//カラーバッファのフォーマットは例によって、32ビット浮動小数点バッファ。
		finalSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//初期化情報を元に加算合成用のスプライトを初期化する。

		finalSprite.Init(finalSpriteInitData);
	}

	void Bloom::Blur(RenderContext& rc)
	{
		gaussianBlur[0].ExecuteOnGPU(rc, 10);
		gaussianBlur[1].ExecuteOnGPU(rc, 10);
		gaussianBlur[2].ExecuteOnGPU(rc, 10);
		gaussianBlur[3].ExecuteOnGPU(rc, 10);
	}
	void Bloom::Render(RenderContext& rc,RenderTarget& rt)
	{
		//レンダリングターゲットとして利用できるまで待つ。
		rc.WaitUntilToPossibleSetRenderTarget(rt);
		//レンダリングターゲットを設定。
		rc.SetRenderTargetAndViewport(rt);
		//最終合成。
		finalSprite.Draw(rc);
		//レンダリングターゲットへの書き込み終了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(rt);
	}
}