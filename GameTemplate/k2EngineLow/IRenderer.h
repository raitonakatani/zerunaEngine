#pragma once


namespace nsK2EngineLow {
	/// <summary>
	/// レンダーラーの基底クラス。
	/// </summary>
	class IRenderer : public Noncopyable {
	public:
		/// <summary>
		/// シャドウマップへの描画パスから呼ばれる処理。
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="ligNo">ライト番号</param>
		/// <param name="lvpMatrix">ライトビュープロジェクション行列</param>
		virtual void OnRenderShadowMap(
			RenderContext& rc,
			const Matrix& lvpMatrix
		)
		{

		}
		
		/// <summary>
		/// フォント描画。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRenderFont2D(RenderContext& rc)
		{

		}
		/// <summary>
		/// スプライト描画。
		/// </summary>
		/// <param name="rc"></param>
		virtual void OnRenderSprite2D(RenderContext& rc)
		{

		}
		/// <summary>
		/// シャドウキャスター？
		/// </summary>
		/// <returns>trueが帰ってくるとシャドウキャスター</returns>
		virtual bool IsShadowCaster() const
		{
			return false;
		}
	};
}