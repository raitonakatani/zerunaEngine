#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

	bool PostEffect::Start()
	{
		ModelRender m;

		// �e�탂�f��������������
		// �w�i���f����������
		ModelInitData bgModelInitData;
		bgModelInitData.m_tkmFilePath = m.a;
		bgModelInitData.m_fxFilePath = "Assets/shader/3D.fx";

		// �w�i���f����������
		Model bgModel;
		bgModel.Init(bgModelInitData);

		// �v���C���[���f����������
		ModelInitData plModelInitData;
		plModelInitData.m_tkmFilePath = m.a;
		plModelInitData.m_fxFilePath = "Assets/shader/3D.fx";
		Model plModel;
		plModel.Init(plModelInitData);

		return true;
	}
}