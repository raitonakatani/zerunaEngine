#pragma once

class GameCamera;
class PAUSE;
class Game;
//class Title;

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
		enPlayerState_snake,
		enPlayerState_StealthySteps,       //�E�ё��X�e�[�g
		enPlayerState_ReceiveDamage,       //��_���[�W�X�e�[�g
		enPlayerState_Down,                //�_�E���X�e�[�g
		enPlayerState_FirstAttack,         //�P���ڂ̍U���X�e�[�g
		enPlayerState_PokeAttack,           //�˂��U���X�e�[�g
		enPlayerState_Win
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
	/// index���v���X����B
	/// </summary>
	void Plusindex()
	{
		index++;
	}
	/// <summary>
	/// index���擾����B
	/// </summary>
	/// <returns>�|�[�V�����B</returns>
	int Getindex()
	{
		return index;
	}

	Vector3                 m_forward = Vector3::AxisZ;                 //�O�����̃x�N�g��
	float m_hp = 100.0f;
	float					m_sp = 150.0f;						//sp�B

	int critical = 0;
	bool prok = false;
	int m_prokcamera = 0;

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
		enAnimClip_Idle,	       //�ҋ@�A�j���[�V����
		enAnimClip_Walk,           //�����A�j���[�V����
		enAnimClip_Run,		       //����A�j���[�V����
		enAnimClip_snake,
		enAnimClip_StealthySteps,  //�E�ё��A�j���[�V����
		enAnimClip_FirstAttack,    //�P���ڂ̍U���A�j���[�V����
		enAnimClip_PokeAttack,     //�˂��U���A�j���[�V����
		enAnimClip_ReceiveDamage,  //��_���[�W�A�j���[�V����
		enAnimClip_Down,           //�_�E���A�j���[�V����
		enAnimClip_Win,
		enAnimClip_Num,		       //�A�j���[�V�����̐�
	};


	int                     m_sword_jointBoneId = -1;                   //�uSword�v�{�[����ID�B
	int                     m_swordBoneId = -1;                   //�uSword�v�{�[����ID�B
	ModelRender				m_modelRender;                              //���f�������_�[
	EnPlayerState           m_playerState = enPlayerState_Idle;         //�v���C���[�X�e�[�g
	int st = 0;
	EnAttackState           m_attackState = enAttackState_FirstAttack;  //�U���X�e�[�g
	Vector3					m_position;					                //���W
	Vector3                 m_moveSpeed;                                //�ړ����x
	Skeleton                m_skeleton;	                                //�X�P���g��
	Quaternion				m_rotation;					                //��]
	Vector3					m_scale = g_vec3One;		                //�g�嗦
	Animation				m_animation;				                //�A�j���[�V����
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //�A�j���[�V�����N���b�v
	CharacterController     m_charaCon;	                                //�L�����R��
	bool                    m_isUnderAttack = false;                    //�U�������H
	EffectEmitter*			m_effectEmitter = nullptr;					//�G�t�F�N�g�B
	EffectEmitter*			m_effectEmitter2 = nullptr;					//�G�t�F�N�g�B
	PAUSE*					m_pause = nullptr;
	Game* m_game;
//	Title* m_title;
	SpriteRender			m_HPRender;							        //�摜�B
	SpriteRender			m_staminaRender;
	SpriteRender			m_senseRender;
	SpriteRender			m_HPberRender;						        //�摜�B
	SpriteRender			m_stmnberRender;
	SpriteRender			m_senseberRender;
	SpriteRender			m_spriteRender3;
	float					life = 100.0f;								//�t��HP�o�[
	float					m_sutamina = 200.0f;						//�X�^�~�i�B
	float					m_hurusutamina = 200.0f;					//�t���X�^�~�i�B
//	float					m_sp = 500.0f;								//sp�B
	float					m_hurusp = 700.0f;							//�t��sp�B
	float					cooltime = 0.0f;
	bool					m_menu = false;
	bool					COOLtime = false;
	int						index = 0;
	float					m_alpha = 0.0f;								//pressbutton�̃��l�B]
	float					criticalAlpha = 0.0f;
	float					criticalscale = 2.0f;
	bool					m_down = false;
	float					m_downtimer = 0.0f;
	int						m_Hitse = 0;
	int						m_deathse = 0;
	float					m_startwalk = 0.0f;

};

