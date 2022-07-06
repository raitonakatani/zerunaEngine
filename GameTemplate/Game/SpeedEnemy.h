#pragma once

#include "EnemyPath.h"

class Player;
class box;
class Game;

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
		enSpeedEnemyState_Death,					//�����A�j���[�V�����B
		enSpeedEnemyState_alert,
	};

public:
	SpeedEnemy();
	~SpeedEnemy();
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
	/// <summary>
	/// HP���擾����
	/// </summary>
	/// <returns>HP</returns>
	const int& GetHp() const
	{
		return m_hp;
	}

	/// <summary>
	/// �R�m�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="kisiNumber">�R�m�̔ԍ��B</param>
	void SetspeedNumber(const int kisiNumber)
	{
		m_speedNumber = kisiNumber;
	}
	/// <summary>
	/// �R�m�̔ԍ����擾����B
	/// </summary>
	/// <returns>�R�m�̔ԍ��B</returns>
	const int GetspeedNumber() const
	{
		return m_speedNumber;
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

	void alertTransition();
//	void lookTransition();

	/// <summary>
	/// �v���C���[�𔭌��������H
	/// </summary>
	/// <returns>����������true</returns>
	void SearchPlayer();
	/// <summary>
	/// �v���C���[���U���ł��邩�H
	/// </summary>
	/// <returns>�U���ł���Ȃ�true</returns>
	const bool IsCanAttack() const;

	/// <summary>
	/// �o�HA
	/// </summary>
	void Aroute();

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
		enAnimationClip_Death,					//�����A�j���[�V�����B
		enAnimationClip_alert,
		enAnimClip_Num             //�A�j���[�V�����̐�
	};
	
	Game*					m_game;										//�Q�[��
	box*					m_box;										//�x���A�͗l
	Player*					m_player = nullptr;                         //�v���C���[
	Point*					m_point[14];								//�|�C���g
	Animation				m_animation;				                //�A�j���[�V����
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //�A�j���[�V�����N���b�v
	CharacterController     m_charaCon;                                 //�L�����R��
	EnemyPath				m_enemypath[14];							//�p�X
	EnSpeedEnemyState       m_speedEnemyState = enSpeedEnemyState_Idle; //�X�s�[�h�G�l�~�[�X�e�[�g
	ModelRender             m_modelRender;                              //���f�������_�[
	SphereCollider			m_sphereCollider;
	Vector3                 m_position;                                 //���W
	Vector3                 m_scale = Vector3::One;                     //�g�嗦
	Vector3                 m_forward = Vector3::AxisZ;                 //�O�����̃x�N�g��
	Vector3                 m_moveSpeed;                                //�ړ����x
	Vector3					Weak;
	Vector3					targetpos;
	Quaternion              m_rotation;                                 //��]
	int						state = 0;
	int                     m_hp = 100;                                 //HP
	int                     m_punchBoneId = -1;                         //�p���`�̃{�[��
	int						alertLevel = 0;								//�x��
	int						m_weakness = -1;							//�um_weakness�v�{�[����ID�B
	int						m_speedNumber = 0;
	int						ab;											//���K��
	int						hakken = 0;									//�v���C���[�����������̃T�E���h
	int						fasthakkenn = 0;							//�ŏ��Ɍ��������̏���				
	bool					m_isSearchPlayer = false;
	bool					mikke = false;
	bool                    m_isUnderAttack = false;					//�U�������H
	float                   m_idleTimer = 0.0f;                         //�ҋ@�^�C�}�[
	float                   m_chaseTimer = 0.0f;                        //�ǐՃ^�C�}�[
	float					m_angl = 0.40f;
	float					m_range = 1000.0f;
	float					m_timer = 0.0f;
	float					alertTimet = 0.0f;							//�g�嗦
};

