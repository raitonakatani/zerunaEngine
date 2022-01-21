#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"

namespace nsK2EngineLow {


	ModelRender::ModelRender()
	{
		//シェーダーファイルのファイルパスを指定する。
		m_InitData.m_fxFilePath = "Assets/shader/model.fx";
		//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
		m_InitData.m_vsEntryPointFunc = "VSMain";
		//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
		m_InitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		//スケルトンを指定する。
		m_InitData.m_skeleton = &m_skeleton;
		//モデルの上方向を指定する。
		//3dsMaxではデフォルトZアップになっているが、
		//ユニティちゃんはアニメーションでYアップに変更されている。
		m_InitData.m_modelUpAxis = enModelUpAxisY;
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
		)//int maxInstance)
	{
		// インスタンシング描画用のデータを初期化。
		//InitInstancingDraw(maxInstance);

		//tkmファイルのファイルパスを指定する。
		m_InitData.m_tkmFilePath = filePath;

		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			m_InitData.m_skeleton = &m_skeleton;
		}

		// スケルトンを初期化。
		InitSkeleton(filePath);
		// アニメーションを初期化。
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);


		m_InitData.m_expandConstantBuffer = &g_directionalLight.GetDirectionalLight();
		m_InitData.m_expandConstantBufferSize = sizeof(g_directionalLight.GetDirectionalLight());
		

		//作成した初期化データをもとにモデルを初期化する
		m_model.Init(m_InitData);
	}

	void ModelRender::InitModel(const char* filePath)
	{
		// インスタンシング描画用のデータを初期化。
		//InitInstancingDraw(maxInstance);

		//tkmファイルのファイルパスを指定する。
		m_bgInitData.m_tkmFilePath = filePath;

		// シェーダーファイルのファイルパスを指定する。
		m_bgInitData.m_fxFilePath = "Assets/shader/model.fx";
		// 頂点シェーダーのエントリーポイント
		m_bgInitData.m_vsEntryPointFunc = "VSMain";


		m_bgInitData.m_expandConstantBuffer = &g_directionalLight.GetDirectionalLight();
		m_bgInitData.m_expandConstantBufferSize = sizeof(g_directionalLight.GetDirectionalLight());


		//作成した初期化データをもとにモデルを初期化する
		m_bgmodel.Init(m_bgInitData);
	}

/*	void ModelRender::InitInstancingDraw(int maxInstance)
	{
		m_maxInstance = maxInstance;
		if (m_maxInstance > 1) {
			//インスタンシング描画を行うので、
			//それ用のデータを構築する。
			//ワールド行列の配列のメモリを確保する。
			m_worldMatrixArray = std::make_unique<Matrix[]>(m_maxInstance);
			//ワールド行列をGPUに転送するためのストラクチャードバッファを確保。
			m_worldMatrixArraySB.Init(
				sizeof(Matrix),
				m_maxInstance,
				nullptr
			);
			m_isEnableInstancingDraw = true;
		}
	}
*/

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
