#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {
    PostEffect g_posteffect;

    auto& renderContext = g_graphicsEngine->GetRenderContext();

    void PostEffect::Init()
    {
        // step-1 メインレンダリングターゲットを作成する
 //       RenderTarget mainRenderTarget;
        mainRenderTarget.Create(
            1600,
            900,
            1,
            1,
            //【注目】カラーバッファのフォーマットを32bit浮動小数点にしている。
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            DXGI_FORMAT_D32_FLOAT
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

    void PostEffect::SpriteInit()
    {
        //輝度抽出用のスプライトを初期化
        //初期化情報を作成する。
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
        luminanceSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
        //描き込むレンダリングターゲットのフォーマットを指定する。
        luminanceSpriteInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32G32B32A32_FLOAT;
    
        luminanceSprite.Init(luminanceSpriteInitData);
   
        //作成した初期化情報をもとにスプライトを初期化する。
       
        luminanceSprite.Init(luminanceSpriteInitData);
        // step-5 ガウシアンブラーを初期化
        gaussianBlur.Init(&luminnceRenderTarget.GetRenderTargetTexture());

        // step-6 ボケ画像を加算合成するスプライトを初期化
        //初期化情報を設定する。
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
        finalSprite.Init(finalSpriteInitData);
        // step-7 テクスチャを貼り付けるためのスプライトを初期化する
        //スプライトの初期化オブジェクトを作成する。
        //テクスチャはmainRenderTargetのカラーバッファ。
        spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
        spriteInitData.m_width = 1600;
        spriteInitData.m_height = 900;
        //モノクロ用のシェーダーを指定する。
        spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";
        //初期化オブジェクトを使って、スプライトを初期化する。
        copyToFrameBufferSprite.Init(spriteInitData);
    }
    
    void PostEffect::maint()
    {

        //レンダリングターゲットをmainRenderTargetに変更する。
          //レンダリングターゲットとして利用できるまで待つ。
        renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        //レンダリングターゲットを設定。
        renderContext.SetRenderTargetAndViewport(mainRenderTarget);
        //レンダリングターゲットをクリア。
        renderContext.ClearRenderTargetView(mainRenderTarget);
    }

    void PostEffect::Render()
    {

        //mainRenderTargetに各種モデルを描画する。
        //plModel.Draw(renderContext);
        //レンダリングターゲットへの書き込み終了待ち。
        renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

        //輝度抽出
        //輝度抽出用のレンダリングターゲットに変更。
        renderContext.WaitUntilToPossibleSetRenderTarget(luminnceRenderTarget);
        //レンダリングターゲットを設定。
        renderContext.SetRenderTargetAndViewport(luminnceRenderTarget);
        //レンダリングターゲットをクリア。
        renderContext.ClearRenderTargetView(luminnceRenderTarget);
        //輝度抽出を行う。
        luminanceSprite.Draw(renderContext);
        //レンダリングターゲットへの書き込み終了待ち。
        renderContext.WaitUntilFinishDrawingToRenderTarget(luminnceRenderTarget);

        //ガウシアンブラーを実行する。
        gaussianBlur.ExecuteOnGPU(renderContext, 20);

        //ボケ画像をメインレンダリングターゲットに加算合成。
        //レンダリングターゲットとして利用できるまで待つ。
        renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
        //レンダリングターゲットを設定。
        renderContext.SetRenderTargetAndViewport(mainRenderTarget);
        //最終合成。
        finalSprite.Draw(renderContext);
        //レンダリングターゲットへの書き込み終了待ち。
        renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

        //メインレンダリングターゲットの絵をフレームバッファにコピー。
        renderContext.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            g_graphicsEngine->GetCurrentFrameBuffuerDSV()
        );


        if (g_pad[0]->IsPress(enButtonA))
        {
            	copyToFrameBufferSprite.Draw(renderContext);
        }
    }
}