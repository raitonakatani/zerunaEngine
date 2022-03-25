#include "stdafx.h"
#include "Path.h"

#include "level3D/level.h"

Path::Path()
{

}

Path::~Path()
{

}

void Path::Load(const char* filePath)
{
	Level levelRender;
	levelRender.Init(filePath, [&](LevelObjectData& objData) {
		if (objData.ForwardMatchName(L"path") == true) {
			Point point;
			int number = _wtoi(&objData.name[4]);
			point.s_vector = objData.position;
			point.s_number = number;
			m_pointMap[number - 1] = point;
			return true;
		}
		return true;
		});
}

const Point* Path::GetNearPoint(const Vector3& position)
{
	Point* point = &m_pointMap[0];
	Vector3 diff = m_pointMap[0].s_vector - position;
	//���̏ꏊ�����ԋ߂��|�C���g�T��
	for (int i = 1; i < m_pointMap.size(); i++) {
		Vector3 diff2 = m_pointMap[i].s_vector - position;
		if (diff.LengthSq() > diff2.LengthSq()) {
			diff = diff2;
			point = &m_pointMap[i];
		}
	}
	return point;
}

const Point* Path::GetNextPoint(const int number)
{
	//�ԍ����Ō�̃|�C���g��������ŏ��̃|�C���g
	if (number == m_pointMap.size()) {
		return &m_pointMap[0];
	}
	else {
		return &m_pointMap[number];
	}
}