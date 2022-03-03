#pragma once
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
		enPlayerState_ReceiveDamage,       //��_���[�W�X�e�[�g
		enPlayerState_Guard,               //�K�[�h�X�e�[�g
		enPlayerState_Avoidance,           //����X�e�[�g
		enPlayerState_FirstAttack,         //�P���ڂ̍U���X�e�[�g
		enPlayerState_SecondAttack,        //�Q���ڂ̍U���X�e�[�g
		enPlayerState_ThirdAttack          //�R���ڂ̍U���X�e�[�g
	};
public:
	/// <summary>
	/// �U���X�e�[�g
	/// </summary>
	enum EnAttackState
	{
		enAttackState_FirstAttack,        //�P����
		enAttackState_SecondAttack,       //�Q����
		enAttackState_ThirdAttack         //�R����
	};
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
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
	/// �A���U������
	/// </summary>
	void Hit();
	/// <summary>
	/// �U���̓����蔻����쐬���鏈��
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// �K�[�h����
	/// </summary>
	void Guard();
	/// <summary>
	/// �h��̓����蔻����쐬���鏈��
	/// </summary>
	void MakeGuardCollision();
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
	/// �K�[�h�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessGuardStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAvoidanceStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttackStateTransition();

private:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	       //�ҋ@�A�j���[�V����
		enAnimClip_Walk,           //�����A�j���[�V����
		enAnimClip_Run,		       //����A�j���[�V����
		enAnimClip_Guard,          //�K�[�h���̃A�j���[�V����
		enAnimClip_Rolling,        //��]����A�j���[�V����
		enAnimClip_FirstAttack,    //�P���ڂ̍U���A�j���[�V����
		enAnimClip_SecondAttack,   //�Q���ڂ̍U���A�j���[�V����
		enAnimClip_ThirdAttack,    //�R���ڂ̍U���A�j���[�V����
		enAnimClip_ReceiveDamage,  //��_���[�W�X�e�[�g
		enAnimClip_Num,		       //�A�j���[�V�����̐�
	};

	ModelRender				m_modelRender;                              //���f�������_�[
	EnPlayerState           m_playerState = enPlayerState_Idle;         //�v���C���[�X�e�[�g
	EnAttackState           m_attackState = enAttackState_FirstAttack;  //�U���X�e�[�g
	Vector3					m_position;					                //���W
	Vector3                 m_moveSpeed;                                //�ړ����x
	Vector3                 m_forward = Vector3::AxisZ;                 //�O�����̃x�N�g��
	Skeleton                m_skeleton;	                                //�X�P���g��
	Quaternion				m_rotation;					                //��]
	Vector3					m_scale = g_vec3One;		                //�g�嗦
	Animation				m_animation;				                //�A�j���[�V����
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //�A�j���[�V�����N���b�v
	CharacterController     m_charaCon;	                                //�L�����R��
	float                   m_secondAttackTimer = 0.0f;                 //�Q���ڂ̍U���^�C�}�[
	float                   m_thirdAttackTimer = 0.0f;                  //�R���ڂ̍U���^�C�}�[
	bool                    m_secondAttackFlag = false;                 //�Q���ڂ̍U���t���O
	bool                    m_thirdAttackFlag = false;                  //�R���ڂ̍U���t���O
	bool                    m_isUnderAttack = false;                    //�U�������H
};