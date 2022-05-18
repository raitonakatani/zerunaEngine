#pragma once

namespace nsK2EngineLow {
	class touka
	{
		touka(){}
		~touka() {}

		void Init();
		void Draw(RenderContext& rc);


	private:
		RenderTarget albedRT;
		RenderTarget normalRT;
		RenderTarget worldPosRT;
		SpriteInitData spriteInitData;
		Sprite defferdLightinSpr;

	};
//	extern touka g_touka;
}