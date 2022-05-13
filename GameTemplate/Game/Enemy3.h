#pragma once

#include "EnemyPath.h"

//クラス宣言。
class Player;
class Game;

/// <summary>
/// エネミー。
/// </summary>
class Enemy3 : public IGameObject
{
public:
	//エネミーステート。
	enum EnEnemy3State {
		enEnemy3State_Idle,					//待機。
		enEnemy3State_Chase,				//追跡。
		enEnemy3State_Attack,				//攻撃。
		enEnemy3State_ReceiveDamage,		//被ダメージ。
		enEnemy3State_Down,					//ダウン。
		enEnemy3State_model
	};
public:
	Enemy3();
	~Enemy3();
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
	/// 騎士の番号を設定する。
	/// </summary>
	/// <param name="kisiNumber">騎士の番号。</param>
	void SetenemyNumber(const int kisiNumber)
	{
		m_enemyNumber = kisiNumber;
	}
	/// <summary>
	/// 騎士の番号を取得する。
	/// </summary>
	/// <returns>騎士の番号。</returns>
	const int GetenemyNumber() const
	{
		return m_enemyNumber;
	}

private:
	/// <summary>
	/// 追跡処理。
	/// </summary>
	void Chase();
	/// <summary>
	/// 回転処理。
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃処理。
	/// </summary>
	void Attack();
	/// <summary>
	/// プレイヤーの攻撃との当たり判定処理。
	/// </summary>
	void Collision();
	/// <summary>
	/// プレイヤーを探索する。
	/// </summary>
	/// <returns>プレイヤーが見つかったらtrue。</returns>
	void SearchPlayer();
	/// <summary>
	/// 攻撃用の当たり判定コリジョンを作成する。
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// アニメーションの再生。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// アニメーションイベント用の関数。
	/// </summary>
	/// <param name="clipName">アニメーションの名前。</param>
	/// <param name="eventName">アニメーションイベントのキーの名前。</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// 各ステートの遷移処理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移処理。
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理。
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 歩きステートの遷移処理。
	/// </summary>
	void ProcessWalkStateTransition();
	/// <summary>
	/// 追跡ステートの背遷移処理。
	/// </summary>
	void ProcessChaseStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理。
	/// </summary>
	void ProcessAttackStateTransition();
	/// 被ダメージステートの遷移処理。
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理。
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack() const;

	/// <summary>
	/// 経路A
	/// </summary>
	void Aroute();

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
		enAnimationClip_Num,					//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	CharacterController			m_charaCon;									//キャラコン。
	EnEnemy3State				m_Enemy3State = enEnemy3State_Idle;			//エネミーステート。
	EffectEmitter* m_effectEmitter = nullptr;			//エフェクト。							//画像。
	bool						m_isUnderAttack = false;					//攻撃中か？
	float							m_hp = 5;									//HP。
	Player* m_player = nullptr;												//プレイヤー。
	Game* m_game = nullptr;
	float						m_chaseTimer = 0.0f;						//追跡タイマー。
	float						m_idleTimer = 0.0f;							//待機タイマー。
	float						a = 0.0f;			//拡大率
	bool						m_isShowHPBar = false;
	bool model = false;
	bool					m_isSearchPlayer = false;
	int                     m_Hand= -1;                   //「Hand」ボーンのID。
	SphereCollider			m_sphereCollider;
	int state = 0;
	int m_enemyNumber = 0;
	EnemyPath m_enemypath;
	EnemyPath m_enemypath2;
	EnemyPath m_enemypath3;
	EnemyPath m_enemypath4;
	Point* m_point;
	Point* m_point2;
	Point* m_point3;
	Point* m_point4;
	Vector3						Weak;
};



