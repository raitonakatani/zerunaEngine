#include "stdafx.h"
#include "Player.h"

#include "CollisionObject.h"

namespace
{
	const float CHARACON_RADIUS = 20.0f;            //キャラコンの半径
	const float CHARACON_HEIGHT = 60.0f;            //キャラコンの高さ
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;   //移動速度の最低値
	const float WALK_MOVESPEED = 60.0f;            //歩きステートの移動速度
	const float RUN_MOVESPEED = 160.0f;            //走りステートの移動速度
	const float STEALTHYSTEP_MOVESPEED = 30.0f;     //忍び足ステートの移動速度
	const float GRAVITY = 1000.0f;                  //重力
	const float ATTACK_TIME = 1.5f;                 //連続攻撃ができる時間
	const float AVOIDANCE_SPEED = 100.0f;           //回避ステートの移動速度
}

void Player::InitAnimation()
{
	//アニメーションクリップをロードする。
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/player2/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/player2/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/player2/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_StealthySteps].Load("Assets/animData/player2/stealthysteps.tka");
	m_animationClipArray[enAnimClip_StealthySteps].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Rolling].Load("Assets/animData/player2/rolling.tka");
	m_animationClipArray[enAnimClip_Rolling].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/firstattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_SecondAttack].Load("Assets/animData/player2/secondattack.tka");
	m_animationClipArray[enAnimClip_SecondAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ThirdAttack].Load("Assets/animData/player2/thirdattack.tka");
	m_animationClipArray[enAnimClip_ThirdAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_PokeAttack].Load("Assets/animData/player2/pokeattack.tka");
	m_animationClipArray[enAnimClip_PokeAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/player2/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/player2/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
}

bool Player::Start()
{
	//アニメーションを初期化。
	InitAnimation();

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/player/player2.tkm"
		, m_animationClipArray, enAnimClip_Num
	);

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//キャラコン
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, g_vec3Zero);

	//剣のボーン
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");
	return true;
}

void Player::Update()
{
	//移動処理
	Move();
	//回転処理
	Rotation();
	//当たり判定処理
	Collision();
	//アニメーションの再生
	PlayAnimation();
	//各ステートの遷移処理
	ManageState();
	//攻撃処理
	Attack();
	//連続攻撃処理
	Hit();
	//回避処理
	Avoidance();

	//座標、回転、大きさの更新
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale({ 1.5f,1.5f,1.5 });
	
	//モデルの更新
	m_modelRender.Update();
}

void Player::Move()
{
	//待機ステート、歩きステート、走りステート以外だったら
	if (m_playerState != enPlayerState_Run &&
		m_playerState != enPlayerState_Walk &&
		m_playerState != enPlayerState_StealthySteps &&
		m_playerState != enPlayerState_Idle)
	{
		//なにもしない
		return;
	}
	//移動速度
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//ステックの入力量を取得
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//cameraの前方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//走りステートだったら
	if (m_playerState == enPlayerState_Run)
	{
		m_moveSpeed += cameraForward * lStick_y * RUN_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * RUN_MOVESPEED;
	}

	else if(m_playerState == enPlayerState_StealthySteps){
		m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
	}
	//それ以外だったら
	else {
		m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;
	}

	//重力
	//m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

	//キャラコンを使用して、座標を更新する
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//座標の更新
	Vector3 modelPosition = m_position;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Rotation()
{
	if (fabsf(m_moveSpeed.x) < MOVE_SPEED_MINIMUMVALUE
		&& fabsf(m_moveSpeed.z) < MOVE_SPEED_MINIMUMVALUE) {
		return;
	}
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);
	//回転を設定する
	m_modelRender.SetRotation(m_rotation);
	//プレイヤーの正面ベクトルを計算する
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	//被ダメージステートだったら
	if (m_playerState == enPlayerState_ReceiveDamage || m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Avoidance)
	{
		//何もしない
		return;
	}

	//スピードエネミーの攻撃の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("speedenemy_attack");
	for (auto collision : collisions)
	{
		//スピードエネミーの攻撃とキャラコンが衝突したら
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 1;
			//HPが0だったら
			if (m_hp == 0)
			{
				//ダウンステートに移行する
				m_playerState = enPlayerState_Down;
			}
			//HPが0ではなかったら
			else
			{
				//被ダメージステートに移行する
				m_playerState = enPlayerState_ReceiveDamage;
			}
		}
	}
}

