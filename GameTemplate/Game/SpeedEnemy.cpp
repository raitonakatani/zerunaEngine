#include "stdafx.h"
#include "SpeedEnemy.h"

#include "Player.h"

#include "CollisionObject.h"

namespace
{
	const float CHARACON_RADIUS = 20.0f;            //キャラコンの半径
	const float CHARACON_HEIGHT = 45.0f;             //キャラコンの高さ
	const float MODEL_POSITION_Y = 2.5f;            //モデルのY座標
	const float MOVESPEED_MINIMUMVALUE = 0.001f;    //移動速度の最低値
	const float MOVESPEED = 200.0f;                 //移動速度
	const float GRAVITY = 1000.0f;                  //重力
	const float IDLETIMER_MAX = 0.3f;               //待機タイマーの最大値
	const float IDLETIMER_DEFAULT = 0.0f;           //待機タイマーの初期値
	const float CHASETIMER_MAX = 1.0f;              //追跡タイマーの最大値
	const float CHASETIMER_DEFAULT = 0.0f;          //追跡タイマーの初期値
	const float SEARCH_RANGE = 300.0f;              //プレイヤーを発見できる距離
	const float SEARCH_ANGLE = 130.0f;              //プレイヤーを発見できる角度
	const float ATTACK_RANGE = 45;					//攻撃できる距離
	const int   ATTACK_PROBABILITY = 1;             //攻撃を行う確率
	const float RECEIVE_DAMAGE = 1;                 //プレイヤーから受けるダメージ
}

SpeedEnemy::SpeedEnemy()
{

}

SpeedEnemy::~SpeedEnemy()
{

}

void SpeedEnemy::InitAnimation()
{
	//アニメーションをロード
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/speedenemy/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/speedenemy/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Attack].Load("Assets/animData/speedenemy/attack.tka");
	m_animationClipArray[enAnimClip_Attack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Shout].Load("Assets/animData/speedenemy/shout.tka");
	m_animationClipArray[enAnimClip_Shout].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/speedenemy/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/speedenemy/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
}

bool SpeedEnemy::Start()
{
	//アニメーションの初期化
	InitAnimation();

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm",
		m_animationClipArray,
		enAnimClip_Num);

	//キャラコンの初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//パンチのボーン
	m_punchBoneId = m_modelRender.FindBoneID(L"LeftHand");

	m_player = FindGO<Player>("player");

	m_enemypath.Init("Assets/path/speed/enemypath1.tkl");
	m_enemypath2.Init("Assets/path/speed/enemypath2.tkl");
	m_enemypath3.Init("Assets/path/speed/enemypath3.tkl");
	m_enemypath4.Init("Assets/path/speed/enemypath4.tkl");
	
	m_point = m_enemypath.GetFirstPoint();
	m_point2 = m_enemypath2.GetFirstPoint();
	m_point3 = m_enemypath3.GetFirstPoint();
	m_point4 = m_enemypath4.GetFirstPoint();

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void SpeedEnemy::Update()
{
	//追跡処理
	Chase();
	//回転処理
	Rotation();
	//当たり判定処理
	Collision();
	//攻撃処理
	Attack();
	//アニメーションの再生
	PlayAnimation();
	//各ステートの遷移処理
	ManageState();

	//座標と拡大率と回転を設定する
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);

	Weak = m_player->GetPosition() - m_position;

	//モデルの更新
	m_modelRender.Update();
}

void SpeedEnemy::Chase()
{
	//プレイヤーを見つけていなかったら。
	if (state == 1)
	{
		m_speedEnemyState = enSpeedEnemyState_Chase;
		Aroute();
	}

	//追跡ステートでないなら、追跡処理はしない。
	if (m_speedEnemyState != enSpeedEnemyState_Chase)
	{
		//何もしない
		return;
	}

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}

	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += MODEL_POSITION_Y;
	m_modelRender.SetPosition(modelPosition);
}

void SpeedEnemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < MOVESPEED_MINIMUMVALUE
		&& fabsf(m_moveSpeed.z) < MOVESPEED_MINIMUMVALUE) {
		//m_moveSpeed.xとm_moveSpeed.zの絶対値がともに0.001以下ということは
		//このフレームではキャラは移動していないので旋回する必要はない。
		return;
	}
	//atan2はtanθの値を角度(ラジアン単位)に変換してくれる関数。
	//m_moveSpeed.x / m_moveSpeed.zの結果はtanθになる。
	//atan2を使用して、角度を求めている。
	//これが回転角度になる。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atanが返してくる角度はラジアン単位なので
	//SetRotationDegではなくSetRotationを使用する。
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの前ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

