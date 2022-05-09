#include "stdafx.h"
#include "EnemyPath.h"

void EnemyPath::Init(const char* filePath)
{
	m_level.Init(filePath, [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"path") == true)
			{
				
				point.s_number = objData.number;
				point.s_position = objData.position;
				//posi = objData.position;
				int number = objData.number;
				m_pointlist.emplace(number, point);
				return true;
			}
			return true;
		});
}