#pragma once

namespace nsK2EngineLow {

	class ModelRender
	{
	public:

		ModelRender();
		~ModelRender();

		void Init(
			const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);

		

		void InitModel(const char* filePath);

	//	void InitRendering(ModelInitData& initData);

	//	void InitInstancingDraw(int maxInstance);

		void Draw(RenderContext& rc);

		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <remark>
		/// �C���X�^���V���O�`�悪�L���̏ꍇ�́A���̐ݒ�͖�������܂��B
		/// </remark>
		/// <param name="pos">���W�B</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// ���W��ݒ�B
		/// </summary>
		/// <param name="x">x���W</param>
		/// <param name="y">y���W</param>
		/// <param name="z">z���W</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// ��]��ݒ�B
		/// </summary>
		/// <remark>
		/// �C���X�^���V���O�`�悪�L���̏ꍇ�́A���̐ݒ�͖�������܂��B
		/// </remark>
		/// <param name="rotation">��]�B</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// �g�嗦��ݒ�B
		/// </summary>
		/// <remark>
		/// �C���X�^���V���O�`�悪�L���̏ꍇ�́A���̐ݒ�͖�������܂��B
		/// </remark>
		/// <param name="scale">�g�嗦�B</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}

		void Update();

	
		/// <summary>
		/// �A�j���[�V�����̏������B
		/// </summary>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�B</param>
		/// <param name="numAnimationClips">�A�j���[�V�����N���b�v�̐��B</param>
		/// <param name="enModelUpAxis">���f���̏�����B</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);
		/// <summary>
		/// �A�j���[�V�����Đ��B
		/// </summary>
		/// <param name="animNo">�A�j���[�V�����N���b�v�̔ԍ��B</param>
		/// <param name="interpolateTime">�⊮����(�P�ʁF�b�B)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// ���f�����擾�B
		/// </summary>
		/// <returns>���f��</returns>
		Model& GetModel()
		{
			return m_model;
		}
		/// <summary>
		/// �C���X�^���V���O�`����s���H
		/// </summary>
		/// <returns></returns>
		bool IsInstancingDraw() const
		{
			return m_isEnableInstancingDraw;
		}
		/// <summary>
		/// ���[���h�s����擾�B
		/// </summary>
		/// <param name="instanceId">
		/// �C���X�^���XID�B
		/// ���̈����̓C���X�^���V���O�`�悪�������̏ꍇ�͖�������܂��B
		/// </param>
		/// <returns></returns>
		const Matrix& GetWorldMatrix(int instanceId) const
		{
			if (IsInstancingDraw()) {
				return m_worldMatrixArray[instanceId];
			}
			return m_model.GetWorldMatrix();
		}

	private:
		/// <summary>
		/// �X�P���g���̏������B
		/// </summary>
		/// <param name="filePath">�t�@�C���p�X�B</param>
		void InitSkeleton(const char* filePath);

	private:
		Model						m_model;	
		Skeleton					m_skeleton;							// �X�P���g��
		float						m_animationSpeed = 1.0f;
		AnimationClip*				m_animationClips = nullptr;			// �A�j���[�V�����N���b�v�B
		int							m_numAnimationClips = 0;			// �A�j���[�V�����N���b�v�̐��B
		Animation					m_animation;						// �A�j���[�V�����B
		Vector3 					m_position = Vector3::Zero;			// ���W�B
		Quaternion	 				m_rotation = Quaternion::Identity;	// ��]�B
		Vector3						m_scale = Vector3::One;				// �g�嗦�B
		int							m_maxInstance = 1;					// �ő�C���X�^���X���B
		//int							m_fixNumInstanceOnFrame = 0;		// ���̃t���[���ɕ`�悷��C���X�^���X�̐��̊m�萔�B�B
		bool						m_isEnableInstancingDraw = false;	// �C���X�^���V���O�`�悪�L���H
		std::unique_ptr<Matrix[]>	m_worldMatrixArray;					// ���[���h�s��̔z��B
		StructuredBuffer			m_worldMatrixArraySB;				// ���[���h�s��̔z��̃X�g���N�`���[�h�o�b�t�@�B
	};
}