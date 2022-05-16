#pragma once

#include "tkFile/TknFile.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include "AI/PathFinding/PathFinding.h"
#include "EnemyPath.h"

//クラス宣言。
class Player;
class Game;
class GameCamera;
class TankEnemy2;
class EnemyPath;

	/// <summary>
	/// エネミー。
	/// </summary>
class TankEnemy : public IGameObject
{
public:
	//エネミーステート。
	enum EnEnemyState {
		enEnemyState_Idle,					//待機。
		enEnemyState_Chase,				//追跡。
		enEnemyState_Attack,				//攻撃。
		enEnemyState_ReceiveDamage,		//被ダメージ。
		enEnemyState_Down,
		enEnemyState_Death,//ダウン。
		enEnemyState_look,//警報。
		enEnemyState_alert,//警戒。
		enEnemyState_model
	};
public:
	TankEnemy();
	~TankEnemy();
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
	/// 敵の番号を設定する。
	/// </summary>
	/// <param name="kisiNumber">敵の番号。</param>
	void SettankNumber(const int kisiNumber)
	{
		m_tankNumber = kisiNumber;
	}
	/// <summary>
	/// 敵の番号を取得する。
	/// </summary>
	/// <returns>敵の番号。</returns>
	const int GettankNumber() const
	{
		return m_tankNumber;
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

	void lookTransition();

	void alertTransition();

	/// <summary>
	/// 攻撃できる距離かどうか調べる。
	/// </summary>
	/// <returns>攻撃できるならtrue。</returns>
	const bool IsCanAttack() const;


	/// <summary>
	/// 経路A
	/// </summary>
	void Aroute();
	/// <summary>
	/// 経路B
	/// </summary>
	void Broute();

	enum EnAnimationClip {						//アニメーション。
		enAnimationClip_Idle,					//待機アニメーション。
		enAnimationClip_Walk,					//歩きアニメーション。
		enAnimationClip_Attack,					//攻撃アニメーション。
		enAnimationClip_Damage,					//被ダメージアニメーション。
		enAnimationClip_Down,					//ダウンアニメーション。
		enAnimationClip_Death,					//即死アニメーション。
		enAnimationClip_look,					//アニメーション。
		enAnimationClip_alert,
		enAnimationClip_Num,					//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender					m_modelRender;								//モデルレンダー。
	Vector3						m_position;									//座標。
	Vector3						m_firstPosition;							//最初の座標。
	Vector3						m_moveSpeed;								//移動速度。
	Vector3						m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion					m_rotation;									//回転。
	Vector3						m_scale = Vector3::One;						//大きさ。
	//Vector3					m_moveVector;
	Vector3						m_toPlayer;
	SoundSource*				SE;
	CharacterController			m_charaCon;									//キャラコン。
	EnEnemyState				m_EnemyState = enEnemyState_Idle;			//エネミーステート。
	EffectEmitter*				m_effectEmitter = nullptr;			//エフェクト。							//画像。
	bool						m_isUnderAttack = false;					//攻撃中か？
	bool						m_isSearchPlayer = false;
	float						m_hp = 20;									//HP。
	Player*						m_player = nullptr;												//プレイヤー。
	TankEnemy2*					m_tank2 = nullptr;
	Game*						m_game = nullptr;
	GameCamera*					m_camera;
	//EnemyPath					m_pathposi;
	EnemyPath					m_enemypath;
	EnemyPath					m_enemypath3;
	EnemyPath					m_enemypath5;
	EnemyPath					m_enemypath6;
	EnemyPath					m_enemypath7;
	Point*						m_point;
	Point*						m_point3;
	Point*						m_point5;
	Point*						m_point6;
	Point*						m_point7;
	SpriteRender				alertSprite;				//警戒レベル
	float						m_chaseTimer = 0.0f;		//追跡タイマー。
	float						m_idleTimer = 0.0f;			//待機タイマー。
	float						alertTimet = 0.0f;					//拡大率
	bool						m_isShowHPBar = false;
	int							m_tankNumber = 0;
	int							m_Hand = -1;				//「Hand」ボーンのID。
	int							m_weakness = -1;			//「m_weakness」ボーンのID。
	SphereCollider				m_sphereCollider;
	int							state = 0;
	int							Bstate = 0;
	int							alertLevel = 0; //警戒
	float						m_timer = 0.0f;
	float						m_timer2 = 0.0f;
	Vector3						Weak;
	int							cooltime = 0;
	int							ab;
	float m_angl =0.40f;
	float m_range =1000.0f;
	Vector3 targetpos;
	bool mikke;
	float huntertimer = 0.0f;

private:
	TknFile m_tknFile;
	nsAI::NaviMesh m_nvmMesh;
	nsAI::Path m_path;
	nsAI::PathFinding m_pathFiding;
};
