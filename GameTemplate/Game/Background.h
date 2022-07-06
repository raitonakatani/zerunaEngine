#pragma once

class Player;

class Background : public IGameObject
{

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

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
	ModelRender					m_modelRender;					//�X�e�[�W
	ModelRender					m_modelRender2;					//�������f��
	Player*						m_player;						//�v���C���[
	PhysicsStaticObject			m_physicsStaticObject;			//�ÓI�����I�u�W�F�N�g�E
	Vector3						m_position;						//���W�B
	Quaternion					m_rotation;						//��]�B
	Vector3						m_scale;						//�傫���B

};