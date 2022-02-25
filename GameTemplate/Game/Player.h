#pragma once


class Player : public IGameObject
{
public:
	////////////////////////////////////
	// メンバ関数
	////////////////////////////////////
	Player();
	~Player();

	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() //const
	{
		return m_position;
	}
	/// <summary>
	/// ジャンプ回数を設定。
	/// </summary>
	/// <param name="secondjump">ジャンプ。</param>
	void PlusSECONDJUMP()
	{
		secondjump++;
	}
	/// <summary>
	/// ジャンプ回数を取得。
	/// </summary>
	/// <returns>ジャンプ。</returns>
	int GetSECONDJUMP()
	{
		return secondjump;
	}

	Vector3 m_moveSpeed;	                         //移動速度。

private:
	/// <summary>
	///移動処理。
	/// <summary>
	void Move();
	/// <summary>
	///回転処理。
	/// <summary>
	void Turn();
	/// <summary>
	///ステート管理。
	/// <summary>
	void ManageState();
	/// <summary>
	///アニメーションの再生。
	/// <summary>
	void PlayAnimation();


	// メンバ変数
	enum EnAnimationClip {		//アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_run,	//走りアニメーション。
		enAnimationClip_Jump,
		enAnimationClip_Num,
	};
	SoundSource* jumpse;
	ModelRender m_modelRender;
	AnimationClip animationClips[enAnimationClip_Num];
	Vector3 m_position;						           //座標。
	CharacterController m_charaCon;		             //キャラクターコントローラー。
	Quaternion m_rotation;                          //クォータニオン
	int playerState = 0;	                            //プレイヤーのステート(状態)を表す変数。
	int secondjump = 0;                           //
};
