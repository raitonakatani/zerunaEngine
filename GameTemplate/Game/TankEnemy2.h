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
class TankEnemy;
class EnemyPath;
class box;

/// <summary>
/// エネミー。
/// </summary>
class TankEnemy2 : public IGameObject
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
	TankEnemy2();
	~TankEnemy2();
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

	Vector3 tankPosi;
	float					m_hp = 20;									//HP。


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
		enAnimationClip_look,					//咆哮アニメーション。
		enAnimationClip_alert,					//警戒アニメーション
		enAnimationClip_Num,					//アニメーションの数。
	};
	AnimationClip			m_animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	ModelRender				m_modelRender;								//モデルレンダー。
	Vector3					m_position;									//座標。
	Vector3					m_moveSpeed;								//移動速度。
	Vector3					m_forward = Vector3::AxisZ;					//エネミーの正面ベクトル。
	Quaternion				m_rotation;									//回転。
	Vector3					m_scale = Vector3::One;						//大きさ。
	CharacterController		m_charaCon;									//キャラコン。
	EnEnemyState			m_EnemyState = enEnemyState_Idle;			//エネミーステート。
	EffectEmitter* m_effectEmitter = nullptr;					//エフェクト。
	bool					m_isUnderAttack = false;					//攻撃中か？
	bool					m_isSearchPlayer = false;					//見るけているか？
	Player* m_player = nullptr;							//プレイヤー。
	TankEnemy* m_tank = nullptr;							//タンク
	Game* m_game = nullptr;							//ゲーム
	GameCamera* m_camera;									//カメラ
	box* m_box;										//警戒、模様
	EnemyPath				m_enemypath2;								//パス
	EnemyPath				m_enemypath4;								//パス
	Point* m_point2;									//ポイント
	Point* m_point4;									//ポイント
	SpriteRender			alertSprite;								//警戒レベル
	float					m_chaseTimer = 0.0f;						//追跡タイマー。
	float					m_idleTimer = 0.0f;							//待機タイマー。
	float					alertTimet = 0.0f;							//拡大率
	int						m_tankNumber = 0;							//「タンク」ナンバー
	int						m_Hand = -1;								//「Hand」ボーンのID。
	int						m_weakness = -1;							//「m_weakness」ボーンのID。
	SphereCollider			m_sphereCollider;							//スフィアコライダー
	int						state = 0;									//警戒ステート
	int						alertLevel = 0;								//警戒レベル
	float					m_timer = 0.0f;								//タイマー
	Vector3					Weak;										//プレイヤーとの距離
	int						ab;											//咆哮回数
	float					m_angl = 0.40f;								//視野角
	float					m_range = 1000.0f;							//視力（見える距離）
	Vector3					targetpos;									//最後に見たプレイヤーのポジション
	bool					mikke = false;								//見失った敵を追いかける。
	int						hakken = 0;									//プレイヤーを見つけた時のサウンド
	int						fasthakkenn = 0;							//最初に見つけた時の処理
};
