#include "k2EngineLowPreCompile.h"
#include "touka.h"

namespace nsK2EngineLow {
 //   touka g_touka;

	void touka::Init(){
       

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

        spriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
        spriteInitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
        spriteInitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());
        // 初期化データを使ってスプライトを作成
        defferdLightinSpr.Init(spriteInitData);
	}

    void touka::Draw(RenderContext& rc) {
        // レンダリングターゲットをG-Bufferに変更して書き込む
        RenderTarget* rts[] = {
            &albedRT, // 0番目のレンダリングターゲット
            &normalRT, // 1番目のレンダリングターゲット
            &worldPosRT // 2番目のレンダリングターゲット
        };

        // まず、レンダリングターゲットとして設定できるようになるまで待つ
        rc.WaitUntilToPossibleSetRenderTargets(ARRAYSIZE(rts), rts);
        // レンダリングターゲットを設定
        rc.SetRenderTargets(ARRAYSIZE(rts), rts);
        // レンダリングターゲットをクリア
        rc.ClearRenderTargetViews(ARRAYSIZE(rts), rts);


        //humanModel.Draw(rc);
        //bgModel.Draw(rc);


        // レンダリングターゲットへの書き込み待ち
        rc.WaitUntilFinishDrawingToRenderTargets(ARRAYSIZE(rts), rts);

        // レンダリング先をフレームバッファーに戻してスプライトをレンダリングする
        g_graphicsEngine->ChangeRenderTargetToFrameBuffer(rc);
        // G-Bufferの内容を元にしてディファードライティング
        defferdLightinSpr.Draw(rc);

        // step-2 深度ステンシルビューをG-Bufferを作成したときのものに変更する
        //深度ステンシルビューはG-Bufferを作成したときのものに変更する。
        rc.SetRenderTarget(
            g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
            rts[0]->GetDSVCpuDescriptorHandle()
        );
    }



}