void Player::Attack()
{
	//攻撃ステートではないなら
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_SecondAttack &&
		m_playerState != enPlayerState_ThirdAttack)
	{
		//攻撃処理をしない
		return;
	}

	//攻撃判定中であれば。
	if (m_isUnderAttack == true)
	{
		//MakeAttackEffect();
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
}

void Player::Hit()
{
	//1撃目の攻撃ステートだったら
	if (m_playerState == enPlayerState_FirstAttack)
	{
		//2撃目の攻撃フラグをtrueにする
		m_secondAttackFlag = true;
	}
	//2撃目の攻撃ステートだったら
	if (m_playerState == enPlayerState_SecondAttack)
	{
		//3撃目の攻撃フラグがtrueにする
		m_thirdAttackFlag = true;
	}

	//２撃目の攻撃フラグがtrueだったら
	if (m_secondAttackFlag == true)
	{
		//２撃目の攻撃タイマーを増加させる
		m_secondAttackTimer += g_gameTime->GetFrameDeltaTime();
		//２撃目の攻撃タイマーが開始されていたら
		if (m_secondAttackTimer > 0.001f && m_secondAttackTimer < ATTACK_TIME)
		{
			//２撃目の攻撃ステートへ移行する
			m_attackState = enAttackState_SecondAttack;

		}
		else
		{
			//１撃目の攻撃ステートへ移行する
			m_attackState = enAttackState_FirstAttack();
			//２撃目の攻撃タイマーを初期化する
			m_secondAttackTimer = 0.0f;
			//２撃目の攻撃フラグをfalseにする
			m_secondAttackFlag = false;
		}
	}
	//３撃目の攻撃フラグがtrueだったら
	if (m_thirdAttackFlag == true)
	{
		//３撃目の攻撃タイマーを増加させる
		m_thirdAttackTimer += g_gameTime->GetFrameDeltaTime();
		//３撃目の攻撃タイマーが動いていたら
		if (m_thirdAttackTimer > 0.001f && m_thirdAttackTimer < ATTACK_TIME)
		{
			//３撃目の攻撃ステートへ移行する
			m_attackState = enAttackState_ThirdAttack;
		}
		else
		{
			//１撃目の攻撃ステートへ移行する
			m_attackState = enAttackState_FirstAttack();
			//２撃目の攻撃タイマーと３撃目の攻撃タイマーを初期化する
			m_secondAttackTimer = 0.0f;
			m_thirdAttackTimer = 0.0f;
			//２撃目の攻撃フラグと３撃目の攻撃フラグをfalseにする
			m_secondAttackFlag = false;
			m_thirdAttackFlag = false;
		}
	}
}

