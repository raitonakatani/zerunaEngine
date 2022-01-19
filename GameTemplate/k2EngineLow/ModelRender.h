#pragma once

namespace nsK2EngineLow {

	class ModelRender
	{
	public:

		ModelRender();
		~ModelRender();

		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ);

		void Draw(RenderContext& rc);

		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <remark>
		/// インスタンシング描画が有効の場合は、この設定は無視されます。
		/// </remark>
		/// <param name="pos">座標。</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// 座標を設定。
		/// </summary>
		/// <param name="x">x座標</param>
		/// <param name="y">y座標</param>
		/// <param name="z">z座標</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x, y, z });
		}
		/// <summary>
		/// 回転を設定。
		/// </summary>
		/// <remark>
		/// インスタンシング描画が有効の場合は、この設定は無視されます。
		/// </remark>
		/// <param name="rotation">回転。</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}

		/// <summary>
		/// 拡大率を設定。
		/// </summary>
		/// <remark>
		/// インスタンシング描画が有効の場合は、この設定は無視されます。
		/// </remark>
		/// <param name="scale">拡大率。</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}
		void SetScale(float x, float y, float z)
		{
			SetScale({ x, y, z });
		}

		void Update();

		/// <summary>
		/// スケルトンの初期化。
		/// </summary>
		/// <param name="filePath">ファイルパス。</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// アニメーションの初期化。
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		/// <param name="enModelUpAxis">モデルの上向き。</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips,
			EnModelUpAxis enModelUpAxis);

		/// <summary>
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}

		/// <summary>
		/// モデルを取得。
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			return m_model;
		}


	private:
		Model						m_model;
		Skeleton					m_skeleton;							// スケルトン
		float						m_animationSpeed = 1.0f;
		AnimationClip*				m_animationClips = nullptr;			// アニメーションクリップ。
		int							m_numAnimationClips = 0;			// アニメーションクリップの数。
		Animation					m_animation;						// アニメーション。
		ModelInitData				m_InitData;
		Vector3 					m_position = Vector3::Zero;			// 座標。
		Quaternion	 				m_rotation = Quaternion::Identity;	// 回転。
		Vector3						m_scale = Vector3::One;				// 拡大率。
	//	CharacterController			m_charaCon;
	};


}