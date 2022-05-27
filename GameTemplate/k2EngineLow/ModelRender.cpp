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

	void ModelRender::InitAnimation(
		AnimationClip* animationClips,
		int numAnimationClips, 
		EnModelUpAxis enModelUpAxis)
	{
		m_animationClips = animationClips;
		m_numAnimationClips = numAnimationClips;
		if (m_animationClips != nullptr) {
			m_animation.Init
			(	m_skeleton,
				m_animationClips,
				m_numAnimationClips);
		}
	}
	void ModelRender::Init(	const char* filePath,
		bool shadowRecieve,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance
		)
	{
		ModelInitData initData;

		//シェーダーファイルのファイルパス。
		initData.m_fxFilePath = "Assets/shader/model.fx";

		//モデルの定数バッファ用の情報をモデルの初期化情報として渡す。
		initData.m_expandConstantBuffer = &g_renderingEngine.GetModelRenderCB();
		initData.m_expandConstantBufferSize = sizeof(g_renderingEngine.GetModelRenderCB());
		if (animationClips == nullptr)
		{
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			initData.m_vsEntryPointFunc = "VSMain";
		}
		else
		{
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			InitSkeleton(filePath);
			//スケルトンを指定する。
			initData.m_skeleton = &m_skeleton;
			InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		}

		if (shadowRecieve) {
			initData.m_psEntryPointFunc = "PSMainShadowReciever";
			m_isShadowCaster = false;
		}
		else
		{
			m_isShadowCaster = true;
		}
		//シャドウマップを拡張SRVに設定する。
		initData.m_expandShaderResoruceView[0] = &g_renderingEngine.GetShadowMap();
		initData.m_tkmFilePath = filePath;

		m_enFbxUpAxis = enModelUpAxis;
		initData.m_modelUpAxis = m_enFbxUpAxis;
		m_model.Init(initData);
		InitShadowModel(filePath, m_enFbxUpAxis);

	}

	void ModelRender::InitModel(const char* filePath)
	{
		// step-1 半透明の球体モデルを初期化
		transModelInitData.m_tkmFilePath = filePath;
		transModelInitData.m_fxFilePath = "Assets/shader/model.fx";
		//半透明モデルはモデルを描くときにライティングを行うので、ライトの情報を渡す。
		transModelInitData.m_expandConstantBuffer = &g_Light.GetLight();
		transModelInitData.m_expandConstantBufferSize = sizeof(g_Light.GetLight());
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
		sphereModel.UpdateWorldMatrix(planePos, g_quatIdentity, g_vec3One);
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
		m_shadowmodel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
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

		g_renderingEngine.AddRenderObject(this);
	}

	void ModelRender::InitShadowModel(const char* tkmFilePath, EnModelUpAxis modelUpAxis)
	{
		ModelInitData ShadowModelInitData;

		// シャドウマップ描画用のシェーダーを指定する
		ShadowModelInitData.m_fxFilePath = "Assets/shader/DrawShadowMap.fx";
		ShadowModelInitData.m_tkmFilePath = tkmFilePath;
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			ShadowModelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			ShadowModelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else
		{
			ShadowModelInitData.m_vsSkinEntryPointFunc = "VSMain";
		}
		ShadowModelInitData.m_colorBufferFormat[0] = DXGI_FORMAT_R32_FLOAT,
			ShadowModelInitData.m_modelUpAxis = modelUpAxis;
			m_shadowmodel.Init(ShadowModelInitData);


	}
	void ModelRender::OnRenderShadowMap(RenderContext& rc, const Matrix& lvpMatrix)
	{
		if (m_isShadowCaster)
		{
			m_shadowmodel.Draw(
				rc,
				g_matIdentity,
				lvpMatrix);

		}
	}
}
