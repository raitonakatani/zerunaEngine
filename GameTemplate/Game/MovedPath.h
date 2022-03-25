#pragma once
#include "Path.h"

/// <summary>
/// �I�u�W�F�N�g���p�X�ړ�������B
/// </summary>
class MovedPath
{
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	MovedPath();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~MovedPath();
	const Vector3& Move();
	/// <summary>
	/// �����������B
	/// </summary>
	/// <param name="position">���W�B</param>
	/// <param name="moveSpeed">�ړ����x�B</param>
	void Init(const Vector3& position, const float moveSpeed);
	/// <summary>
	/// �p�X�f�[�^��ǂݍ��ށB
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void ReadPath(const char* filePath);
	/// <summary>
	/// �ړ������̃x�N�g�����擾�B
	/// </summary>
	/// <returns>�ړ������x�N�g���B</returns>
	const Vector3& GetMoveVector() const
	{
		return m_moveVector;
	}
	/// <summary>
	/// ��~���H
	/// </summary>
	/// <returns>��~���Ȃ�true�B</returns>
	const bool GetIsStop() const
	{
		return m_isStop;
	}

private:
	Vector3			m_position = Vector3::Zero;					//���W
	Vector3			m_moveVector = Vector3::Zero;				//�ړ��x�N�g��
	float			m_moveSpeed = 0.0f;							//�ړ����x
	bool			m_isCulcMoveVector = false;					//�ړ��x�N�g�����v�Z�������ǂ���
	Path			m_path;
	const Point* m_point = nullptr;							//���������Ă�|�C���g
	bool			m_isStart = true;							//�X�^�[�g
	bool			m_isStop = false;							//�X�g�b�v�H
	bool			m_isAddDegree = true;						//�����]��
	float			m_time = 0.0f;								//�����]���̎���
	float			m_timer = 0.0f;

};
