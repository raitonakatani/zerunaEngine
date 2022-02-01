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
		//�X�P���g���̃f�[�^��ǂݍ��݁B
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

		//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
		m_InitData.m_tkmFilePath = filePath;

		//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
		m_InitData.m_fxFilePath = "Assets/shader/model.fx";



		m_animationClips = animationClips;
		
		// �X�P���g�����������B
		InitSkeleton(filePath);

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			m_InitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			m_InitData.m_vsSkinEntryPointFunc = "VSSkinMain";

			m_InitData.m_modelUpAxis = enModelUpAxisY;
		}
		else
		{
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			m_InitData.m_vsEntryPointFunc = "VSMain";
		}

		m_InitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
		m_InitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());

		// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);
		//�쐬�����������f�[�^�����ƂɃ��f��������������
		m_model.Init(m_InitData);
	}

	void ModelRender::Update()
	{
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

		if (m_skeleton.IsInited()) {
			//�X�P���g�����X�V�B
			m_skeleton.Update(m_model.GetWorldMatrix());
		}

		//�A�j���[�V������i�߂�B
		m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
	}


	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
	
}
