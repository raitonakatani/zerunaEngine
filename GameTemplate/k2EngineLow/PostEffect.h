#pragma once

namespace nsK2EngineLow {

	class PostEffect
	{
	public:
		PostEffect() {}
		~PostEffect() {}

		void Init();
		void SpriteInit();
		void maint();
		void Render();

		RenderTarget mainRenderTarget;

		RenderTarget luminnceRenderTarget;

		SpriteInitData luminanceSpriteInitData;
		Sprite luminanceSprite;
		GaussianBlur gaussianBlur;
		SpriteInitData finalSpriteInitData;
		Sprite finalSprite;
		SpriteInitData spriteInitData;
		Sprite copyToFrameBufferSprite;
	};

	extern PostEffect g_posteffect;

}