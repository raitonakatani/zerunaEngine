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
	void ModelRender::Init(	const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReciever,
		int maxInstance
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

				// �A�j���[�V�������������B
		InitAnimation(animationClips, numAnimationClips, enModelUpAxis);

		if (m_animationClips != nullptr) {
			//�X�P���g�����w�肷��B
			m_InitData.m_skeleton = &m_skeleton;
			//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
			m_InitData.m_vsSkinEntryPointFunc = "VSSkinMain";

			m_InitData.m_modelUpAxis = enModelUpAxis;
		}
		else
		{
			//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
			m_InitData.m_vsEntryPointFunc = "VSMain";
		}

		m_InitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
		m_InitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());

		//m_InitData.m_alphaBlendMode = AlphaBlendMode_Trans;

		//�쐬�����������f�[�^�����ƂɃ��f��������������
		m_model.Init(m_InitData);
	}

	void ModelRender::UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
	{
		K2_ASSERT(m_numInstance < m_maxInstance, "�C���X�^���X�̐����������ł��B");
		if (!m_isEnableInstancingDraw) {
			return;
		}
		auto wlorldMatrix = m_zprepassModel.CalcWorldMatrix(pos, rot, scale);

		// �C���X�^���V���O�`����s���B
		m_worldMatrixArray[m_numInstance] = wlorldMatrix;
		if (m_numInstance == 0) {
			//�C���X�^���X����0�̏ꍇ�̂݃A�j���[�V�����֌W�̍X�V���s���B
			// �X�P���g�����X�V�B
			// �e�C���X�^���X�̃��[���h��Ԃւ̕ϊ��́A
			// �C���X�^���X���Ƃɍs���K�v������̂ŁA���_�V�F�[�_�[�ōs���B
			// �Ȃ̂ŁA�P�ʍs���n���āA���f����ԂŃ{�[���s����\�z����B
			m_skeleton.Update(g_matIdentity);
			//�A�j���[�V������i�߂�B
			m_animation.Progress(g_gameTime->GetFrameDeltaTime() * m_animationSpeed);
		}
		m_numInstance++;
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

	void ModelRender::InitModel(const char* filePath)
	{
		// step-1 �������̋��̃��f����������
		transModelInitData.m_tkmFilePath = filePath;
		transModelInitData.m_fxFilePath = "Assets/shader/model1.fx";
		//���������f���̓��f����`���Ƃ��Ƀ��C�e�B���O���s���̂ŁA���C�g�̏���n���B
		transModelInitData.m_expandConstantBuffer = &g_Light.GetDirectionalLight();
		transModelInitData.m_expandConstantBufferSize = sizeof(g_Light.GetDirectionalLight());
		//�s�N�Z���V�F�[�_�̃G���g���[�|�C���g���s�������f���Ƃ͈قȂ�B
		//�s�������f����PSMain�A���������f����PSMainTrans���g�p����B
		//�s�N�Z���V�F�[�_�̎����͌�Ŋm�F�B
//		transModelInitData.m_psEntryPointFunc = "PSMainTrans";

		transModelInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
		//�������̋��̃��f�����������B
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

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
	
}
