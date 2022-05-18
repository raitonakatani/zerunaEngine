#include "k2EngineLowPreCompile.h"
#include "FontRender.h"

namespace nsK2EngineLow {
	void FontRender::Draw(RenderContext& rc)
	{
		if (m_text == nullptr)
		{
			return;
		}
		g_renderingEngine.AddRenderObject(this);
	}
}