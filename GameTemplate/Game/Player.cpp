#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "PAUSE.h"


namespace
{
	const float CHARACON_RADIUS = 25.0f;            //キャラコンの半径
	const float CHARACON_HEIGHT = 100.0f;            //キャラコンの高さ
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;   //移動速度の最低値
	const float WALK_MOVESPEED = 200.0f;            //歩きステートの移動速度
	const float RUN_MOVESPEED = 400.0f;            //走りステートの移動速度
	const float STEALTHYSTEP_MOVESPEED = 100.0f;     //忍び足ステートの移動速度
	const float GRAVITY = 1000.0f;                  //重力
	const float ATTACK_TIME = 1.5f;                 //連続攻撃ができる時間
	const float AVOIDANCE_SPEED = 400.0f;           //回避ステートの移動速度
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
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/slashattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
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

	//「Sword」ボーンのID(番号)を取得する。
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"Sword_joint");

	camera = FindGO<GameCamera>("GameCamera");

	//サウンドを読み込む。
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/11yoroi.wav");

	//キャラコン
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, g_vec3Zero);


	m_HPberRender.Init("Assets/sprite/ft.dds", 1600, 900);
	//表示する座標を設定する。
	m_HPberRender.SetPivot({ 0.0f, 0.5f });
	m_HPberRender.SetPosition({ 0.0f,0.0f ,0.0f });

	m_HPRender.Init("Assets/sprite/HP.dds", 500, 27);
	//表示する座標を設定する。
	m_HPRender.SetPivot({ 0.0f, 0.5f });
	m_HPRender.SetPosition({ 0.0f,0.0f ,0.0f });
	m_HPRender.Update();
	m_stmnberRender.Init("Assets/sprite/ft.dds", 1600, 900);
	//表示する座標を設定する。
	m_stmnberRender.SetPivot({ 0.0f, 0.5f });
	m_stmnberRender.SetPosition({ 0.0f,0.0f ,0.0f });

	m_staminaRender.Init("Assets/sprite/stmn.dds", 500, 27);
	//表示する座標を設定する。
	m_staminaRender.SetPivot({ 0.0f, 0.5f });
	m_staminaRender.SetPosition({ 0.0f,0.0f ,0.0f });
	m_staminaRender.Update();

	m_senseberRender.Init("Assets/sprite/ft.dds", 1600, 900);
	//表示する座標を設定する。
	m_senseberRender.SetPivot({ 0.0f, 0.5f });
	m_senseberRender.SetPosition({ -800.0f,-45.0f ,0.0f });
	m_senseberRender.Update();

	m_senseRender.Init("Assets/sprite/sp.dds", 500, 27);
	//表示する座標を設定する。
	m_senseRender.SetPivot({ 0.0f, 0.5f });
	m_senseRender.SetPosition({ 0.0f,0.0f ,0.0f });
	m_senseRender.Update();

	return true;
}

