#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

	bool PostEffect::Start()
	{
		ModelRender m;

		// 各種モデルを初期化する
		// 背景モデルを初期化
		ModelInitData bgModelInitData;
		bgModelInitData.m_tkmFilePath = m.a;
		bgModelInitData.m_fxFilePath = "Assets/shader/3D.fx";

		// 背景モデルを初期化
		Model bgModel;
		bgModel.Init(bgModelInitData);

		// プレイヤーモデルを初期化
		ModelInitData plModelInitData;
		plModelInitData.m_tkmFilePath = m.a;
		plModelInitData.m_fxFilePath = "Assets/shader/3D.fx";
		Model plModel;
		plModel.Init(plModelInitData);

		return true;
	}
}