//衝突したときに呼ばれる関数オブジェクト(壁用)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//衝突フラグ。

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//壁とぶつかってなかったら。
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//衝突したのは壁ではない。
			return 0.0f;
		}

		//壁とぶつかったら。
		//フラグをtrueに。
		isHit = true;
		return 0.0f;
	}
};

void SpeedEnemy::Collision()
{
	//被ダメージステートとダウンステートだったら
	if (m_speedEnemyState == enSpeedEnemyState_ReceiveDamage ||
		m_speedEnemyState == enSpeedEnemyState_Down)
	{
		//何もしない
		return;
	}

	//プレイヤーの攻撃の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//プレイヤーの攻撃とキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= RECEIVE_DAMAGE;
			//HPが0だったら
			if (m_hp == 0)
			{
				//ダウンステートに移行する
				m_speedEnemyState = enSpeedEnemyState_Down;
			}
			//HPが0ではなかったら
			else
			{
				//被ダメージステートに移行する
				m_speedEnemyState = enSpeedEnemyState_ReceiveDamage;
			}
		}
	}
}

void SpeedEnemy::Attack()
{
	//攻撃ステートではなかったら
	if (m_speedEnemyState != enSpeedEnemyState_Attack)
	{
		//何もしない
		return;
	}
	//攻撃中だったら
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		//MakeAttackCollision();
	}
}

void SpeedEnemy::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 100.0f;
	collisionPosition.y += 100.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,				  //座標。
		Quaternion::Identity,                                     //回転。
		Vector3(30.0f, 30.0f, 30.0f)                              //大きさ。
	);

	Matrix matrix = m_modelRender.GetBone(m_punchBoneId)->GetWorldMatrix();
	//剣のボーンのワールド行列をコリジョンに適用させる
	collisionObject->SetName("speedenemy_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void SpeedEnemy::PlayAnimation()
{
	switch (m_speedEnemyState)
	{
		//待機ステートの時
	case SpeedEnemy::enSpeedEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.6f);
		break;
		//追跡ステートの時
	case SpeedEnemy::enSpeedEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
		m_modelRender.SetAnimationSpeed(1.4f);
		break;
		//攻撃ステートの時
	case SpeedEnemy::enSpeedEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimClip_Attack, 0.2f);
		m_modelRender.SetAnimationSpeed(3.0f);
		break;
		//叫びステートの時
	case SpeedEnemy::enSpeedEnemyState_Shout:
		m_modelRender.PlayAnimation(enAnimClip_Shout, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//被ダメージステートの時
	case SpeedEnemy::enSpeedEnemyState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//ダウンステートの時
	case SpeedEnemy::enSpeedEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.2f);
		m_modelRender.SetAnimationSpeed(1.5f);
		break;
		//それ以外の時
	default:
		break;
	}
}

void SpeedEnemy::ManageState()
{
	switch (m_speedEnemyState)
	{
		//待機ステートの時
	case SpeedEnemy::enSpeedEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//追跡ステートの時
	case SpeedEnemy::enSpeedEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//攻撃ステートの時
	case SpeedEnemy::enSpeedEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
		//叫びステートの時
	case SpeedEnemy::enSpeedEnemyState_Shout:
		ProcessShoutStateTransition();
		break;
		//被ダメージステートの時
	case SpeedEnemy::enSpeedEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時
	case SpeedEnemy::enSpeedEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//それ以外の時
	default:
		break;
	}
}

void SpeedEnemy::ProcessCommonStateTransition()
{
	//待機タイマーと追跡タイマーを初期化する
	m_idleTimer = IDLETIMER_DEFAULT;
	m_chaseTimer = CHASETIMER_DEFAULT;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを発見したら
	if (m_isSearchPlayer == true && diff.LengthSq() <= 500.0 * 500.0f)
	{
		state = 0;
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 150.0f;

		Vector3 toPlayerDir = diff;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);

		//攻撃できるなら
		if (IsCanAttack() == true)
		{
			int ram = rand() % 100;
			if (ram >= ATTACK_PROBABILITY)
			{
				//攻撃ステートに移行する
				m_speedEnemyState = enSpeedEnemyState_Attack;
				//m_isUnderAttack = false;
				return;
			}
			else
			{
				//待機ステートに移行する
				m_speedEnemyState = enSpeedEnemyState_Idle;
				return;
			}
		}
		//攻撃できないなら
		else
		{
			//追跡ステートに移行する
			m_speedEnemyState = enSpeedEnemyState_Chase;
			return;
		}
	}
	//プレイヤーを発見できなかったら
	else
	{
		state = 1;
	}
}

