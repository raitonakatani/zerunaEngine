#pragma once


class Player : public IGameObject
{
public:
	////////////////////////////////////
	// �����o�֐�
	////////////////////////////////////
	Player();
	~Player();

	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

	/// <summary>
	/// ���W��ݒ�B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾�B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition() //const
	{
		return m_position;
	}
	/// <summary>
	/// �W�����v�񐔂�ݒ�B
	/// </summary>
	/// <param name="secondjump">�W�����v�B</param>
	void PlusSECONDJUMP()
	{
		secondjump++;
	}
	/// <summary>
	/// �W�����v�񐔂��擾�B
	/// </summary>
	/// <returns>�W�����v�B</returns>
	int GetSECONDJUMP()
	{
		return secondjump;
	}

	Vector3 m_moveSpeed;	                         //�ړ����x�B

private:
	/// <summary>
	///�ړ������B
	/// <summary>
	void Move();
	/// <summary>
	///��]�����B
	/// <summary>
	void Turn();
	/// <summary>
	///�X�e�[�g�Ǘ��B
	/// <summary>
	void ManageState();
	/// <summary>
	///�A�j���[�V�����̍Đ��B
	/// <summary>
	void PlayAnimation();


	// �����o�ϐ�
	enum EnAnimationClip {		//�A�j���[�V�����B
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_run,	//����A�j���[�V�����B
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	SoundSource* jumpse;
	ModelRender m_modelRender;
	AnimationClip animationClips[enAnimationClip_Num];
	Vector3 m_position;						           //���W�B
	CharacterController m_charaCon;		             //�L�����N�^�[�R���g���[���[�B
	Quaternion m_rotation;                          //�N�H�[�^�j�I��
	int playerState = 0;	                            //�v���C���[�̃X�e�[�g(���)��\���ϐ��B
	int secondjump = 0;                           //
};
