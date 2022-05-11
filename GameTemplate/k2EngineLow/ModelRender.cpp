#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {


	ModelRender::ModelRender()
	{
	}

	ModelRender::~ModelRender()
	{
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		//スケルトンのデータを読み込み。
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips, EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) {
			m_animation.Init(m_skeleton,
				m_animationClips,
				numAnimationClips);
		}
	}
	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance
	)
	{
		ModelInitData m_InitData;

		//tkmファイルのファイルパスを指定する。
		m_InitData.m_tkmFilePath = filePath;


		//シェーダーファイルのファイルパスを指定する。
		m_InitData.m_fxFilePath = "Assets/shader/model.fx";

		m_animationClips = animationClips;

		// スケルトンを初期化。
		InitSkeleton(filePath);

		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			m_InitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			m_InitData.m_vsSkinEntryPointFunc = "VSSkinMain";

			m_InitData.m_modelUpAxis = enModelUpAxis;
		}
		else
		{
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			m_InitData.m_vsEntryPointFunc = "VSMain";
		}

		m_InitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
		m_InitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());

		//作成した初期化データをもとにモデルを初期化する
		m_model.Init(m_InitData);
	}

	void ModelRender::InitModel(const char* filePath)
	{
		// step-1 半透明の球体モデルを初期化
		transModelInitData.m_tkmFilePath = filePath;
		transModelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//半透明モデルはモデルを描くときにライティングを行うので、ライトの情報を渡す。
		transModelInitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
		transModelInitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());
		//ピクセルシェーダのエントリーポイントが不透明モデルとは異なる。
		//不透明モデルはPSMain、半透明モデルはPSMainTransを使用する。
		//ピクセルシェーダの実装は後で確認。
//		transModelInitData.m_psEntryPointFunc = "PSMainTrans";

		transModelInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
		//半透明の球体モデルを初期化。
		sphereModel.Init(transModelInitData);
	}

	void ModelRender::modelUpdate()
	{
		if (g_pad[0]->IsPress(enButtonRight))
		{
			planePos.x -= 1.0f;
		}
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			planePos.x += 1.0f;
		}
		if (g_pad[0]->IsPress(enButtonUp))
		{
			planePos.z -= 1.0f;
		}
		if (g_pad[0]->IsPress(enButtonDown))
		{
			planePos.z += 1.0f;
		}
		sphereModel.UpdateWorldMatrix(planePos, g_quatIdentity, g_vec3One);
	}

	void ModelRender::touka()
	{
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



	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "インスタンスの数が多すぎです。");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);

		// インスタンシング描画を行う。
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//インスタンス数が0の場合のみアニメーション関係の更新を行う。
			// スケルトンを更新。
			// 各インスタンスのワールド空間への変換は、
			// インスタンスごとに行う必要があるので、頂点シェーダーで行う。
			// なので、単位行列を渡して、モデル空間でボーン行列を構築する。
			m_skeleton.Update(g_matIdentity);
			//アニメーションを進める。
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
	}

	void ModelRender::Update()
	{
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_skeleton.IsInited()) {
			//スケルトンを更新。
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//アニメーションを進める。
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}


	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
		//半透明オブジェクトを描画！
		sphereModel.Draw(rc);
	}

	void ModelRender::Drawtwo(RenderContext& rc)
	{
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
