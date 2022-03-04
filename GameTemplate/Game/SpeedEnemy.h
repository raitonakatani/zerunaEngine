#pragma once

class Player;

/// <summary>
/// �X�s�[�h�G�l�~�[�N���X
/// </summary>
class SpeedEnemy :public IGameObject
{
public:
	/// <summary>
	/// �X�s�[�h�G�l�~�[�X�e�[�g
	/// </summary>
	enum EnSpeedEnemyState
	{
		enSpeedEnemyState_Idle,           //�ҋ@�X�e�[�g
		enSpeedEnemyState_Chase,          //�ǐՃX�e�[�g
		enSpeedEnemyState_Attack,         //�U���X�e�[�g
		enSpeedEnemyState_Shout,          //���уX�e�[�g
		enSpeedEnemyState_ReceiveDamage,  //��_���[�W�X�e�[�g
		enSpeedEnemyState_Down,           //�_�E���X�e�[�g
	};

public:
	SpeedEnemy();
	~SpeedEnemy();
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
	/// <summary>
	/// HP���擾����
	/// </summary>
	/// <returns>HP</returns>
	const int& GetHp() const
	{
		return m_hp;
	}

private:
	/// <summary>
	/// �A�j���[�V�����̏�����
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// �ǐՏ���
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �U���p�̓����蔻����쐬
	/// </summary>
	void MakeAttackCollision();
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
	/// �ǐՃX�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// ���уX�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessShoutStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDownStateTransition();

	/// <summary>
	/// �v���C���[�𔭌��������H
	/// </summary>
	/// <returns>����������true</returns>
	const bool SearchPlayer() const;
	/// <summary>
	/// �v���C���[���U���ł��邩�H
	/// </summary>
	/// <returns>�U���ł���Ȃ�true</returns>
	const bool IsCanAttack() const;

private:
	/// <summary>
	/// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^
	/// </summary>
	enum EnAnimationClip
	{
		enAnimClip_Idle,           //�ҋ@�A�j���[�V����
		enAnimClip_Run,            //����A�j���[�V����
		enAnimClip_Attack,         //�U���A�j���[�V����
		enAnimClip_Shout,          //���уA�j���[�V����
		enAnimClip_ReceiveDamage,  //��_���[�W�X�e�[�g
		enAnimClip_Down,           //�_�E���X�e�[�g
		enAnimClip_Num             //�A�j���[�V�����̐�
	};

	ModelRender             m_modelRender;                              //���f�������_�[
	CharacterController     m_charaCon;                                 //�L�����R��
	EnSpeedEnemyState       m_speedEnemyState = enSpeedEnemyState_Idle; //�X�s�[�h�G�l�~�[�X�e�[�g
	Animation				m_animation;				                //�A�j���[�V����
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //�A�j���[�V�����N���b�v
	Vector3                 m_position;                                 //���W
	Vector3                 m_scale = Vector3::One;                     //�g�嗦
	Vector3                 m_forward = Vector3::AxisZ;                 //�O�����̃x�N�g��
	Vector3                 m_moveSpeed;                                //�ړ����x
	Quaternion              m_rotation;                                 //��]
	Player*                 m_player = nullptr;                         //�v���C���[
	float                   m_idleTimer = 0.0f;                         //�ҋ@�^�C�}�[
	float                   m_chaseTimer = 0.0f;                        //�ǐՃ^�C�}�[
	int                     m_hp = 100;                                 //HP
	int                     m_punchBoneId = -1;                         //�p���`�̃{�[��
	bool                    m_isUnderAttack = false;                    //�U�������H
};

