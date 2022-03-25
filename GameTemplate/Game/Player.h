#pragma once

class GameCamera;

// �v���C���[�N���X�B
class Player : public IGameObject
{
public:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,                //�ҋ@�X�e�[�g
		enPlayerState_Walk,                //�����X�e�[�g
		enPlayerState_Run,                 //����X�e�[�g
		enPlayerState_StealthySteps,       //�E�ё��X�e�[�g
		enPlayerState_Crouch,
		enPlayerState_ReceiveDamage,       //��_���[�W�X�e�[�g
		enPlayerState_Down,                //�_�E���X�e�[�g
		enPlayerState_Avoidance,           //����X�e�[�g
		enPlayerState_FirstAttack,         //�P���ڂ̍U���X�e�[�g
		enPlayerState_PokeAttack           //�˂��U���X�e�[�g
	};
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	Vector3                 m_forward = Vector3::AxisZ;                 //�O�����̃x�N�g��

private:
	/// <summary>
	/// �A�j���[�V�����̏�����
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	///	�U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �U���̓����蔻����쐬���鏈��
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// �������
	/// </summary>
	void Avoidance();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�������O�C�x���g
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O</param>
	/// <param name="eventName">�A�j���[�V�����L�[�̖��O</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// ���ʂ̃X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// �E�ё��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessStealthyStepsStateTransition();

	void ProcessCrouchStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAvoidanceStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDownStateTransition();

private:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	       //�ҋ@�A�j���[�V����
		enAnimClip_Walk,           //�����A�j���[�V����
		enAnimClip_Run,		       //����A�j���[�V����
		enAnimClip_StealthySteps,  //�E�ё��A�j���[�V����
		enAnimClip_Crouch,
		enAnimClip_Rolling,        //��]����A�j���[�V����
		enAnimClip_FirstAttack,    //�P���ڂ̍U���A�j���[�V����
		enAnimClip_PokeAttack,     //�˂��U���A�j���[�V����
		enAnimClip_ReceiveDamage,  //��_���[�W�A�j���[�V����
		enAnimClip_Down,           //�_�E���A�j���[�V����
		enAnimClip_Num,		       //�A�j���[�V�����̐�
	};
	int                     m_sword_jointBoneId = -1;                   //�uSword�v�{�[����ID�B
	ModelRender				m_modelRender;                              //���f�������_�[
	EnPlayerState           m_playerState = enPlayerState_Idle;         //�v���C���[�X�e�[�g
	Vector3					m_position;					                //���W
	Vector3                 m_moveSpeed;                                //�ړ����x
	Skeleton                m_skeleton;	                                //�X�P���g��
	Quaternion				m_rotation;					                //��]
	Vector3					m_scale;		                //�g�嗦
	Animation				m_animation;				                //�A�j���[�V����
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //�A�j���[�V�����N���b�v
	CharacterController     m_charaCon;	                                //�L�����R��
	bool                    m_isUnderAttack = false;                    //�U�������H
	GameCamera* camera;
};