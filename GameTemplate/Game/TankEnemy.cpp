#include "stdafx.h"
#include "TankEnemy.h"

#include "Game.h"
#include "Player.h"

//#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"


#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



TankEnemy::TankEnemy()
{

}

TankEnemy::~TankEnemy()
{

}

bool TankEnemy::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
//	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy3/damage.tka");
//	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
//	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy3/sibou.tka");
//	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/Enemy/enemy.tkm", m_animationClips, enAnimationClip_Num);

	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale({ 1.5f,1.5f,1.5 });
	m_modelRender.Update();

	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		50.0f,			//半径。
		200.0f,			//高さ。
		m_position		//座標。
	);



	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void TankEnemy::Update()
{

	//追跡処理。
	Chase();
	//回転処理。
	Rotation();
	//当たり判定。
	Collision();
	//攻撃処理。
	Attack();
	//アニメーションの再生。
	PlayAnimation();
	//ステートの遷移処理。
	ManageState();

	//モデルの更新。
	m_modelRender.Update();
}

void TankEnemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
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

void TankEnemy::Chase()
{
	//追跡ステートでないなら、追跡処理はしない。
	if (m_EnemyState != enEnemyState_Chase)
	{
		return;
	}

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//ちょっとだけモデルの座標を挙げる。
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void TankEnemy::Collision()
{
/*	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_EnemyState == enEnemyState_ReceiveDamage ||
		m_EnemyState == enEnemyState_Down)
	{
		return;
	}
*/
	/* {
			//プレイヤーの攻撃用のコリジョンを取得する。
			const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
			//コリジョンの配列をfor文で回す。
			for (auto collision : collisions)
			{
				//コリジョンとキャラコンが衝突したら。
				if (collision->IsHit(m_charaCon))
				{
					//HPを1減らす。
					m_hp -= karyoku;

					//音を読み込む。
					g_soundEngine->ResistWaveFileBank(3, "Assets/sound/3damage.wav");
					//効果音を再生する。
					SoundSource* damagese = NewGO<SoundSource>(0);
					damagese->Init(3);
					damagese->Play(false);
					damagese->SetVolume(1.5f);

					//HPが0になったら。
					if (m_hp <= 0)
					{
						//ダウンステートに遷移する。
						m_EnemyState = enEnemy3State_Down;
					}
					else {
						//被ダメージステートに遷移する。
						m_Enemy3State = enEnemy3State_ReceiveDamage;
					}
					return;
				}
			}
		}
	*/
}

void TankEnemy::Attack()
{
	//攻撃ステートでないなら処理をしない。
	if (m_EnemyState != enEnemyState_Attack)
	{
		return;
	}

	//攻撃中であれば。
	if (m_isUnderAttack == true)
	{
		//攻撃用のコリジョンを作成する。
		MakeAttackCollision();
	}
}

const bool TankEnemy::SearchPlayer() const
{
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーにある程度近かったら.。
	if (diff.LengthSq() <= 200.0 * 200.0f)
	{
		//エネミーからプレイヤーに向かうベクトルを正規化する。
		diff.Normalize();
		//エネミーの正面のベクトルと、エネミーからプレイヤーに向かうベクトルの。
		//内積(cosθ)を求める。
		float cos = m_forward.Dot(diff);
		//内積(cosθ)から角度(θ)を求める。
		float angle = acosf(cos);
		//角度(θ)が120°より小さければ。
		if (angle <= (Math::PI / 180.0f) * 135.0f)
		{
			//プレイヤーを見つけた！
			return true;
		}
	}
	//プレイヤーを見つけられなかった。
	return false;
}

void TankEnemy::MakeAttackCollision()
{
	/*	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
		auto collisionObject = NewGO<CollisionObject>(0);
		//剣のボーンのワールド行列を取得する。
		Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
		//ボックス状のコリジョンを作成する。
		collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(300.0f, 100.0f, 100.0f));
		collisionObject->SetWorldMatrix(matrix);
		collisionObject->SetName("TankEnemy_attack");
		*/
}

void TankEnemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//エネミーからプレイヤーに向かうベクトルを計算する。
	Vector3 diff = m_player->GetPosition() - m_position;

	//プレイヤーを見つけたら。
	if (SearchPlayer() == true)
	{
		//ベクトルを正規化する。
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 50.0f;

		Vector3 toPlayerDir = diff;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);

		//攻撃できる距離なら。
		if (IsCanAttack() == true)
		{
			//乱数によって、攻撃するか待機させるかを決定する。	
			int ram = rand() % 100;
			if (ram >= 5)
			{
				//攻撃ステートに遷移する。
				m_EnemyState = enEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//魔法攻撃ステートに遷移する。
				m_EnemyState = enEnemyState_Attack;
				return;
			}

		}
		//攻撃できない距離なら。
		else
		{
			//追跡ステートに遷移する。
			m_EnemyState = enEnemyState_Chase;
			return;
		}
	}
	//プレイヤーを見つけられなければ。
	else
	{
		//待機ステートに遷移する。
		m_EnemyState = enEnemyState_Idle;
		return;

	}
}

void TankEnemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.9f)
	{
		//他のステートへ遷移する。
		ProcessCommonStateTransition();
	}

}

void TankEnemy::ProcessWalkStateTransition()
{
	//他のステートに遷移する。
	ProcessCommonStateTransition();
}

void TankEnemy::ProcessChaseStateTransition()
{
	//攻撃できる距離なら。
	if (IsCanAttack() == true)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//追跡時間がある程度経過したら。
	if (m_chaseTimer >= 0.8f)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void TankEnemy::ProcessAttackStateTransition()
{
	//攻撃アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void TankEnemy::ProcessReceiveDamageStateTransition()
{
	//被ダメージアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//攻撃されたら距離関係無しに、取り敢えず追跡させる。
		m_EnemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff * 50.0f;
	}
}

void TankEnemy::ProcessDownStateTransition()
{
	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		g_soundEngine->ResistWaveFileBank(1, "Assets/sound/1sibouzi.wav");
		//効果音を再生する。
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(1);
		se->Play(false);
		se->SetVolume(1.5f);
		//自身を削除する。
		DeleteGO(this);
	}
}

void TankEnemy::ManageState()
{
	switch (m_EnemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機ステートのステート遷移処理。
		ProcessIdleStateTransition();
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		//追跡ステートのステート遷移処理。
		ProcessChaseStateTransition();
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		//攻撃ステートのステート遷移処理。
		ProcessAttackStateTransition();
		break;
/*		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
*/	}
}

void TankEnemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_EnemyState)
	{
		//待機ステートの時。
	case enEnemyState_Idle:
		//待機アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//追跡ステートの時。
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.4f);
		//走りアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//攻撃ステートの時。
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.3f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
/*		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.1f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
*/	default:
		break;
	}
}

void TankEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「attack_start」の時。
	if (wcscmp(eventName, L"attack_start") == 0) {
		//攻撃中判定をtrueにする。
		m_isUnderAttack = true;
	}
	//キーの名前が「attack_end」の時。
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//攻撃中判定をfalseにする。
		m_isUnderAttack = false;
	}
}

const bool TankEnemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//エネミーとプレイヤーの距離が近かったら。
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}

void TankEnemy::MODEL()
{
	m_model = 1;
}

void TankEnemy::Render(RenderContext& rc)
{

	//モデルを描画する。
	m_modelRender.Draw(rc);
}