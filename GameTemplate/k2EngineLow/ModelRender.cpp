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
	void ModelRender::Init(
		const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis
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

		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			m_InitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			m_InitData.m_vsSkinEntryPointFunc = "VSSkinMain";

			m_InitData.m_modelUpAxis = enModelUpAxisY;
		}
		else
		{
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			m_InitData.m_vsEntryPointFunc = "VSMain";
		}

		m_InitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
		m_InitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());

		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		//作成した初期化データをもとにモデルを初期化する
		m_model.Init(m_InitData);
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
	}
	
}
