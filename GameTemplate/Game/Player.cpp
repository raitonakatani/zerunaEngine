#include "stdafx.h"
#include "Player.h"
#include "PAUSE.h"
#include "Game.h"

// EffectEmitterを使用するために、ファイルをインクルードする。
#include "graphics/effect/EffectEmitter.h"

namespace
{
	// Vector3
	const Vector3 HP_SPRITE_POSITION = { -748.0f,420.0f ,0.0f };
	const Vector3 SUTAMINA_SPRITE_POSITION = { -748.0f, 372.0f, 0.0f };
	const Vector3 SP_SPRITE_POSITION = { -748.0f, 325.0f, 0.0f };
	// Vector2
	const Vector2 SPRITE_PIVOT_POSITION = { 0.0f,0.5f };	// スプライトレンダーのピボット。
	// float
	const float SPRITE_WIDTH = 500.0f;						// スプライトレンダーの横サイズ。
	const float SPRITE_HEIGHT = 27.0f;						// スプライトレンダーの縦サイズ。
	const float CHARACON_RADIUS = 30.0f;					// キャラコンの半径
	const float CHARACON_HEIGHT = 100.0f;					// キャラコンの高さ
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;			// 移動速度の最低値
	const float COOLTIME_MOVESPEED = 100.0f;				// 歩きステートの移動速度
	const float WALK_MOVESPEED = 200.0f;					// 歩きステートの移動速度
	const float RUN_MOVESPEED = 400.0f;						// 走りステートの移動速度
	const float STEALTHYSTEP_MOVESPEED = 100.0f;			// 忍び足ステートの移動速度
	const float GRAVITY = 1000.0f;							// 重力
	const float TANKENEMY_ATTACK = 80.0f;					// タンクエネミーの攻撃力。
	const float SPPEDENEMY_ATTACK = 18.0f;					// スピードエネミーの攻撃力。
}

void Player::InitAnimation()
{
	// アニメーションクリップをロードする。
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/player2/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/player2/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/player2/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_snake].Load("Assets/animData/player2/snakes.tka");
	m_animationClipArray[enAnimClip_snake].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_StealthySteps].Load("Assets/animData/player2/stealthysteps.tka");
	m_animationClipArray[enAnimClip_StealthySteps].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/slashattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_PokeAttack].Load("Assets/animData/player2/pokeattack2.tka");
	m_animationClipArray[enAnimClip_PokeAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/player2/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/player2/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Win].Load("Assets/animData/player2/win.tka");
	m_animationClipArray[enAnimClip_Win].SetLoopFlag(false);
}

bool Player::Start()
{
	// アニメーションを初期化。
	InitAnimation();

	// モデルの読み込み
	m_modelRender.Init("Assets/modelData/player/player2.tkm", false, m_animationClipArray, enAnimClip_Num);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	// アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//「Sword」ボーンのID(番号)を取得する。
	m_swordJointBoneId = m_modelRender.FindBoneID(L"Sword_joint");
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");


	// サウンドを読み込む。
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/11yoroi.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/audio/kiru.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/audio/tuki.wav");
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/audio/playerdamage.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/audio/playerdown.wav");
	// キャラコン
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);

	// エフェクトを読み込む。
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/efk/blood2.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/star.efk");

	// ゲームのクラスを探してもってくる。
	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{
	// ゲームが始まってないときの処理。
	if (m_game->start == 0) {
		return;
	}

	// メニュー画面を開いている時の処理。
	if (g_pad[0]->IsTrigger(enButtonStart)		// Startボタンが押された。
		&& m_menu == false)						// かつm_menu==falseの時。
	{
		m_menu = true;
		m_pause = NewGO<PAUSE>(0, "PASUE");
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		// Startボタンが押された。
		&& m_menu == true)							// かつm_menu==trueの時。 
	{
		m_menu = false;
	}
	if (m_menu == true)
	{
		return;
	}



	// 移動処理
	Move();
	// 回転処理
	Rotation();
	// 当たり判定処理
	Collision();
	// アニメーションの再生
	PlayAnimation();
	// 各ステートの遷移処理
	ManageState();
	// 攻撃処理
	Attack();

	// 座標、回転、大きさの更新
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale({ 2.0f,2.0f,2.0 });

	// モデルの更新
	m_modelRender.Update();

	// HPバー。
	if (m_hp >= 100.0f)
	{
		m_hp = 100.0f;
	}
	if (m_hp <= 0) {
		m_hp = 0;
	}
	m_maxHP = m_hp / 100.0f;



	// スタミナバー
	if (m_stamina <= 0.0f)
	{
		m_stamina = 0.0f;
	}
	if (m_stamina == 0.0f)
	{
		m_cooltime = true;
	}
	if (m_stamina >= 200.0f)
	{
		m_stamina = 200.0f;
	}

	// クールタイム。
	if (m_cooltime == true)
	{
		if (m_stamina >= 50.0f)
		{
			m_cooltime = false;
		}
	}

	// 画像の大きさ変更。
	m_maxStamina = m_stamina / 200.0f;


	// SPバー。
	if (g_pad[0]->IsPress(enButtonY) && m_sp >= 1) {
		--m_sp;
	}
	else if (m_sp <= 200) {
		m_sp += 0.2f;
	}
	m_maxSP = m_sp / 200.0f;


}

