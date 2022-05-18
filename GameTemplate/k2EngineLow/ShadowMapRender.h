#pragma once
namespace nsK2EngineLow {
	class ShadowMapRender
	{
	public:
		void Init();

		void Render(RenderContext& rc, Vector3 ligDirection, std::vector< IRenderer* >& renderObjects);

		Texture& GetShadowMap()
		{
			
			return shadowMap.GetRenderTargetTexture();
		}
		
		Camera& GetLightCamera()
		{
			return m_lightCamera;
		}

	private:
		RenderTarget shadowMap;
		Camera m_lightCamera;
	};

}