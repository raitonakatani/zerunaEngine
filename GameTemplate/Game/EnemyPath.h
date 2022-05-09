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
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		point.s_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition()
	{
		return point.s_position;
	}

	void Init(const char* filePath);
	std::map<int, Point> m_pointlist;
	LevelRender m_level;
	LevelRender m_level2;

	Point point;
	Vector3 posi;
private:
//	Point m_point;
};