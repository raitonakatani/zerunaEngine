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
		/// �X�V�B
		/// </summary>
		void Update();
		/// <summary>
		/// <summary>
		/// �����_�����O�p�C�v���C�������s
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g�B</param>
	//	void Execute(RenderContext& rc);
		/// <summary>

		void SpriteRenderDraw(RenderContext& rc);
	private:
		std::vector<SpriteRender*>m_Spriterenders;
	};

	extern RenderingEngine g_renderEngine;
}
