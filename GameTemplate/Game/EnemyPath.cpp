#include "stdafx.h"
#include "EnemyPath.h"

void EnemyPath::Init(const char* filePath)
{
	m_level.Init(filePath, [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"path") == true)
			{

				m_point.s_number = objData.number;
				m_point.s_position = objData.position;
				int number = objData.number;
				m_pointlist.emplace(number, m_point);
				return true;
			}
			return true;
		});
}