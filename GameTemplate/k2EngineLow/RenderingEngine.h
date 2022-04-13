#pragma once

//#include "SpriteRender.h"

namespace nsK2EngineLow {

	class RenderingEngine : public Noncopyable
	{
	public:
		///
		/// 
		/// 
		/// 
		void AddRenderObject(SpriteRender* renderObject)
		{
			m_Spriterenders.push_back(renderObject);
		}

		/// <summary>
		/// 更新。
		/// </summary>
		void Update();
		/// <summary>
		/// <summary>
		/// レンダリングパイプラインを実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト。</param>
	//	void Execute(RenderContext& rc);
		/// <summary>

		void SpriteRenderDraw(RenderContext& rc);
	private:
		std::vector<SpriteRender*>m_Spriterenders;
	};

	extern RenderingEngine g_renderEngine;
}