void Player::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//座標をプレイヤーの少し前に設定する。
	collisionPosition += m_forward * 100.0f;
	collisionPosition.y += 100.0f;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,				   //座標。
		Quaternion::Identity,                                      //回転。
		Vector3(10.0f, 10.0f, 60.0f)                              //大きさ。
	);

	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//剣のボーンのワールド行列をコリジョンに適用させる
	collisionObject->SetName("player_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void Player::Avoidance()
{
	//回避ステート以外だったら
	if (m_playerState != enPlayerState_Avoidance)
	{
		//何もしない
		return;
	}
	m_moveSpeed = m_forward * AVOIDANCE_SPEED;
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.5f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//歩きステートの時
	case Player::enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//走りステートの時
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//忍び足ステートの時
	case Player::enPlayerState_StealthySteps:
		m_modelRender.PlayAnimation(enAnimClip_StealthySteps, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//回避ステートの時
	case Player::enPlayerState_Avoidance:
		m_modelRender.PlayAnimation(enAnimClip_Rolling, 0.1f);
		m_modelRender.SetAnimationSpeed(1.5f);
		break;
		//1撃目の攻撃ステートの時
	case Player::enPlayerState_FirstAttack:
		m_modelRender.PlayAnimation(enAnimClip_FirstAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//２撃目の攻撃ステートの時
	case Player::enPlayerState_SecondAttack:
		m_modelRender.PlayAnimation(enAnimClip_SecondAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.4f);
		break;
		//３撃目の攻撃ステートの時
	case Player::enPlayerState_ThirdAttack:
		m_modelRender.PlayAnimation(enAnimClip_ThirdAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.6f);
		break;
		//突き攻撃ステートの時
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//被ダメージステートの時
	case Player::enPlayerState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//ダウンステートの時
	case Player::enPlayerState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
	default:
		break;
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
		//歩きステートの時
	case Player::enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
		//走りステートの時
	case Player::enPlayerState_Run:
		ProcessRunStateTransition();
		break;
		//忍び足ステートの時
	case Player::enPlayerState_StealthySteps:
		ProcessStealthyStepsStateTransition();
		break;
		//回避ステートの時
	case Player::enPlayerState_Avoidance:
		ProcessAvoidanceStateTransition();
		break;
		//1撃の攻撃ステートの時
	case Player::enPlayerState_FirstAttack:
		ProcessAttackStateTransition();
		break;
		//2撃目の攻撃ステートの時
	case Player::enPlayerState_SecondAttack:
		ProcessAttackStateTransition();
		break;
		//3撃目の攻撃ステートの時
	case Player::enPlayerState_ThirdAttack:
		ProcessAttackStateTransition();
		break;
		//突き攻撃ステートの時
	case Player::enPlayerState_PokeAttack:
		ProcessAttackStateTransition();
		break;
		//被ダメージステートの時
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	default:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	//Bボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//回避ステートへ移行する
		m_playerState = enPlayerState_Avoidance;
		return;
	}

	//RB1ボタンが押されたら＆攻撃ステート１だったら
	if (g_pad[0]->IsTrigger(enButtonRB1) && m_attackState == enAttackState_FirstAttack)
	{
		//１撃目の攻撃ステートに移行する
		m_playerState = enPlayerState_FirstAttack;

		return;
	}

	//RB1ボタンが押されたら＆攻撃ステート２だったら
	if (g_pad[0]->IsTrigger(enButtonRB1) && m_attackState == enAttackState_SecondAttack)
	{
		//２撃目の攻撃ステートに移行する
		m_playerState = enPlayerState_SecondAttack;
		return;
	}

	//RB1ボタンが押されたら＆攻撃ステート３だったら
	if (g_pad[0]->IsTrigger(enButtonRB1) && m_attackState == enAttackState_ThirdAttack)
	{
		//３撃目の攻撃ステートに移行する
		m_playerState = enPlayerState_ThirdAttack;
		return;
	}

	//RB2ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonRB2))
	{
		//突き攻撃ステートに移行する
		m_playerState = enPlayerState_PokeAttack;
		return;
	}

	//xかzの移動速度があったら
	else if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		//Aボタンが押されたら
		if (g_pad[0]->IsPress(enButtonA))
		{
			//走りステートへ移行する
			m_playerState = enPlayerState_Run;
			return;
		}
		//Xボタンが押されたら
		if (g_pad[0]->IsPress(enButtonX))
		{
			//忍び足ステートへ移行する
			m_playerState = enPlayerState_StealthySteps;
			return;
		}
		//どのボタンも押されなかったら
		else
		{
			//歩きステートへ移行する
			m_playerState = enPlayerState_Walk;
			return;
		}
	}

	//xかzの移動速度がなかったら
	else
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
		return;

	}
}

void Player::ProcessIdleStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
}

void  Player::ProcessWalkStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessStealthyStepsStateTransition()
{
	//他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessAvoidanceStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessAttackStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		DeleteGO(this);
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Player::Render(RenderContext& rc)
{
	//ドロー。
	m_modelRender.Draw(rc);
}