void Player::Move()
{
	// 待機ステート、歩きステート、走りステート以外だったら
	if (m_playerState != enPlayerState_Run &&
		m_playerState != enPlayerState_Walk &&
		m_playerState != enPlayerState_StealthySteps &&
		m_playerState != enPlayerState_snake &&
		m_playerState != enPlayerState_Idle ||
		prok == true)
	{
		// なにもしない
		return;
	}
	// 移動速度
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	// ステックの入力量を取得
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	// cameraの前方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	// XZ平面での前方方向、右方向に変換する
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();


	// クリア時の処理。
	if (GetPlayerHaveIndex() == 3)
	{
		m_playerState = enPlayerState_Win;
	}

	// 通常時の処理。
	if (GetPlayerHaveIndex() <= 2) {
		m_startTimer += g_gameTime->GetFrameDeltaTime();

		// ゲーム開始時の処理。
		if (m_startTimer >= 1.0f && m_startTimer <= 2.7f)
		{
			m_moveSpeed.z += WALK_MOVESPEED;
		}
		// 通常時の処理。
		else if (m_startTimer > 2.7f) {
			// クールタイムの処理。
			if (m_cooltime == true)
			{
				m_moveSpeed += cameraForward * lStick_y * COOLTIME_MOVESPEED;				// 奥方向への移動速度を加算。
				m_moveSpeed += cameraRight * lStick_x * COOLTIME_MOVESPEED;					// 右方向への移動速度を加算。
				m_stamina++;
			}
			if (m_cooltime == false)
			{
				// 走りステートだったら
				if (m_playerState == enPlayerState_Run)
				{
					m_moveSpeed += cameraForward * lStick_y * RUN_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * RUN_MOVESPEED;
					m_stamina -= 1.5f;
				}
				// しゃがみステートだったら
				else if (m_playerState == enPlayerState_StealthySteps || m_playerState == enPlayerState_snake) {
					m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
					m_stamina -= 0.5f;
				}
				// それ以外だったら
				else {
					m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;
					m_stamina++;
				}
			}
		}
	}
	// 重力
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	// キャラコンを使用して、座標を更新する
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	// 座標の更新
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
	// 回転を設定する
	m_modelRender.SetRotation(m_rotation);
	// プレイヤーの正面ベクトルを計算する
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	// 被ダメージステートだったら
	if (m_playerState == enPlayerState_ReceiveDamage|| m_playerState == enPlayerState_Down)
	{
		// 何もしない
		return;
	}
	// エネミー（タンク）の攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	// コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		// コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= TANKENEMY_ATTACK;
			if (m_hp <= 0)
			{
				// ダウンステートに遷移する。
				m_playerState = enPlayerState_Down;
			}
			else {
				// 被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}
	// エネミー（スピード）の攻撃用のコリジョンを取得する。
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("Speedenemy_attack");
	// コリジョンの配列をfor文で回す。
	for (auto collision2 : collisions2)
	{
		// コリジョンとキャラコンが衝突したら。
		if (collision2->IsHit(m_charaCon))
		{
			m_hp -= SPPEDENEMY_ATTACK;
			if (m_hp <= 0)
			{
				// ダウンステートに遷移する。
				m_playerState = enPlayerState_Down;
			}
			else {
				// 被ダメージステートに遷移する。
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}
}

void Player::Attack()
{
	// 攻撃ステートではないなら
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_PokeAttack)
	{
		// 攻撃処理をしない
		return;
	}
	// 攻撃判定中であれば。
	if (m_isUnderAttack == true)
	{
		// 攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
}

void Player::MakeAttackCollision()
{
	// コリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	// ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(collisionPosition,				   // 座標。
		Quaternion::Identity,                                      // 回転。
		Vector3(20.0f, 20.0f, 120.0f)                              // 大きさ。
	);
	Matrix matrix = m_modelRender.GetBone(m_swordJointBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
	if (m_playerState == enPlayerState_PokeAttack) {
		// 突き攻撃だったら。
		collisionObject->SetName("player_porkattack");
	}
	else {
		// 通常攻撃だったら。
		collisionObject->SetName("player_attack");
	}
}


void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		// 待機ステートの時
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		// 歩きステートの時
	case Player::enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		// 走りステートの時
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		// しゃがみステートの時
	case Player::enPlayerState_snake:
		m_modelRender.PlayAnimation(enAnimClip_snake, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// 忍び足ステートの時
	case Player::enPlayerState_StealthySteps:
		m_modelRender.PlayAnimation(enAnimClip_StealthySteps, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// 攻撃ステートの時
	case Player::enPlayerState_FirstAttack:
		m_modelRender.PlayAnimation(enAnimClip_FirstAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		// 突き攻撃ステートの時
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.4f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// 被ダメージステートの時
	case Player::enPlayerState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// ダウンステートの時
	case Player::enPlayerState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// ダウンステートの時
	case Player::enPlayerState_Win:
		m_modelRender.PlayAnimation(enAnimClip_Win, 0.2f);
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
		// 待機ステートの時
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
		// 歩きステートの時
	case Player::enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
		// 走りステートの時
	case Player::enPlayerState_Run:
		ProcessRunStateTransition();
		break;
		// しゃがみステートの時
	case Player::enPlayerState_snake:
		ProcesssnakeStateTransition();
		break;
		// 忍び足ステートの時
	case Player::enPlayerState_StealthySteps:
		ProcessStealthyStepsStateTransition();
		break;
		// 1撃の攻撃ステートの時
	case Player::enPlayerState_FirstAttack:
		ProcessAttackStateTransition();
		break;
		// 突き攻撃ステートの時
	case Player::enPlayerState_PokeAttack:
		ProcessAttackStateTransition();
		break;
		// 被ダメージステートの時
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		// ダウンステートの時
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
		// 勝利ステートの時
	case Player::enPlayerState_Win:
		ProcessWinStateTransition();
		break;
	default:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (prok == true)
	{
		m_playerState = enPlayerState_Idle;
		return;
	}
	if (m_startTimer >= 1.0f && m_startTimer <= 2.7f)
	{
		m_playerState = enPlayerState_Walk;
		return;
	}

	// RB1ボタンが押されたら＆攻撃ステート１だったら
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		// 攻撃ステートに移行する
		m_playerState = enPlayerState_FirstAttack;
		m_isUnderAttack = false;
		return;
	}

	if (m_playerState != enPlayerState_PokeAttack && g_pad[0]->IsTrigger(enButtonRB2))
	{
		// コリジョンオブジェクトを作成する。
		auto collisionObject = NewGO<CollisionObject>(0);
		Vector3 collisionPosition = m_position;
		collisionPosition += m_forward * 100.0f;
		collisionPosition.y += 100.0f;
		// ボックス状のコリジョンを作成する。
		collisionObject->CreateBox(collisionPosition,		 // 座標。
			m_rotation,                                      // 回転。
			Vector3(40.0f, 30.0f, 140.0f)                    // 大きさ。
		);
		collisionObject->SetName("player");
		// 突き攻撃ステートに移行する
		m_playerState = enPlayerState_PokeAttack;
		return;
	}

	// xかzの移動速度があったら
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{

		if (m_cooltime == true)
		{
			// 歩きステートへ移行する
			m_playerState = enPlayerState_Walk;
			return;
		}
		else {
			// Aボタンが押されたら
			if (g_pad[0]->IsPress(enButtonA))
			{
				// 走りステートへ移行する
				m_playerState = enPlayerState_Run;
				st = 1;
				return;
			}
			// Xボタンが押されたら
			if (g_pad[0]->IsPress(enButtonX))
			{
				// 忍び足ステートへ移行する
				m_playerState = enPlayerState_StealthySteps;
				return;
			}

			// 押されなかったら
			else
			{
				// 歩きステートへ移行する
				m_playerState = enPlayerState_Walk;
			}
		}
	}
	// xかzの移動速度がなかったら
	else
	{
		// Xボタンが押されたら
		if (g_pad[0]->IsPress(enButtonX))
		{
			// 忍び足ステートへ移行する
			m_playerState = enPlayerState_snake;
			return;
		}
		else {
			// 待機ステートに移行する
			m_playerState = enPlayerState_Idle;
		}
		st = 0;
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	// 他のステートに遷移する
	ProcessCommonStateTransition();
}

void  Player::ProcessWalkStateTransition()
{
	st = 0;
	// 他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	// 他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcesssnakeStateTransition()
{
	// 他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessStealthyStepsStateTransition()
{
	// 他のステートに遷移する
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	// アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		// 他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	if (m_hitSE == 0) {
		// 効果音を再生する。
		SoundSource* damagese = NewGO<SoundSource>(0);
		damagese->Init(13);
		damagese->Play(false);
		damagese->SetVolume(0.6f);
		m_hitSE = 1;
	}
	// アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_hitSE = 0;
		// 他のステートに遷移する
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	m_downTimer += g_gameTime->GetFrameDeltaTime();
	// アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_down = true;
	}
	if (m_hitSE == 0) {
		// 効果音を再生する。
		SoundSource* damagese = NewGO<SoundSource>(0);
		damagese->Init(13);
		damagese->Play(false);
		damagese->SetVolume(0.8f);
		m_hitSE = 1;
	}
	if (m_deathSE == 0 && m_downTimer >= 1.2f) {
		// 効果音を再生する。
		SoundSource* downse = NewGO<SoundSource>(0);
		downse->Init(14);
		downse->Play(false);
		downse->SetVolume(0.8f);
		m_deathSE = 1;
	}
	if (m_deathSE == 1 && m_downTimer >= 2.5f) {
		// 効果音を再生する。
		SoundSource* downse = NewGO<SoundSource>(0);
		downse->Init(14);
		downse->Play(false);
		downse->SetVolume(0.8f);
		m_deathSE = 2;
	}
}


void Player::ProcessWinStateTransition()
{
	// アニメーションの再生が終わったら
	if (m_modelRender.IsPlayingAnimation() == false) {
		SetPlayerHaveIndex(4);
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		// 効果音を再生する。
		SoundSource* slashse = NewGO<SoundSource>(0);
		slashse->Init(11);
		slashse->Play(false);
		slashse->SetVolume(0.6f);
		// 攻撃フラグをtrueにする
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		// 攻撃フラグをfalseにする
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"porkattack_start") == 0)
	{
		if (m_critical == 1) {
			// エフェクトを再生する。
			Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
			Vector3 m_effectPosition = m_position;
			m_effectPosition.y += 100.0f;
			// エフェクトのオブジェクトを作成する。
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(2);
			m_effectEmitter->SetPosition(m_effectPosition);
			// エフェクトの大きさを設定する。
			m_effectEmitter->SetScale(m_scale * 5.0f);
			m_effectEmitter->Play();
			m_effectEmitter->SetWorldMatrix(matrix);
		}
	}
	else if (wcscmp(eventName, L"porkattack") == 0)
	{
		// 効果音を再生する。
		SoundSource* slashse = NewGO<SoundSource>(0);
		slashse->Init(12);
		slashse->Play(false);
		slashse->SetVolume(0.6f);
		// 攻撃フラグをtrueにする
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"porkattack_end") == 0)
	{
		if (prok == true) {
			// エフェクトを再生する。
			Matrix matrix = m_modelRender.GetBone(m_swordJointBoneId)->GetWorldMatrix();
			Vector3 m_effectPosition = m_position;
			m_effectPosition.y += 100.0f;
			// エフェクトのオブジェクトを作成する。
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(3);
			m_effectEmitter->SetPosition(m_effectPosition);
			// エフェクトの大きさを設定する。
			m_effectEmitter->SetScale(m_scale * 50.0f);
			m_effectEmitter->Play();
			m_effectEmitter->SetWorldMatrix(matrix);
		}
		m_critical = 0;
		// 攻撃フラグをfalseにする
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"Run_step") == 0) {
		// 足音。
		// 効果音を再生する。
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.8f);
	}

	if (wcscmp(eventName, L"Walk_step") == 0) {
		// 足音。
		// 効果音を再生する。
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.5f);
	}

	if (wcscmp(eventName, L"snake_step") == 0) {
		// 足音。
		// 効果音を再生する。
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.3f);
	}
}

void Player::Render(RenderContext& rc)
{
	if (m_game->start == 0) {
		return;
	}
	// モデルをドロー。
	m_modelRender.Draw(rc);
}