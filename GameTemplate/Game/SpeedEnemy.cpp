#include "stdafx.h"
#include "SpeedEnemy.h"

#include "Player.h"

#include "CollisionObject.h"

namespace
{
	const float CHARACON_RADIUS = 20.0f;            //キャラコンの半径
	const float CHARACON_HEIGHT = 45.0f;             //キャラコンの高さ
	const float MODEL_POSITION_Y = 10.0f;            //モデルのY座標
	const float MOVESPEED_MINIMUMVALUE = 0.001f;    //移動速度の最低値
	const float MOVESPEED = 200.0f;                 //移動速度
	const float GRAVITY = 1000.0f;                  //重力
	const float IDLETIMER_MAX = 0.3f;               //待機タイマーの最大値
	const float IDLETIMER_DEFAULT = 0.0f;           //待機タイマーの初期値
	const float CHASETIMER_MAX = 0.8f;              //追跡タイマーの最大値
	const float CHASETIMER_DEFAULT = 0.0f;          //追跡タイマーの初期値
	const float SEARCH_RANGE = 300.0f;              //プレイヤーを発見できる距離
	const float SEARCH_ANGLE = 130.0f;              //プレイヤーを発見できる角度
	const float ATTACK_RANGE = 100;					//攻撃できる距離
	const int   ATTACK_PROBABILITY = 0;             //攻撃を行う確率
	const float RECEIVE_DAMAGE = 50;                 //プレイヤーから受けるダメージ
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
	m_animationClipArray[enAnimationClip_alert].Load("Assets/animData/speedenemy/alert.tka");
	m_animationClipArray[enAnimationClip_alert].SetLoopFlag(false);
}

bool SpeedEnemy::Start()
{
	//アニメーションの初期化
	InitAnimation();

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm",
		false,
		m_animationClipArray,
		enAnimClip_Num);
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_position.y = 15.0f;
	//キャラコンの初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//パンチのボーン
	m_punchBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");

	m_player = FindGO<Player>("player");

	m_enemypath.Init("Assets/path/speed/enemypath1.tkl");
	m_enemypath2.Init("Assets/path/speed/enemypath2.tkl");
	m_enemypath3.Init("Assets/path/speed/enemypath3.tkl");
	m_enemypath4.Init("Assets/path/speed/enemypath4.tkl");
	m_enemypath5.Init("Assets/path/speed/enemypath5.tkl");
	m_enemypath6.Init("Assets/path/speed/enemypath6.tkl");
	m_enemypath7.Init("Assets/path/speed/enemypath7.tkl");
	m_enemypath8.Init("Assets/path/speed/enemypath8.tkl");
	m_enemypath9.Init("Assets/path/speed/enemypath9.tkl");
	m_enemypath10.Init("Assets/path/speed/enemypath10.tkl");
	m_enemypath11.Init("Assets/path/speed/enemypath11.tkl");
	m_enemypath12.Init("Assets/path/speed/enemypath12.tkl");
	m_enemypath13.Init("Assets/path/speed/enemypath13.tkl");
	
	m_point = m_enemypath.GetFirstPoint();
	m_point2 = m_enemypath2.GetFirstPoint();
	m_point3 = m_enemypath3.GetFirstPoint();
	m_point4 = m_enemypath4.GetFirstPoint();
	m_point5 = m_enemypath5.GetFirstPoint();
	m_point6 = m_enemypath6.GetFirstPoint();
	m_point7 = m_enemypath7.GetFirstPoint();
	m_point8 = m_enemypath8.GetFirstPoint();
	m_point9 = m_enemypath9.GetFirstPoint();
	m_point10 = m_enemypath10.GetFirstPoint();
	m_point11 = m_enemypath11.GetFirstPoint();
	m_point12 = m_enemypath12.GetFirstPoint();
	m_point13 = m_enemypath13.GetFirstPoint();

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);

	alertSprite.Init("Assets/sprite/alert.dds", 64, 64);
	//表示する座標を設定する。
	alertSprite.SetPosition({ 0.0f,0.0f ,0.0f });

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void SpeedEnemy::Update()
{
	Weak = m_player->GetPosition() - m_position;
	if (Weak.Length() >= 3000.0f)
	{
		return;
	}

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
	//サーチ
	SearchPlayer();


	if (alertLevel == 0)
	{
		alertSprite.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		m_angl = 0.40f;
		m_range = 1000.0f;
	}
	if (alertLevel == 1)
	{
		alertSprite.SetMulColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
		m_angl = 0.45f;
		m_range = 1200.0f;
	}
	if (alertLevel == 2)
	{
		alertSprite.SetMulColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		m_angl = 0.50f;
		m_range = 1500.0f;
	}
	if (alertLevel == 3)
	{
		alertSprite.SetMulColor(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
		m_angl = 0.5f;
		m_range = 1500.0f;
	}

	if (state == 0 && m_isSearchPlayer == false) {

		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 0.5f) {
			m_speedEnemyState = enSpeedEnemyState_Idle;
		}
		if (m_timer >= 0.5f) {
			m_speedEnemyState = enSpeedEnemyState_Chase;
			state = 1;
		}
	}

	Vector3 diff = m_player->GetPosition() - m_position;

	//ベクトルの長さが700.0fより小さかったら。
	if (diff.LengthSq() <= 1500.0f * 1500.0f && m_hp > 0)
	{
		//ワールド座標に変換。
		//座標をエネミーの少し上に設定する。
		Vector4 worldPos = Vector4(m_position.x, m_position.y + 250.0f, m_position.z, 1.0f);

		Matrix matrix;
		matrix.Multiply(g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());

		matrix.Apply(worldPos);

		//カメラのビュー行列を掛ける。
		//カメラ座標に変換。
		worldPos.x = (worldPos.x / worldPos.w);
		worldPos.y = (worldPos.y / worldPos.w);

		//カメラのプロジェクション行列を掛ける。
		//スクリーン座標に変換。
		worldPos.x *= FRAME_BUFFER_W / 2;
		worldPos.y *= FRAME_BUFFER_H / 2;

		//ポジションの設定。
		alertSprite.SetPosition(Vector3(worldPos.x, worldPos.y, 0.0f));
	}
	else {
		alertSprite.SetPosition({ 2000.0f,2000.0f,0.0f });
	}
	alertSprite.Update();

	//モデルの更新
	m_modelRender.Update();
}