void Player::Update()
{
	if (g_pad[0]->IsTrigger(enButtonStart)		//Startボタンが押された。
		&& m_menu == false)					//かつm_menu==falseの時。
	{
		m_menu = true;
		m_pause = NewGO<PAUSE>(0, "PASUE");
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		//Startボタンが押された。
		&& m_menu == true)					//かつm_menu==trueの時。 
	{
		m_menu = false;
	}


	if (m_menu == true)
	{
		return;
	}



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
	//回避処理
	Avoidance();

	//座標、回転、大きさの更新
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale({ 2.0f,2.0f,2.0 });
	
	//モデルの更新
	m_modelRender.Update();


	//HPバー
/*	if (m_hp <= 0.0f)
	{
		m_hp = 0.0f;
	}
*/
	if (m_hp >= 100.0f)
	{
		m_hp = 100.0f;
	}

	life = m_hp / 100.0f;
	m_HPRender.SetScale({ life, 1.0f, 0.0f });
//	m_HPRender.SetPivot({ 0.0f, 0.5f });
	m_HPRender.SetPosition({ -748.0f,420.0f ,0.0f });
	m_HPRender.Update();



	if (m_sutamina <= 0.0f)
	{
		m_sutamina = 0.0f;
	}
	if (m_sutamina == 0.0f)
	{
		COOLtime = true;
	}
	if (m_sutamina >= 150.0f)
	{
		m_sutamina = 150.0f;
	}

	if (COOLtime == true)
	{
		cooltime += g_gameTime->GetFrameDeltaTime();
		if (m_sutamina >= 50.0f)
		{
			COOLtime = false;
		}
	}
	else {
		cooltime = 0.0f;
	}

	m_hurusutamina = m_sutamina / 150.0f;
	m_staminaRender.SetScale({ m_hurusutamina,1.0f,0.0f });
	m_staminaRender.SetPosition({ -748.0f,372.0f ,0.0f });
	//m_staminaRender.SetPivot({ 0.0f, 0.5f });
	m_staminaRender.Update();

	if (g_pad[0]->IsPress(enButtonY) && m_sp >= 1) {
		--m_sp;
	}
	else if(m_sp <= 700){
		m_sp += 0.3f;
	}

	m_hurusp = m_sp / 700.0f;
	m_senseRender.SetScale({ m_hurusp,1.0f,0.0f });
	m_senseRender.SetPosition({ -748.0f,325.0f ,0.0f });
	//m_staminaRender.SetPivot({ 0.0f, 0.5f });
	m_senseRender.Update();


	m_HPberRender.SetPosition({ -800.0f,45.0f ,0.0f });
	m_HPberRender.Update();
	m_stmnberRender.SetPosition({ -800.0f,0.0f ,0.0f });
	m_stmnberRender.Update();
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

	if (COOLtime == true)
	{
		m_moveSpeed += cameraForward * lStick_y * 100.0f;	//奥方向への移動速度を加算。
		m_moveSpeed += cameraRight * lStick_x * 100.0f;		//右方向への移動速度を加算。
		m_sutamina++;
	}
	if (COOLtime == false)
	{
		//走りステートだったら
		if (m_playerState == enPlayerState_Run)
		{
			m_moveSpeed += cameraForward * lStick_y * RUN_MOVESPEED;
			m_moveSpeed += cameraRight * lStick_x * RUN_MOVESPEED;
			m_sutamina--;
		}

		else if (m_playerState == enPlayerState_StealthySteps) {
			m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
			m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
			m_sutamina++;
		}
		//それ以外だったら
		else {
			m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
			m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;
			m_sutamina++;
		}
	}
	//重力
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

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
	if (m_playerState == enPlayerState_ReceiveDamage)
	{
		//何もしない
		return;
	}

	//エネミー（ボス）の攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 80.0f;

			if (m_hp <= 0)
			{
				m_playerState = enPlayerState_Down;
			}
			else {
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}

	//エネミー（ボス）の攻撃用のコリジョンを取得する。
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("enemy3_attack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions3)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 80.0f;
			if (m_hp <= 0)
			{
				m_playerState = enPlayerState_Down;
			}
			else {
				//被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}

}

void Player::Attack()
{
	//攻撃ステートではないなら
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_PokeAttack)
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

void Player::MakeAttackCollision()
{
	//コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,				   //座標。
		Quaternion::Identity,                                      //回転。
		Vector3(20.0f, 20.0f, 100.0f)                              //大きさ。
	);


	if (m_playerState == enPlayerState_PokeAttack){
		collisionObject->SetName("player_porkattack");
	}
	else {
		collisionObject->SetName("player_attack");
	}
	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
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
		//突き攻撃ステートの時
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.4f);
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
		st = 2;
		//回避ステートへ移行する
		m_playerState = enPlayerState_Avoidance;
		return;
	}


	//RB1ボタンが押されたら＆攻撃ステート１だったら
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//１撃目の攻撃ステートに移行する
		m_playerState = enPlayerState_FirstAttack;
		m_isUnderAttack = false;
		return;
	}

	//RB2ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonRB2))
	{

		//コリジョンオブジェクトを作成する。
		auto collisionObject = NewGO<CollisionObject>(0);

		Vector3 collisionPosition = m_position;
		collisionPosition.y += 120.0f;
		//ボックス状のコリジョンを作成する。
		collisionObject->CreateBox(collisionPosition,				   //座標。
			Quaternion::Identity,                                      //回転。
			Vector3(200.0f, 200.0f, 200.0f)                              //大きさ。
		);
		collisionObject->SetName("player");


		//突き攻撃ステートに移行する
		m_playerState = enPlayerState_PokeAttack;

		return;
	}

	//Xボタンが押されたら
	if (g_pad[0]->IsPress(enButtonX))
	{
		//忍び足ステートへ移行する
		m_playerState = enPlayerState_StealthySteps;
		return;
	}

	//xかzの移動速度があったら
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		if (COOLtime == true)
		{
			//歩きステートへ移行する
			m_playerState = enPlayerState_Walk;
			st = 0;
			return;
		}
		//Aボタンが押されたら
		if (COOLtime == false && g_pad[0]->IsPress(enButtonA))
		{
			//走りステートへ移行する
			m_playerState = enPlayerState_Run;
			st = 1;
			return;
		}
		//Xボタンが押されたら
		if (g_pad[0]->IsPress(enButtonX))
		{
			//忍び足ステートへ移行する
			m_playerState = enPlayerState_StealthySteps;
			st = 3;
			return;
		}
		//押されなかったら
		else
		{
			//歩きステートへ移行する
			m_playerState = enPlayerState_Walk;
			st = 0;
		}
	}

	//xかzの移動速度がなかったら
	else
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
		st = 0;
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
//	if (m_modelRender.IsPlayingAnimation() == false)
//	{
//		DeleteGO(this);
//	}
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

	if (wcscmp(eventName, L"porkattack_start") == 0)
	{
		//攻撃フラグをtrueにする
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"porkattack_end") == 0)
	{
		//攻撃フラグをfalseにする
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"Run_step") == 0) {
		//足音。
		 //効果音を再生する。
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(1.2f);
	}

	if (wcscmp(eventName, L"Walk_step") == 0) {
		//足音。
		 //効果音を再生する。
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.7f);
	}

}

void Player::Render(RenderContext& rc)
{
	if (m_menu == false) {
		//画像を描写する。
		m_HPRender.Draw(rc);
		m_staminaRender.Draw(rc);
		m_senseRender.Draw(rc);

		m_HPberRender.Draw(rc);
		m_stmnberRender.Draw(rc);
		m_senseberRender.Draw(rc);
	}
	//ドロー。
	m_modelRender.Draw(rc);
}