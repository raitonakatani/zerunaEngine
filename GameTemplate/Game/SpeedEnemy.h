#pragma once

#include "EnemyPath.h"

class Player;
class box;
class Game;

/// <summary>
/// スピードエネミークラス
/// </summary>
class SpeedEnemy :public IGameObject
{
public:
	/// <summary>
	/// スピードエネミーステート
	/// </summary>
	enum EnSpeedEnemyState
	{
		enSpeedEnemyState_Idle,           //待機ステート
		enSpeedEnemyState_Chase,          //追跡ステート
		enSpeedEnemyState_Attack,         //攻撃ステート
		enSpeedEnemyState_Shout,          //叫びステート
		enSpeedEnemyState_ReceiveDamage,  //被ダメージステート
		enSpeedEnemyState_Down,           //ダウンステート
		enSpeedEnemyState_Death,					//即死アニメーション。
		enSpeedEnemyState_alert,
	};

public:
	SpeedEnemy();
	~SpeedEnemy();
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
/// 回転を設定する。
/// </summary>
/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// HPを取得する
	/// </summary>
	/// <returns>HP</returns>
	const int& GetHp() const
	{
		return m_hp;
	}

	/// <summary>
	/// 騎士の番号を設定する。
	/// </summary>
	/// <param name="kisiNumber">騎士の番号。</param>
	void SetspeedNumber(const int kisiNumber)
	{
		m_speedNumber = kisiNumber;
	}
	/// <summary>
	/// 騎士の番号を取得する。
	/// </summary>
	/// <returns>騎士の番号。</returns>
	const int GetspeedNumber() const
	{
		return m_speedNumber;
	}

private:
	/// <summary>
	/// アニメーションの初期化
	/// </summary>
	void InitAnimation();
	/// <summary>
	/// 追跡処理
	/// </summary>
	void Chase();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// 攻撃用の当たり判定を作成
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
	/// 追跡ステートの遷移処理
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 叫びステートの遷移処理
	/// </summary>
	void ProcessShoutStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition();

	void alertTransition();
//	void lookTransition();

	/// <summary>
	/// プレイヤーを発見したか？
	/// </summary>
	/// <returns>発見したらtrue</returns>
	void SearchPlayer();
	/// <summary>
	/// プレイヤーを攻撃できるか？
	/// </summary>
	/// <returns>攻撃できるならtrue</returns>
	const bool IsCanAttack() const;

	/// <summary>
	/// 経路A
	/// </summary>
	void Aroute();

private:
	/// <summary>
	/// アニメーションクリップの番号を表す列挙型
	/// </summary>
	enum EnAnimationClip
	{
		enAnimClip_Idle,           //待機アニメーション
		enAnimClip_Run,            //走りアニメーション
		enAnimClip_Attack,         //攻撃アニメーション
		enAnimClip_Shout,          //叫びアニメーション
		enAnimClip_ReceiveDamage,  //被ダメージステート
		enAnimClip_Down,           //ダウンステート
		enAnimationClip_Death,					//即死アニメーション。
		enAnimationClip_alert,
		enAnimClip_Num             //アニメーションの数
	};
	
	Game*					m_game;										//ゲーム
	box*					m_box;										//警戒、模様
	Player*					m_player = nullptr;                         //プレイヤー
	Point*					m_point[14];								//ポイント
	Animation				m_animation;				                //アニメーション
	AnimationClip           m_animationClipArray[enAnimClip_Num];	    //アニメーションクリップ
	CharacterController     m_charaCon;                                 //キャラコン
	EnemyPath				m_enemypath[14];							//パス
	EnSpeedEnemyState       m_speedEnemyState = enSpeedEnemyState_Idle; //スピードエネミーステート
	ModelRender             m_modelRender;                              //モデルレンダー
	SphereCollider			m_sphereCollider;
	Vector3                 m_position;                                 //座標
	Vector3                 m_scale = Vector3::One;                     //拡大率
	Vector3                 m_forward = Vector3::AxisZ;                 //前方向のベクトル
	Vector3                 m_moveSpeed;                                //移動速度
	Vector3					Weak;
	Vector3					targetpos;
	Quaternion              m_rotation;                                 //回転
	int						state = 0;
	int                     m_hp = 100;                                 //HP
	int                     m_punchBoneId = -1;                         //パンチのボーン
	int						alertLevel = 0;								//警戒
	int						m_weakness = -1;							//「m_weakness」ボーンのID。
	int						m_speedNumber = 0;
	int						ab;											//咆哮回数
	int						hakken = 0;									//プレイヤーを見つけた時のサウンド
	int						fasthakkenn = 0;							//最初に見つけた時の処理				
	bool					m_isSearchPlayer = false;
	bool					mikke = false;
	bool                    m_isUnderAttack = false;					//攻撃中か？
	float                   m_idleTimer = 0.0f;                         //待機タイマー
	float                   m_chaseTimer = 0.0f;                        //追跡タイマー
	float					m_angl = 0.40f;
	float					m_range = 1000.0f;
	float					m_timer = 0.0f;
	float					alertTimet = 0.0f;							//拡大率
};