void SpeedEnemy::ProcessIdleStateTransition()
{
	//待機タイマーをカウントさせる
	m_idleTimer += g_gameTime->GetFrameDeltaTime();

	//待機タイマーが0.7fより大きかったら
	if (m_idleTimer >= IDLETIMER_MAX)
	{
		//共通のステートの遷移処理に移行する
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessChaseStateTransition()
{
	//攻撃できる距離なら
	if (IsCanAttack() == true)
	{
		//共通のステートの遷移処理に移行する
		ProcessCommonStateTransition();
		return;
	}

	//追跡タイマーをカウントさせる
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();

	//追跡タイマーが1.0fより大きかったら
	if (m_chaseTimer >= CHASETIMER_MAX)
	{
		//共通のステートの遷移処理に移行する
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わっていたら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステートの遷移処理に移行する
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessShoutStateTransition()
{
	//アニメーションの再生が終わっていたら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステートの遷移処理に移行する
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessReceiveDamageStateTransition()
{
	//アニメーションの再生が終わっていたら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//共通のステートの遷移処理に移行する
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessDownStateTransition()
{
	//アニメーションの再生が終わっていたら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//自身を削除する
		DeleteGO(this);
		return;
	}
}

void SpeedEnemy::SearchPlayer()
{
	m_isSearchPlayer = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));

	//プレイヤーが視界内に居なかったら。
	if (Math::PI * 0.45f <= fabsf(angle))
	{
		//プレイヤーは見つかっていない。
		return;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 70.0f, playerPosition.z));

	SweepResultWall callback;
	//コライダーを始点から終点まで動かして。
	//衝突するかどうかを調べる。
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//壁と衝突した！
	if (callback.isHit == true)
	{
		//プレイヤーは見つかっていない。
		return;
	}

	//壁と衝突してない！！
	//プレイヤー見つけたフラグをtrueに。
	m_isSearchPlayer = true;
}

const bool SpeedEnemy::IsCanAttack() const
{
	//プレイヤーに向かうベクトル
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= ATTACK_RANGE * ATTACK_RANGE)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}

void SpeedEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//攻撃フラグをtrueにする
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//攻撃フラグをfalseにする
		m_isUnderAttack = false;
	}
}

void SpeedEnemy::Render(RenderContext& rc)
{
	if (Weak.Length() <= 3000.0f)
	{
		//モデルの描画
		m_modelRender.Draw(rc);
	}
}

///経路
void SpeedEnemy::Aroute()
{

	if (m_speedNumber == 0)
	{
		Vector3 diff = m_point->s_position - m_position;
		if (diff.Length() <= 50.0f) {
			if (m_point->s_number == m_enemypath.GetPointListSize() - 1) {
				m_point = m_enemypath.GetFirstPoint();
			}
			else {
				m_point = m_enemypath.GetNextPoint(m_point->s_number);
			}

		}

		Vector3 range = m_point->s_position - m_position;
		m_moveSpeed = range * 20.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}

	if (m_speedNumber == 1)
	{
		Vector3 diff = m_point2->s_position - m_position;
		if (diff.Length() <= 50.0f) {
			if (m_point2->s_number == m_enemypath2.GetPointListSize() - 1) {
				m_point2 = m_enemypath2.GetFirstPoint();
			}
			else {
				m_point2 = m_enemypath2.GetNextPoint(m_point2->s_number);
			}

		}
		Vector3 range = m_point2->s_position - m_position;
		m_moveSpeed = range * 20.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}
	if (m_speedNumber == 2)
	{
		Vector3 diff = m_point3->s_position - m_position;
		if (diff.Length() <= 50.0f) {
			if (m_point3->s_number == m_enemypath3.GetPointListSize() - 1) {
				m_point3 = m_enemypath3.GetFirstPoint();
			}
			else {
				m_point3 = m_enemypath3.GetNextPoint(m_point3->s_number);
			}

		}

		Vector3 range = m_point3->s_position - m_position;
		m_moveSpeed = range * 20.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}
	if (m_speedNumber == 3)
	{
		Vector3 diff = m_point4->s_position - m_position;
		if (diff.Length() <= 50.0f) {
			if (m_point4->s_number == m_enemypath4.GetPointListSize() - 1) {
				m_point4 = m_enemypath4.GetFirstPoint();
			}
			else {
				m_point4 = m_enemypath4.GetNextPoint(m_point4->s_number);
			}

		}

		Vector3 range = m_point4->s_position - m_position;
		m_moveSpeed = range * 10.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}

}