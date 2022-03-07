#pragma once
// プレイヤークラス。
class Player : public IGameObject
{
public:
	/// <summary>
	/// プレイヤーステート
	/// </summary>
	enum EnPlayerState
	{
		enPlayerState_Idle,                //待機ステート
		enPlayerState_Walk,                //歩きステート
		enPlayerState_Run,                 //走りステート
		enPlayerState_StealthySteps,       //忍び足ステート
		enPlayerState_ReceiveDamage,       //被ダメージステート
		enPlayerState_Down,                //ダウンステート
		enPlayerState_Avoidance,           //回避ステート
		enPlayerState_FirstAttack,         //１撃目の攻撃ステート
		enPlayerState_SecondAttack,        //２撃目の攻撃ステート
		enPlayerState_ThirdAttack,         //３撃目の攻撃ステート
		enPlayerState_PokeAttack           //突き攻撃ステート
	};

	/// <summary>
	/// 段階攻撃ステート
	/// </summary>
	enum EnAttackState
	{
		enAttackState_FirstAttack,        //１撃目
		enAttackState_SecondAttack,       //２撃目
		enAttackState_ThirdAttack         //３撃目
	};

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

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
	/// 連続攻撃処理
	/// </summary>
	void Hit();
	/// <summary>
	/// 攻撃の当たり判定を作成する処理
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// 回避処理
	/// </summary>
	void Avoidance();
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
	/// 忍び足ステートの遷移処理
	/// </summary>
	void ProcessStealthyStepsStateTransition();
	/// <summary>
	/// 回避ステートの遷移処理
	/// </summary>
	void ProcessAvoidanceStateTransition();
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

private:
	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	       //待機アニメーション
		enAnimClip_Walk,           //歩きアニメーション
		enAnimClip_Run,		       //走りアニメーション
		enAnimClip_StealthySteps,  //忍び足アニメーション
		enAnimClip_Rolling,        //回転回避アニメーション
		enAnimClip_FirstAttack,    //１撃目の攻撃アニメーション
		enAnimClip_SecondAttack,   //２撃目の攻撃アニメーション
		enAnimClip_ThirdAttack,    //３撃目の攻撃アニメーション
		enAnimClip_PokeAttack,     //突き攻撃アニメーション
		enAnimClip_ReceiveDamage,  //被ダメージアニメーション
		enAnimClip_Down,           //ダウンアニメーション
		enAnimClip_Num,		       //アニメーションの数
	};

	ModelRender				m_modelRender;                              //モデルレンダー
	CharacterController     m_charaCon;	                                //キャラコン
	EnPlayerState           m_playerState = enPlayerState_Idle;         //プレイヤーステート
	EnAttackState           m_attackState = enAttackState_FirstAttack;  //攻撃ステート
	Animation				m_animation;				                //アニメーション
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //アニメーションクリップ
	Vector3					m_position;					                //座標
	Vector3					m_scale = g_vec3One;		                //拡大率
	Vector3                 m_forward = Vector3::AxisZ;                 //前方向のベクトル
	Vector3                 m_moveSpeed;                                //移動速度
	Skeleton                m_skeleton;	                                //スケルトン
	Quaternion				m_rotation;					                //回転
	float                   m_secondAttackTimer = 0.0f;                 //２撃目の攻撃タイマー
	float                   m_thirdAttackTimer = 0.0f;                  //３撃目の攻撃タイマー
	bool                    m_secondAttackFlag = false;                 //２撃目の攻撃フラグ
	bool                    m_thirdAttackFlag = false;                  //３撃目の攻撃フラグ
	bool                    m_isUnderAttack = false;                    //攻撃中か？
	int                     m_swordBoneId = -1;                         //ソードボーンID
	int                     m_hp = 1;
};