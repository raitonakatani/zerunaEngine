#include "stdafx.h"
#include "MovedPath.h"

namespace 
{
	const float MOLECULE = 5.0f;
	const float ADD_DEGREE = 180.0f / 2 / 180.0f * Math::PI;
	const float DEGREE_SPEED = 380.0f;

}

MovedPath::MovedPath()
{

}

MovedPath::~MovedPath()
{

}

void MovedPath::Init(const Vector3& position, const float moveSpeed)
{
	m_position = position;
	m_moveSpeed = moveSpeed;
	//���݂̍��W�����ԋ߂��|�C���g���擾����
	m_point = m_path.GetNearPoint(m_position);
}

void MovedPath::ReadPath(const char* filePath)
{
	m_path.Load(filePath);
}

const Vector3& MovedPath::Move()
{
	if (m_isStart) {
		m_moveVector = m_point->s_vector - m_position;
		m_moveVector.Normalize();
		m_isStart = false;
	}
	else {
		Vector3 distance = m_point->s_vector - m_position;
		//��~������Ȃ�������
		if (!m_isStop) {
			if (distance.LengthSq() <= m_moveSpeed * m_moveSpeed * g_gameTime->GetFrameDeltaTime()) {
				//���݂̐��ʂ̃x�N�g���ƁA���̃p�X�Ɍ����Ă̈ړ��x�N�g�������߂�
				m_point = m_path.GetNextPoint(m_point->s_number);
				Vector3 distance = m_point->s_vector - m_position;
				distance.y = 0;
				distance.Normalize();
				Vector3 moveVector = m_moveVector;
				moveVector.y = 0.0f;
				moveVector.Normalize();
				//�e�x�N�g���̊p�x�����߂�(0�`PI)
				float degree1 = acosf(moveVector.x);
				float degree2 = acosf(distance.x);
				//2PI�܂őΉ�������
				if (moveVector.z <= 0.0f) {
					degree1 = degree1 + (Math::PI - degree1) * DEGREE_SPEED;
				}
				if (distance.z <= 0.0f) {
					degree2 = degree2 + (Math::PI - degree2) * DEGREE_SPEED;
				}
				//�p�x�̍������߂�
				float degree = 0.0f;
				if (degree1 >= degree2) {
					if (degree1 - degree2 >= Math::PI) {
						degree = Math::PI * 2 - degree1 + degree2;
						m_isAddDegree = false;
					}
					else {
						degree = degree1 - degree2;
						m_isAddDegree = true;
					}
				}
				else {
					if (degree2 - degree1 >= Math::PI) {
						degree = Math::PI * 2 - degree2 + degree1;
						m_isAddDegree = true;
					}
					else {
						degree = degree2 - degree1;
						m_isAddDegree = false;
					}
				}
				m_time = degree / ADD_DEGREE;
				//�ꎞ��~
				m_isStop = true;
			}
		}
		//��~����������
		else {
			//�^�C�}�[�����ȏ�ɂȂ�����ړ�����
			if (m_timer >= m_time) {
				m_moveVector = m_point->s_vector - m_position;
				m_moveVector.Normalize();
				m_isStop = false;
				m_timer = 0.0f;
			}
			//�x�N�g������]������
			else {
				Quaternion rot;
				if (m_isAddDegree) {
					rot.SetRotation(Vector3::AxisY, ADD_DEGREE * g_gameTime->GetFrameDeltaTime());
				}
				else {
					rot.SetRotation(Vector3::AxisY, -ADD_DEGREE * g_gameTime->GetFrameDeltaTime());
				}
				rot.Apply(m_moveVector);
				m_timer += g_gameTime->GetFrameDeltaTime();
			}
		}
	}
	//��~������Ȃ�������
	if (!m_isStop) {
		m_position += m_moveVector * g_gameTime->GetFrameDeltaTime() * m_moveSpeed;
	}
	return m_position;
}
