#pragma once

namespace nsK2EngineLow {

	class SpriteRender
	{
	public:
		void Init(const char* filePath,const float w,const float h,AlphaBlendMode alphaBlendMode);
		void Draw(RenderContext& rc);
		
		void OnRender2D(RenderContext& rc)
		{
			m_sprite.Draw(rc);
		}

		Sprite m_sprite;

	};

}