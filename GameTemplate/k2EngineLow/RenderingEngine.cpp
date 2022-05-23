#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"

namespace nsK2EngineLow
{
	RenderingEngine g_renderingEngine;
	void RenderingEngine::Render2DDraw(RenderContext& rc)
	{
		for (auto& renderObj : m_renderobject) {
			renderObj->OnRenderFont2D(rc);
		}
		for (auto& renderObj : m_renderobject) {
			renderObj->OnRenderSprite2D(rc);
		}


	}

	void RenderingEngine::ShadowMapDraw(RenderContext& rc)
	{
		Vector3 dir = { g_Light.GetDirectionLightDirection().x,g_Light.GetDirectionLightDirection().y,g_Light.GetDirectionLightDirection().z };
		m_shadowMapRender.Render(rc, dir, m_renderobject);
	}
	void RenderingEngine::Execute(RenderContext& rc)
	{
		m_modelRenderCB.m_light = g_Light.GetLight();
		m_modelRenderCB.mlvp = GetLightCamera().GetViewProjectionMatrix();
		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_engine->ExecuteRender();
	//	ShadowMapDraw(rc);
		
		m_postEffect->Render(rc);

		Render2DDraw(rc);
		m_renderobject.clear();
	}
	void RenderingEngine::Init()
	{
		m_postEffect = &g_postEffect;
		m_shadowMapRender.Init();
		m_modelRenderCB.m_light = g_Light.GetLight();
		m_modelRenderCB.mlvp = GetLightCamera().GetViewProjectionMatrix();

	}
}