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

	void Init(const char* filePath);
	std::map<int, Point> m_pointlist;
	LevelRender m_level;
	
private:
	Point* m_point;
};