#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include "EnemyPath.h"

//�N���X�錾�B
class Player;
class Game;
class GameCamera;

/// <summary>
/// �G�l�~�[�B
/// </summary>
class TankEnemy2 : public IGameObject
{
public:
	//�G�l�~�[�X�e�[�g�B
	enum EnEnemyState {
		enEnemyState_Idle,					//�ҋ@�B
		enEnemyState_Chase,				//�ǐՁB
		enEnemyState_Attack,				//�U���B
		enEnemyState_ReceiveDamage,		//��_���[�W�B
		enEnemyState_Down,
		enEnemyState_Death,//�_�E���B
		enEnemyState_model
	};
public:
	TankEnemy2();
	~TankEnemy2();
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
	/// �R�m�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="kisiNumber">�R�m�̔ԍ��B</param>
	void SettankNumber(const int kisiNumber)
	{
		m_tankNumber = kisiNumber;
	}
	/// <summary>
	/// �R�m�̔ԍ����擾����B
	/// </summary>
	/// <returns>�R�m�̔ԍ��B</returns>
	const int GettankNumber() const
	{
		return m_tankNumber;
	}

	float							m_hp = 20;									//HP�B

private:
	/// <summary>
	/// �ǐՏ����B
	/// </summary>
	void Chase();
	/// <summary>
	/// ��]�����B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �U�������B
	/// </summary>
	void Attack();
	/// <summary>
	/// �v���C���[�̍U���Ƃ̓����蔻�菈���B
	/// </summary>
	void Collision();
	/// <summary>
	/// �v���C���[��T������B
	/// </summary>
	/// <returns>�v���C���[������������true�B</returns>
	void SearchPlayer();
	/// <summary>
	/// �U���p�̓����蔻��R���W�������쐬����B
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// �A�j���[�V�����̍Đ��B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �A�j���[�V�����C�x���g�p�̊֐��B
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O�B</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�̃L�[�̖��O�B</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ManageState();
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ����B
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �����X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// �ǐՃX�e�[�g�̔w�J�ڏ����B
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// �U���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessAttackStateTransition();
	/// ��_���[�W�X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// �_�E���X�e�[�g�̑J�ڏ����B
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// �U���ł��鋗�����ǂ������ׂ�B
	/// </summary>
	/// <returns>�U���ł���Ȃ�true�B</returns>
	const bool IsCanAttack() const;


	/// <summary>
	/// �o�HA
	/// </summary>
	void Aroute();
	/// <summary>
	/// �o�HB
	/// </summary>
	void Broute();

	enum EnAnimationClip {						//�A�j���[�V�����B
		enAnimationClip_Idle,					//�ҋ@�A�j���[�V�����B
		enAnimationClip_Walk,					//�����A�j���[�V�����B
		enAnimationClip_Attack,					//�U���A�j���[�V�����B
		enAnimationClip_Damage,					//��_���[�W�A�j���[�V�����B
		enAnimationClip_Down,					//�_�E���A�j���[�V�����B
		enAnimationClip_Death,					//�����A�j���[�V�����B
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;								//���f�������_�[�B
	Vector3						m_position;									//���W�B
	Vector3						m_firstPosition;							//�ŏ��̍��W�B
	Vector3						m_moveSpeed;								//�ړ����x�B
	Vector3						m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion					m_rotation;									//��]�B
	Vector3						m_scale = Vector3::One;						//�傫���B
	//Vector3 m_moveVector;
	Vector3        m_toPlayer;
	CharacterController			m_charaCon;									//�L�����R���B
	EnEnemyState				m_EnemyState = enEnemyState_Idle;			//�G�l�~�[�X�e�[�g�B
	EffectEmitter* m_effectEmitter = nullptr;			//�G�t�F�N�g�B							//�摜�B
	bool						m_isUnderAttack = false;					//�U�������H
	bool					m_isSearchPlayer = false;
	
	Player* m_player = nullptr;												//�v���C���[�B
	Game* m_game = nullptr;
	GameCamera* m_camera;
	EnemyPath m_enemypath;
	EnemyPath m_enemypath2;
	EnemyPath m_enemypath3;
	EnemyPath m_enemypath4;
	Point* m_point;
	Point* m_point2;
	Point* m_point3;
	Point* m_point4;
	float						m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float						m_idleTimer = 0.0f;							//�ҋ@�^�C�}�[�B
	float						a = 0.0f;			//�g�嗦
	bool						m_isShowHPBar = false;
	int m_tankNumber = 0;
	int                     m_Hand = -1;                   //�uHand�v�{�[����ID�B
	int                     m_weakness = -1;               //�um_weakness�v�{�[����ID�B
	SphereCollider			m_sphereCollider;
	int m_state = 0;
	int Bstate = 0;
	float m_timer = 0.0f;
	float m_timer2 = 0.0f;
	Vector3						Weak;

private:
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
};