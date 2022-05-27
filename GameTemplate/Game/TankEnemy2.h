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
class TankEnemy;
class EnemyPath;
class box;

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
		enEnemyState_look,//�x��B
		enEnemyState_alert,//�x���B
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
	/// �G�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="kisiNumber">�G�̔ԍ��B</param>
	void SettankNumber(const int kisiNumber)
	{
		m_tankNumber = kisiNumber;
	}
	/// <summary>
	/// �G�̔ԍ����擾����B
	/// </summary>
	/// <returns>�G�̔ԍ��B</returns>
	const int GettankNumber() const
	{
		return m_tankNumber;
	}

	Vector3 tankPosi;
	float					m_hp = 20;									//HP�B


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

	void lookTransition();

	void alertTransition();

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
		enAnimationClip_look,					//���K�A�j���[�V�����B
		enAnimationClip_alert,					//�x���A�j���[�V����
		enAnimationClip_Num,					//�A�j���[�V�����̐��B
	};
	AnimationClip			m_animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	ModelRender				m_modelRender;								//���f�������_�[�B
	Vector3					m_position;									//���W�B
	Vector3					m_moveSpeed;								//�ړ����x�B
	Vector3					m_forward = Vector3::AxisZ;					//�G�l�~�[�̐��ʃx�N�g���B
	Quaternion				m_rotation;									//��]�B
	Vector3					m_scale = Vector3::One;						//�傫���B
	CharacterController		m_charaCon;									//�L�����R���B
	EnEnemyState			m_EnemyState = enEnemyState_Idle;			//�G�l�~�[�X�e�[�g�B
	EffectEmitter* m_effectEmitter = nullptr;					//�G�t�F�N�g�B
	bool					m_isUnderAttack = false;					//�U�������H
	bool					m_isSearchPlayer = false;					//���邯�Ă��邩�H
	Player* m_player = nullptr;							//�v���C���[�B
	TankEnemy* m_tank = nullptr;							//�^���N
	Game* m_game = nullptr;							//�Q�[��
	GameCamera* m_camera;									//�J����
	box* m_box;										//�x���A�͗l
	EnemyPath				m_enemypath2;								//�p�X
	EnemyPath				m_enemypath4;								//�p�X
	Point* m_point2;									//�|�C���g
	Point* m_point4;									//�|�C���g
	SpriteRender			alertSprite;								//�x�����x��
	float					m_chaseTimer = 0.0f;						//�ǐՃ^�C�}�[�B
	float					m_idleTimer = 0.0f;							//�ҋ@�^�C�}�[�B
	float					alertTimet = 0.0f;							//�g�嗦
	int						m_tankNumber = 0;							//�u�^���N�v�i���o�[
	int						m_Hand = -1;								//�uHand�v�{�[����ID�B
	int						m_weakness = -1;							//�um_weakness�v�{�[����ID�B
	SphereCollider			m_sphereCollider;							//�X�t�B�A�R���C�_�[
	int						state = 0;									//�x���X�e�[�g
	int						alertLevel = 0;								//�x�����x��
	float					m_timer = 0.0f;								//�^�C�}�[
	Vector3					Weak;										//�v���C���[�Ƃ̋���
	int						ab;											//���K��
	float					m_angl = 0.40f;								//����p
	float					m_range = 1000.0f;							//���́i�����鋗���j
	Vector3					targetpos;									//�Ō�Ɍ����v���C���[�̃|�W�V����
	bool					mikke = false;								//���������G��ǂ�������B
	int						hakken = 0;									//�v���C���[�����������̃T�E���h
	int						fasthakkenn = 0;							//�ŏ��Ɍ��������̏���
};
