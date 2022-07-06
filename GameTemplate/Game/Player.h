#pragma once

class PAUSE;
class Game;

// プレイヤークラス。
class Player : public IGameObject
{
public:
	/// <summary>
	/// プレイヤーステート
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,					//待機ステート
		enPlayerState_Walk,					//歩きステート
		enPlayerState_Run,					//走りステート
		enPlayerState_snake,				//しゃがみステート
		enPlayerState_StealthySteps,		//忍び足ステート
		enPlayerState_ReceiveDamage,		//被ダメージステート
		enPlayerState_Down,					//ダウンステート
		enPlayerState_FirstAttack,			//切り攻撃ステート
		enPlayerState_PokeAttack,			//突き攻撃ステート
		enPlayerState_Win					//クリアステート
	};
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 宝箱取得数を設定する。
	/// <param name="index">宝箱所持数。</param>
	void SetPlayerHaveIndex(const int& index)
	{
		m_index = index;
	}
	/// <summary>
	/// 宝箱所持数を取得する。
	/// </summary>
	/// <returns>宝箱所持数。</returns>
	int GetPlayerHaveIndex()
	{
		return m_index;
	}
	/// <summary>
	///	ヒットポイントの最大値を取得する。
	/// </summary>
	/// <returns>ヒットポイント。</returns>
	float GetMaxHP()
	{
		return m_maxHP;
	}
	/// <summary>
	/// スタミナポイントの最大値を取得する。
	/// </summary>
	/// <returns>スタミナポイント。</returns>
	float GetMaxStamina()
	{
		return m_maxStamina;
	}
	/// <summary>
	/// スキルポイントの最大値を取得する。
	/// </summary>
	/// <returns>スキルポイント。</returns>
	float GetMaxSP()
	{
		return m_maxSP;
	}
	/// <summary>
	/// スタートタイマーを取得する。
	/// </summary>
	/// <returns>スタートタイマー。</returns>
	float GetStartTimer()
	{
		return m_startTimer;
	}
	/// <summary>
	/// クリティカル攻撃のヒット数を取得する。
	/// </summary>
	/// <returns>クリティカル。</returns>
	int GetHitCritical()
	{
		return m_critical;
	}
	/// <summary>
	/// クリティカル攻撃のヒット数を設定する。
	/// </summary>
	/// <param name="critical">クリティカルのヒット数。</param>
	void SetHitCritical(const int& critical)
	{
		m_critical = critical;
	}
	/// <summary>
	/// メニュー画面の状態を設定する。
	/// </summary>
	/// <returns>メニュー。</returns>
	bool IsOpenMenu()
	{
		return m_menu;
	}
	/// <summary>
	/// 現在のHPを取得する。
	/// </summary>
	/// <returns>現在のヒットポイント。</returns>
	float GetHP()
	{
		return m_hp;
	}
	/// <summary>
	/// 現在のSPを取得する。
	/// </summary>
	/// <returns>現在のスキルポイント。</returns>
	float GetSP()
	{
		return m_sp;
	}


	bool					prok = false;						//クリティカル攻撃の処理判定。
	bool					m_down = false;								//死亡判定。
	int						st = 0;										//プレイヤーステート

private:
	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	///	攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// 攻撃の当たり判定を作成する処理
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーション名前イベント
	/// </summary>
	/// <param name="clipName">アニメーションの名前</param>
	/// <param name="eventName">アニメーションキーの名前</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// 共通のステートの遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 歩きステートの遷移処理
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 走りステートの遷移処理
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// しゃがみステートの遷移処理
	/// </summary>
	void ProcesssnakeStateTransition();
	/// <summary>
	/// 忍び足ステートの遷移処理
	/// </summary>
	void ProcessStealthyStepsStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// クリアステートの遷移処理
	/// </summary>
	void ProcessWinStateTransition();

	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,			//待機アニメーション
		enAnimClip_Walk,			//歩きアニメーション
		enAnimClip_Run,				//走りアニメーション
		enAnimClip_snake,			//しゃがみアニメーション
		enAnimClip_StealthySteps,	//忍び足アニメーション
		enAnimClip_FirstAttack,		//切り攻撃アニメーション
		enAnimClip_PokeAttack,		//突き攻撃アニメーション
		enAnimClip_ReceiveDamage,	//被ダメージアニメーション
		enAnimClip_Down,			//ダウンアニメーション
		enAnimClip_Win,				//クリアアニメーション
		enAnimClip_Num,				//アニメーションの数
	};

	// クラス IGameObject
	Game*					m_game = nullptr;							// ゲーム。
	PAUSE*					m_pause = nullptr;							// ポーズ。
	// クラス Engine
	EffectEmitter*			m_effectEmitter = nullptr;					// エフェクト。
	EffectEmitter*			m_effectEmitter2 = nullptr;					// エフェクト。
	ModelRender				m_modelRender;                              // モデルレンダー
	CharacterController     m_charaCon;	                                // キャラコン
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    // アニメーションクリップ
	// enum
	EnPlayerState           m_playerState = enPlayerState_Idle;         // プレイヤーステート
	// 関数
	Vector3                 m_forward = Vector3::AxisZ;                 // 前方向のベクトル
	Vector3					m_position;					                // 座標
	Vector3                 m_moveSpeed;                                // 移動速度
	Vector3					m_scale = g_vec3One;		                // 拡大率
	Quaternion				m_rotation;					                // 回転
	int                     m_swordJointBoneId = -1;					//「Sword」ボーンのID。
	int                     m_swordBoneId = -1;							//「Sword」ボーンのID。
	int						m_hitSE = 0;								// 被ダメージの効果音。
	int						m_deathSE = 0;								// ダウン時の効果音。
	int						m_critical = 0;								// クリティカル攻撃の処理判定。
	int						m_index = 0;								// 宝箱。
	bool					m_menu = false;								// メニュー
	bool					m_cooltime = false;							// クールタイム。
	bool                    m_isUnderAttack = false;                    // 攻撃中か？
	float					m_hp = 100.0f;								// HP。
	float					m_stamina = 200.0f;							// スタミナ。
	float					m_sp = 200.0f;								// SP。
	float					m_maxHP = 100.0f;							// フルHP
	float					m_maxStamina = 200.0f;						// フルスタミナ。
	float					m_maxSP = 700.0f;							// フルSP。
	float					m_downTimer = 0.0f;							// 死亡してからの時間。
	float					m_startTimer = 0.0f;						// ゲーム開始時の処理。
};