void SpeedEnemy::Chase()
{
	//プレイヤーを見つけていなかったら。
	if (state == 1)
	{
		if (Weak.Length() <= 800.0f && m_player->st == 1 || Weak.Length() <= 800.0f && m_player->st == 2)
		{
			m_speedEnemyState = enSpeedEnemyState_alert;

			if (alertLevel == 0) {
				alertLevel = 1;
			}
			//	if (alertLevel == 3)
			//	{
			//		return;
			//	}
		}
		else if (alertLevel == 0 || alertLevel == 3) {
			m_speedEnemyState = enSpeedEnemyState_Chase;
			Aroute();
		}
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
	m_modelRender.SetPosition(m_position);

	//Vector3 modelPosition = m_position;
	////ちょっとだけモデルの座標を挙げる。
	//modelPosition.y += MODEL_POSITION_Y;
	//m_modelRender.SetPosition(modelPosition);
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

	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
//	Matrix matrix = m_modelRender.GetBone(m_weakness)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 50.0f, 50.0f));
//	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy");


	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_porkattack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(collisionObject))
		{
			//state = 2;
			//m_camera->m_camera = 0;
			//ダウンステートに遷移する。
			m_speedEnemyState = enSpeedEnemyState_Down;
			return;
		}
	}

	//プレイヤーの攻撃の当たり判定
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision2 : collisions2)
	{
		//プレイヤーの攻撃とキャラコンが衝突したら
		if (collision2->IsHit(m_charaCon))
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
		MakeAttackCollision();
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
	collisionObject->SetName("Speedenemy_attack");
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
		m_modelRender.SetAnimationSpeed(1.6f);
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
		//警戒ステートの時。
	case SpeedEnemy::enSpeedEnemyState_alert:
		//警戒アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_alert, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
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
		//警戒ステートの時。
	case SpeedEnemy::enSpeedEnemyState_alert:
		//警戒ステートのステート遷移処理。
		alertTransition();
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

	//プレイヤーを見つけたら。
	if (m_isSearchPlayer == true && diff.LengthSq() <= m_range * m_range)
	{
		state = 0;
		m_timer = 0.0f;
		alertTimet = 0.0f;
		alertLevel = 2;
		mikke = true;
		targetpos = m_player->GetPosition();

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
				m_speedEnemyState = enSpeedEnemyState_Attack;
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
	else if (mikke == true) {
		Vector3 diff2 = targetpos - m_position;
		if (diff2.LengthSq() <= 500.0f * 500.0f * g_gameTime->GetFrameDeltaTime()) {
			if (m_isSearchPlayer == false) {
				mikke = false;
			}
		}

		//エネミーからプレイヤーに向かうベクトルを計算する。
		Vector3 diff1 = targetpos - m_position;
	//	huntertimer += g_gameTime->GetFrameDeltaTime();
		//ベクトルを正規化する。
		diff1.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff1 * 200.0f;

		Vector3 toPlayerDir = diff1;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);
	}
	//プレイヤーを見つけられなければ。
	else
	{
		if (alertLevel == 2)
		{
			alertLevel = 3;
		}
		else {
			alertLevel = 0;
		}
		//	alertLevel = 0;
		//	alertTimet += g_gameTime->GetFrameDeltaTime();
		//	if (alertTimet >= 10.0f)
		//	{
		//		alertLevel = 0;
		//	}
		return;
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

	//追跡タイマーが0.8fより大きかったら
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
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_speedEnemyState = enSpeedEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 250.0f;
	}
}

