/*!
 *@brief	�}�b�v�`�b�v
 */
#include "k2EngineLowPreCompile.h"
#include "MapChipRender.h"
#include "LevelRender.h"

namespace nsK2EngineLow {

	MapChipRender::MapChipRender(const LevelObjectData& objData, const char* filePath)
	{
		//���f���̃t�@�C���p�X���擾�B
		m_filePath = std::make_unique<const char*>(filePath);

		//�}�b�v�`�b�v�f�[�^��ǉ�����B
		AddMapChipData(objData);
	}

	void MapChipRender::AddMapChipData(const LevelObjectData& objData)
	{
		MapChipData mapChipData;
		mapChipData.position = objData.position;
		mapChipData.rotation = objData.rotation;
		mapChipData.scale = objData.scale;
		m_mapChipDataVector.push_back(mapChipData);
	}

	void MapChipRender::Init()
	{
		
			m_modelRender.Init(*m_filePath.get(),true);
			m_modelRender.SetCasterShadow(false);
			auto& mapChipData = m_mapChipDataVector[0];
			m_modelRender.SetTRS(mapChipData.position, mapChipData.rotation, mapChipData.scale);
			m_modelRender.Update();
			auto p = std::make_unique<PhysicsStaticObject>();
			//�ÓI�����I�u�W�F�N�g���쐬�B
			p->CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
			m_physicsStaticObjectPtrVector.push_back(std::move(p));
			return;
	}

	void MapChipRender::Update()
	{
		m_modelRender.Update();
	}
	void MapChipRender::Draw(RenderContext& rc)
	{
		m_modelRender.Draw(rc);
	}
}
