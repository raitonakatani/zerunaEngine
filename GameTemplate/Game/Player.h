#pragma once

class GameCamera;
class PAUSE;
class Game;
//class Title;

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
		enPlayerState_snake,
		enPlayerState_StealthySteps,       //忍び足ステート
		enPlayerState_ReceiveDamage,       //被ダメージステート
		enPlayerState_Down,                //ダウンステート
		enPlayerState_FirstAttack,         //１撃目の攻撃ステート
		enPlayerState_PokeAttack,           //突き攻撃ステート
		enPlayerState_Win
	};
public:
	/// <summary>
	/// 攻撃ステート
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
	/// indexをプラスする。
	/// </summary>
	void Plusindex()
	{
		index++;
	}
	/// <summary>
	/// indexを取得する。
	/// </summary>
	/// <returns>ポーション。</returns>
	int Getindex()
	{
		return index;
	}

	Vector3                 m_forward = Vector3::AxisZ;                 //前方向のベクトル
	float m_hp = 100.0f;
	float					m_sp = 150.0f;						//sp。

	int critical = 0;
	bool prok = false;
	int m_prokcamera = 0;

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
		enAnimClip_Idle,	       //待機アニメーション
		enAnimClip_Walk,           //歩きアニメーション
		enAnimClip_Run,		       //走りアニメーション
		enAnimClip_snake,
		enAnimClip_StealthySteps,  //忍び足アニメーション
		enAnimClip_FirstAttack,    //１撃目の攻撃アニメーション
		enAnimClip_PokeAttack,     //突き攻撃アニメーション
		enAnimClip_ReceiveDamage,  //被ダメージアニメーション
		enAnimClip_Down,           //ダウンアニメーション
		enAnimClip_Win,
		enAnimClip_Num,		       //アニメーションの数
	};


	int                     m_sword_jointBoneId = -1;                   //「Sword」ボーンのID。
	int                     m_swordBoneId = -1;                   //「Sword」ボーンのID。
	ModelRender				m_modelRender;                              //モデルレンダー
	EnPlayerState           m_playerState = enPlayerState_Idle;         //プレイヤーステート
	int st = 0;
	EnAttackState           m_attackState = enAttackState_FirstAttack;  //攻撃ステート
	Vector3					m_position;					                //座標
	Vector3                 m_moveSpeed;                                //移動速度
	Skeleton                m_skeleton;	                                //スケルトン
	Quaternion				m_rotation;					                //回転
	Vector3					m_scale = g_vec3One;		                //拡大率
	Animation				m_animation;				                //アニメーション
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //アニメーションクリップ
	CharacterController     m_charaCon;	                                //キャラコン
	bool                    m_isUnderAttack = false;                    //攻撃中か？
	EffectEmitter*			m_effectEmitter = nullptr;					//エフェクト。
	EffectEmitter*			m_effectEmitter2 = nullptr;					//エフェクト。
	PAUSE*					m_pause = nullptr;
	Game* m_game;
//	Title* m_title;
	SpriteRender			m_HPRender;							        //画像。
	SpriteRender			m_staminaRender;
	SpriteRender			m_senseRender;
	SpriteRender			m_HPberRender;						        //画像。
	SpriteRender			m_stmnberRender;
	SpriteRender			m_senseberRender;
	SpriteRender			m_spriteRender3;
	float					life = 100.0f;								//フルHPバー
	float					m_sutamina = 200.0f;						//スタミナ。
	float					m_hurusutamina = 200.0f;					//フルスタミナ。
//	float					m_sp = 500.0f;								//sp。
	float					m_hurusp = 700.0f;							//フルsp。
	float					cooltime = 0.0f;
	bool					m_menu = false;
	bool					COOLtime = false;
	int						index = 0;
	float					m_alpha = 0.0f;								//pressbuttonのα値。]
	float					criticalAlpha = 0.0f;
	float					criticalscale = 2.0f;
	bool					m_down = false;
	float					m_downtimer = 0.0f;
	int						m_Hitse = 0;
	int						m_deathse = 0;
	float					m_startwalk = 0.0f;

};