void SpeedEnemy::ProcessDownStateTransition()
{
	m_position.y = 15.0f;
	m_modelRender.SetPosition(m_position);
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_hp = 0;
		state = 3;
		m_charaCon.RemoveRigidBoby();
	}
	////アニメーションの再生が終わっていたら
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{
	//	//自身を削除する
	//	DeleteGO(this);
	//	return;
	//}
}

void SpeedEnemy::alertTransition()
{
	if (m_modelRender.IsPlayingAnimation() == true)
	{
		//エネミーからプレイヤーに向かうベクトルを計算する。
		Vector3 diff = m_player->GetPosition() - m_position;
		if (m_isSearchPlayer == true && diff.LengthSq() <= m_range * m_range)
		{
			ProcessCommonStateTransition();
		}
	}

	//警報アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (alertLevel == 1) {
			alertLevel = 0;
		}
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

//void SpeedEnemy::lookTransition()
//{
//	//警報アニメーションの再生が終わったら。
//	if (m_modelRender.IsPlayingAnimation() == false)
//	{
//		//	alertLevel = 0;
//			//他のステートに遷移する。
//		ProcessCommonStateTransition();
//	}
//}

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
	if (Math::PI * m_angl <= fabsf(angle))
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


///経路
void SpeedEnemy::Aroute()
{

	if (m_speedNumber == 0)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point->s_number == m_enemypath.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point = m_enemypath.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point = m_enemypath.GetNextPoint(m_point->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point->s_position.x, m_point->s_position.y + 70.0f, m_point->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point = m_enemypath.GetNextPoint(m_point->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}

	if (m_speedNumber == 1)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point2->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point2->s_number == m_enemypath2.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point2 = m_enemypath2.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point2 = m_enemypath2.GetNextPoint(m_point2->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point2->s_position.x, m_point2->s_position.y + 70.0f, m_point2->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point2 = m_enemypath2.GetNextPoint(m_point2->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 2)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point3->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point3->s_number == m_enemypath3.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point3 = m_enemypath3.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point3 = m_enemypath3.GetNextPoint(m_point3->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point3->s_position.x, m_point3->s_position.y + 70.0f, m_point3->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point3 = m_enemypath3.GetNextPoint(m_point3->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 3)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point4->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point4->s_number == m_enemypath4.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point4 = m_enemypath4.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point4 = m_enemypath4.GetNextPoint(m_point4->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point4->s_position.x, m_point4->s_position.y + 70.0f, m_point4->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point4 = m_enemypath4.GetNextPoint(m_point4->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
		if (m_speedNumber == 4)
		{
			//目標地点までのベクトル
			Vector3 diff = m_point5->s_position - m_position;
			//目標地点に近かったら
			if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
			{
				//最後の目標地点だったら
				if (m_point5->s_number == m_enemypath5.GetPointListSize() - 1)
				{
					//最初の目標地点へ
					m_point5 = m_enemypath5.GetFirstPoint();
				}
				//最後の目標地点ではなかったら
				else
				{
					//次の目標地点へ
					m_point5 = m_enemypath5.GetNextPoint(m_point5->s_number);
				}
			}
			else {
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//始点はエネミーの座標。
				start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
				//終点は次のパスの座標。
				end.setOrigin(btVector3(m_point5->s_position.x, m_point5->s_position.y + 70.0f, m_point5->s_position.z));

				SweepResultWall callback;
				//コライダーを始点から終点まで動かして。
				//衝突するかどうかを調べる。
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
				//壁と衝突した！
				if (callback.isHit == true)
				{
					m_point5 = m_enemypath5.GetNextPoint(m_point5->s_number);
					return;
				}
				else {
					//正規化
					diff.Normalize();
					//目標地点に向かうベクトル×移動速度
					m_moveSpeed = diff * 200.0f;
					//Y座標の移動速度を0にする
					m_moveSpeed.y = 0.0f;
				}
			}
		}
	if (m_speedNumber == 5)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point6->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point6->s_number == m_enemypath6.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point6 = m_enemypath6.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point6 = m_enemypath6.GetNextPoint(m_point6->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point6->s_position.x, m_point6->s_position.y + 70.0f, m_point6->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point6 = m_enemypath6.GetNextPoint(m_point6->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 6)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point7->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point7->s_number == m_enemypath7.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point7 = m_enemypath7.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point7 = m_enemypath7.GetNextPoint(m_point7->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point7->s_position.x, m_point7->s_position.y + 70.0f, m_point7->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point7 = m_enemypath7.GetNextPoint(m_point7->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 7)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point8->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point8->s_number == m_enemypath8.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point8 = m_enemypath8.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point8 = m_enemypath8.GetNextPoint(m_point8->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point8->s_position.x, m_point8->s_position.y + 70.0f, m_point8->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point8 = m_enemypath8.GetNextPoint(m_point8->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 8)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point9->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point9->s_number == m_enemypath9.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point9 = m_enemypath9.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point9 = m_enemypath9.GetNextPoint(m_point9->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point9->s_position.x, m_point9->s_position.y + 70.0f, m_point9->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point9 = m_enemypath9.GetNextPoint(m_point9->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 9)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point10->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point10->s_number == m_enemypath10.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point10 = m_enemypath10.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point10 = m_enemypath10.GetNextPoint(m_point10->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point10->s_position.x, m_point10->s_position.y + 70.0f, m_point10->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point10 = m_enemypath10.GetNextPoint(m_point10->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 10)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point11->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point11->s_number == m_enemypath11.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point11 = m_enemypath11.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point11 = m_enemypath11.GetNextPoint(m_point11->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point11->s_position.x, m_point11->s_position.y + 70.0f, m_point11->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point11 = m_enemypath11.GetNextPoint(m_point11->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 11)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point12->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point12->s_number == m_enemypath12.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point12 = m_enemypath12.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point12 = m_enemypath12.GetNextPoint(m_point12->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point12->s_position.x, m_point12->s_position.y + 70.0f, m_point12->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point12 = m_enemypath12.GetNextPoint(m_point12->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 12)
	{
		//目標地点までのベクトル
		Vector3 diff = m_point13->s_position - m_position;
		//目標地点に近かったら
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//最後の目標地点だったら
			if (m_point13->s_number == m_enemypath13.GetPointListSize() - 1)
			{
				//最初の目標地点へ
				m_point13 = m_enemypath13.GetFirstPoint();
			}
			//最後の目標地点ではなかったら
			else
			{
				//次の目標地点へ
				m_point13 = m_enemypath13.GetNextPoint(m_point13->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//始点はエネミーの座標。
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//終点は次のパスの座標。
			end.setOrigin(btVector3(m_point13->s_position.x, m_point13->s_position.y + 70.0f, m_point13->s_position.z));

			SweepResultWall callback;
			//コライダーを始点から終点まで動かして。
			//衝突するかどうかを調べる。
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//壁と衝突した！
			if (callback.isHit == true)
			{
				m_point13 = m_enemypath13.GetNextPoint(m_point13->s_number);
				return;
			}
			else {
				//正規化
				diff.Normalize();
				//目標地点に向かうベクトル×移動速度
				m_moveSpeed = diff * 200.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}
		}
	}

}

void SpeedEnemy::Render(RenderContext& rc)
{
	if (Weak.Length() <= 3000.0f)
	{
		//モデルの描画
		m_modelRender.Draw(rc);
	}

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//始点はエネミーの座標。
	start.setOrigin(btVector3(m_position.x, m_position.y + 150.0f, m_position.z));
	//終点はプレイヤーの座標。
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 150.0f, playerPosition.z));

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
	else {

		if (Weak.Length() <= 1500.0f)
		{
			alertSprite.Draw(rc);
		}
	}
}