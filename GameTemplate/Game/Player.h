#pragma once

class PAUSE;
class Game;

// �v���C���[�N���X�B
class Player : public IGameObject
{
public:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,					//�ҋ@�X�e�[�g
		enPlayerState_Walk,					//�����X�e�[�g
		enPlayerState_Run,					//����X�e�[�g
		enPlayerState_snake,				//���Ⴊ�݃X�e�[�g
		enPlayerState_StealthySteps,		//�E�ё��X�e�[�g
		enPlayerState_ReceiveDamage,		//��_���[�W�X�e�[�g
		enPlayerState_Down,					//�_�E���X�e�[�g
		enPlayerState_FirstAttack,			//�؂�U���X�e�[�g
		enPlayerState_PokeAttack,			//�˂��U���X�e�[�g
		enPlayerState_Win					//�N���A�X�e�[�g
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
	/// �󔠎擾����ݒ肷��B
	/// <param name="index">�󔠏������B</param>
	void SetPlayerHaveIndex(const int& index)
	{
		m_index = index;
	}
	/// <summary>
	/// �󔠏��������擾����B
	/// </summary>
	/// <returns>�󔠏������B</returns>
	int GetPlayerHaveIndex()
	{
		return m_index;
	}
	/// <summary>
	///	�q�b�g�|�C���g�̍ő�l���擾����B
	/// </summary>
	/// <returns>�q�b�g�|�C���g�B</returns>
	float GetMaxHP()
	{
		return m_maxHP;
	}
	/// <summary>
	/// �X�^�~�i�|�C���g�̍ő�l���擾����B
	/// </summary>
	/// <returns>�X�^�~�i�|�C���g�B</returns>
	float GetMaxStamina()
	{
		return m_maxStamina;
	}
	/// <summary>
	/// �X�L���|�C���g�̍ő�l���擾����B
	/// </summary>
	/// <returns>�X�L���|�C���g�B</returns>
	float GetMaxSP()
	{
		return m_maxSP;
	}
	/// <summary>
	/// �X�^�[�g�^�C�}�[���擾����B
	/// </summary>
	/// <returns>�X�^�[�g�^�C�}�[�B</returns>
	float GetStartTimer()
	{
		return m_startTimer;
	}
	/// <summary>
	/// �N���e�B�J���U���̃q�b�g�����擾����B
	/// </summary>
	/// <returns>�N���e�B�J���B</returns>
	int GetHitCritical()
	{
		return m_critical;
	}
	/// <summary>
	/// �N���e�B�J���U���̃q�b�g����ݒ肷��B
	/// </summary>
	/// <param name="critical">�N���e�B�J���̃q�b�g���B</param>
	void SetHitCritical(const int& critical)
	{
		m_critical = critical;
	}
	/// <summary>
	/// ���j���[��ʂ̏�Ԃ�ݒ肷��B
	/// </summary>
	/// <returns>���j���[�B</returns>
	bool IsOpenMenu()
	{
		return m_menu;
	}
	/// <summary>
	/// ���݂�HP���擾����B
	/// </summary>
	/// <returns>���݂̃q�b�g�|�C���g�B</returns>
	float GetHP()
	{
		return m_hp;
	}
	/// <summary>
	/// ���݂�SP���擾����B
	/// </summary>
	/// <returns>���݂̃X�L���|�C���g�B</returns>
	float GetSP()
	{
		return m_sp;
	}


	bool					prok = false;						//�N���e�B�J���U���̏�������B
	bool					m_down = false;								//���S����B
	int						st = 0;										//�v���C���[�X�e�[�g

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
	/// ���Ⴊ�݃X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcesssnakeStateTransition();
	/// <summary>
	/// �E�ё��X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessStealthyStepsStateTransition();
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
	/// <summary>
	/// �N���A�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessWinStateTransition();

	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,			//�ҋ@�A�j���[�V����
		enAnimClip_Walk,			//�����A�j���[�V����
		enAnimClip_Run,				//����A�j���[�V����
		enAnimClip_snake,			//���Ⴊ�݃A�j���[�V����
		enAnimClip_StealthySteps,	//�E�ё��A�j���[�V����
		enAnimClip_FirstAttack,		//�؂�U���A�j���[�V����
		enAnimClip_PokeAttack,		//�˂��U���A�j���[�V����
		enAnimClip_ReceiveDamage,	//��_���[�W�A�j���[�V����
		enAnimClip_Down,			//�_�E���A�j���[�V����
		enAnimClip_Win,				//�N���A�A�j���[�V����
		enAnimClip_Num,				//�A�j���[�V�����̐�
	};

	// �N���X IGameObject
	Game*					m_game = nullptr;							// �Q�[���B
	PAUSE*					m_pause = nullptr;							// �|�[�Y�B
	// �N���X Engine
	EffectEmitter*			m_effectEmitter = nullptr;					// �G�t�F�N�g�B
	EffectEmitter*			m_effectEmitter2 = nullptr;					// �G�t�F�N�g�B
	ModelRender				m_modelRender;                              // ���f�������_�[
	CharacterController     m_charaCon;	                                // �L�����R��
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    // �A�j���[�V�����N���b�v
	// enum
	EnPlayerState           m_playerState = enPlayerState_Idle;         // �v���C���[�X�e�[�g
	// �֐�
	Vector3                 m_forward = Vector3::AxisZ;                 // �O�����̃x�N�g��
	Vector3					m_position;					                // ���W
	Vector3                 m_moveSpeed;                                // �ړ����x
	Vector3					m_scale = g_vec3One;		                // �g�嗦
	Quaternion				m_rotation;					                // ��]
	int                     m_swordJointBoneId = -1;					//�uSword�v�{�[����ID�B
	int                     m_swordBoneId = -1;							//�uSword�v�{�[����ID�B
	int						m_hitSE = 0;								// ��_���[�W�̌��ʉ��B
	int						m_deathSE = 0;								// �_�E�����̌��ʉ��B
	int						m_critical = 0;								// �N���e�B�J���U���̏�������B
	int						m_index = 0;								// �󔠁B
	bool					m_menu = false;								// ���j���[
	bool					m_cooltime = false;							// �N�[���^�C���B
	bool                    m_isUnderAttack = false;                    // �U�������H
	float					m_hp = 100.0f;								// HP�B
	float					m_stamina = 200.0f;							// �X�^�~�i�B
	float					m_sp = 200.0f;								// SP�B
	float					m_maxHP = 100.0f;							// �t��HP
	float					m_maxStamina = 200.0f;						// �t���X�^�~�i�B
	float					m_maxSP = 700.0f;							// �t��SP�B
	float					m_downTimer = 0.0f;							// ���S���Ă���̎��ԁB
	float					m_startTimer = 0.0f;						// �Q�[���J�n���̏����B
};