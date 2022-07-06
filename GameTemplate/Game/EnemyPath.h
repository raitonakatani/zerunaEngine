#pragma once

#include "LevelRender.h"

struct Point {
	Vector3 s_position;
	int     s_number;
};

class  EnemyPath
{
public:
	Point* GetFirstPoint()
	{
		return &m_pointlist[0];
	}

	Point* GetNextPoint(const int number)
	{
		return &m_pointlist[number + 1];
	}

	int GetPointListSize()
	{
		return m_pointlist.size();
	}


	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_point.s_position = position;
	}
	/// <summary>
	/// ���W���擾����B
	/// </summary>
	/// <returns>���W�B</returns>
	const Vector3& GetPosition()
	{
		return m_point.s_position;
	}

	void Init(const char* filePath);
	std::map<int, Point>	m_pointlist;		//�|�C���g���X�g�B
	LevelRender				m_level;			//���x���B
	Point					m_point;			//�|�C���g�B
};