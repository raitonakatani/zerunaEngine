#pragma once

class Player;
class Game;
class MovedPath;

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"

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
		enSpeedEnemyState_Wander,
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

	void LoadPath(const int number);

private:
	void PathFind();

	void Wander();
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
	/// �p�j�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessWanderStateTransition();
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

	bool GetPlayer()
	{
		if (m_player == nullptr)
		{
			m_player = FindGO<Player>("player");
			return false;
		}
		return true;
	}

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
	//�ړ������x�N�g���B
	std::unique_ptr<MovedPath>	m_movedPath;
	ModelRender                 m_modelRender;                              //���f�������_�[
	CharacterController         m_charaCon;                                 //�L�����R��
	EnSpeedEnemyState           m_speedEnemyState = enSpeedEnemyState_Wander; //�X�s�[�h�G�l�~�[�X�e�[�g
	Animation			     	m_animation;				                //�A�j���[�V����
	AnimationClip               m_animationClipArray[enAnimClip_Num];	    //�A�j���[�V�����N���b�v
	Vector3                     m_position;                                 //���W
	Vector3                     m_scale = Vector3::One;                     //�g�嗦
	Vector3                     m_forward = Vector3::AxisZ;                 //�O�����̃x�N�g��
	Vector3                     m_moveSpeed;                                //�ړ����x
	Quaternion                  m_rotation;                                 //��]
	Player*                     m_player = nullptr;                         //�v���C���[
	float                       m_idleTimer = 0.0f;                         //�ҋ@�^�C�}�[
	float                       m_chaseTimer = 0.0f;                        //�ǐՃ^�C�}�[
	int                         m_hp = 100;                                 //HP
	int                         m_punchBoneId = -1;                         //�p���`�̃{�[��
	bool                        m_isUnderAttack = false;                    //�U�������H
	Game*                       m_game = nullptr;
	TknFile                     m_tknFile;
	nsAI::NaviMesh              m_nvmMesh;
	nsAI::Path                  m_path;
	nsAI::PathFinding           m_pathFinding;
};

