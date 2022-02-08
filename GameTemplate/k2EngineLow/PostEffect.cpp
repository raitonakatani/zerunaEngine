#include "k2EngineLowPreCompile.h"
#include "PostEffect.h"

namespace nsK2EngineLow {

	PostEffect g_posteffect;

	void PostEffect::Init()
	{
	mainRenderTarget.Create(
		1600,
		900,
		1,
		1,
		//【注目】カラーバッファのフォーマットを32bit浮動小数点にしている。
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
	}
}