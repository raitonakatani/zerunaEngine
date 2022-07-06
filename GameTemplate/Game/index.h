#pragma once
class Player;

	//���N���X�B
class index : public IGameObject
{
public:
	index();
	~index();
	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);
	//��]�����B
	void Rotation();

	//���W��ݒ�B
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// ��]��ݒ�B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// �傫����ݒ�B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private:
	//�������烁���o�ϐ��B
	ModelRender					m_modelRender;				//���f�������_�[�B
	Vector3						m_position;					//���W�B
	Quaternion					m_rotation;					//��]�B
	Vector3						m_scale;					//�傫���B
	Player*						m_player;                   //�v���C���[�B
	float						light = 0.4f;				//���C�g�B
	float						light2 = 0.6f;				//���C�g�B
	float						light3 = 0.0f;				//���C�g�B
	PhysicsStaticObject			m_physicsStaticObject;		//�ÓI�����I�u�W�F�N�g�B
};

