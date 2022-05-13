#include "stdafx.h"
#include "TankEnemy.h"
#include "TankEnemy2.h"

#include "Game.h"
#include "Player.h"
#include "EnemyPath.h"
#include "GameCamera.h"

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
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/tankenemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/tankenemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/tankenemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/tankenemy/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/tankenemy/sibou.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Death].Load("Assets/animData/tankenemy/sokusi.tka");
	m_animationClips[enAnimationClip_Death].SetLoopFlag(false);
	m_animationClips[enAnimationClip_look].Load("Assets/animData/tankenemy/look.tka");
	m_animationClips[enAnimationClip_look].SetLoopFlag(false);
	m_animationClips[enAnimationClip_alert].Load("Assets/animData/tankenemy/alert.tka");
	m_animationClips[enAnimationClip_alert].SetLoopFlag(false);
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/tankEnemy/tank.tkm", m_animationClips, enAnimationClip_Num);

	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//大きさを設定する。
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_firstPosition = m_position;
	m_position.y = 15.0f;
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(
		50.0f,			//半径。
		140.0f,			//高さ。
		m_position		//座標。
	);

	//スフィアコライダーを初期化。
	m_sphereCollider.Create(1.0f);

	//アニメーションイベント用の関数を設定する
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//「Sword」ボーンのID(番号)を取得する。
	m_Hand = m_modelRender.FindBoneID(L"LeftHand");
	m_weakness = m_modelRender.FindBoneID(L"Spine");

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	m_camera = FindGO<GameCamera>("gameCamera");
	m_tank2 = FindGO<TankEnemy2>("TankEnemy2");

	//乱数を初期化。
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	m_enemypath.Init("Assets/path/tank/enemypath1.tkl");
	m_enemypath2.Init("Assets/path/tank/enemypath2.tkl");
	m_enemypath3.Init("Assets/path/tank/enemypath3.tkl");
	m_enemypath4.Init("Assets/path/tank/enemypath4.tkl");

	m_point = m_enemypath.GetFirstPoint();
	m_point2 = m_enemypath2.GetFirstPoint();
	m_point3 = m_enemypath3.GetFirstPoint();
	m_point4 = m_enemypath4.GetFirstPoint();

	// ナビメッシュを構築。
	m_nvmMesh.Init("Assets/modelData/nvm/test2.tkn");

	m_timer = 0.0f;

	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/10houkou.wav");

	alertSprite.Init("Assets/sprite/alert.dds", 64, 64);
	//表示する座標を設定する。
	alertSprite.SetPosition({ 0.0f,0.0f ,0.0f });

	return true;
}

void TankEnemy::Update()
{
	if (g_pad[0]->IsPress(enButtonY))
	{
		g_Light.SetLigPoint({ m_position.x,100.0f,m_position.z });
		g_Light.SetPointRange(300.0f);
		g_Light.SetLigPointColor({ 10.0f,0.0f,0.0f });
	}
	else {
		g_Light.SetLigPointColor({ 0.0f,0.0f,0.0f });
	}

	Weak = m_player->GetPosition() - m_position;

/*	if (Weak.Length() <= 800.0f && m_hp > 0)
	{
		g_Light.SetLigSpot({ m_position.x,200.0f,m_position.z });
		g_Light.SetLigSpotRot({ 0.0f,-1.0f,0.0f });
		if (m_isSearchPlayer == true)
		{
			g_Light.SetLigSpotColor({ 10.0f,0.0f,0.0f });
		}
		else
		{
			g_Light.SetLigSpotColor({ 0.0f,10.0f,0.0f });
		}
	}
	else {
		g_Light.SetLigSpotColor({ 0.0f,0.0f,0.0f });
	}
*/	


	Vector3 range = m_tank2->GetPosition() - m_position;

	if (range.Length() <= 300.0f)
	{
		
		if (m_tank2->m_hp <= 0 && ab==0)
		{
			m_EnemyState = enEnemyState_look;
			ab = 1;
			//効果音を再生する。
			SE = NewGO<SoundSource>(0);
			SE->Init(10);
			SE->Play(false);
			SE->SetVolume(1.5f);
		}
	}

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


	if (state == 0 &&m_isSearchPlayer == false) {

		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 2.0f) {
			m_EnemyState = enEnemyState_Idle;
		}
		if (m_timer >= 2.0f) {
			m_EnemyState = enEnemyState_Chase;
			state = 1;
		}
	}

	if (m_camera->m_camera == 1)
	{
		m_EnemyState = enEnemyState_Idle;
	}

	Vector3 diff = m_player->GetPosition() - m_position;

	//ベクトルの長さが700.0fより小さかったら。
	if (diff.LengthSq() <= m_range * m_range && m_hp > 0)
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
		alertSprite.SetPosition({2000.0f,2000.0f,0.0f});
	}
	alertSprite.Update();
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

void TankEnemy::Chase()
{
	//プレイヤーを見つけていなかったら。
	if (state == 1)
	{
		if (Weak.Length() <= 800.0f && m_player->st == 1 || Weak.Length() <= 800.0f && m_player->st == 2)
		{
			m_EnemyState = enEnemyState_alert;	
			
			if (alertLevel == 0) {
				alertLevel = 1;
			}
		//	if (alertLevel == 3)
		//	{
		//		return;
		//	}
		}
		else if (alertLevel == 0 || alertLevel == 3) {
			m_EnemyState = enEnemyState_Chase;
			Aroute();
		}
	}

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
	m_modelRender.SetPosition(m_position);
}

void TankEnemy::Collision()
{
	//被ダメージ、あるいはダウンステートの時は。
	//当たり判定処理はしない。
	if (m_EnemyState == enEnemyState_ReceiveDamage ||
		m_EnemyState == enEnemyState_Down)
	{
		return;
	}

	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_weakness)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 50.0f, 50.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy");


	//プレイヤーの攻撃用のコリジョンを取得する。
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_porkattack");
	//コリジョンの配列をfor文で回す。
	for (auto collision : collisions)
	{
		//コリジョンとキャラコンが衝突したら。
		if (collision->IsHit(collisionObject))
		{
			state = 2;
			m_camera->m_camera = 0;
			//ダウンステートに遷移する。
			m_EnemyState = enEnemyState_Death;
			return;
		}
	}

	{

		//プレイヤーの攻撃用のコリジョンを取得する。
		const auto& collisions5 = g_collisionObjectManager->FindCollisionObjects("player");
		//コリジョンの配列をfor文で回す。
		for (auto collision : collisions5)
		{
			state = 2;
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(collisionObject))
			{
			//	m_camera->m_camera = 1;
				return;
			}
		}


		//プレイヤーの攻撃用のコリジョンを取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
		//コリジョンの配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				state = 2;
				if (m_isSearchPlayer == false) {
					m_hp -= 10;
					//HPが0になったら。
					if (m_hp <= 0)
					{
						//ダウンステートに遷移する。
						m_EnemyState = enEnemyState_Death;
					}
					else {
						//被ダメージステートに遷移する。
						m_EnemyState = enEnemyState_ReceiveDamage;
					}
				}
				else {
					m_hp -= 4;

					//HPが0になったら。
					if (m_hp <= 0)
					{
						//ダウンステートに遷移する。
						m_EnemyState = enEnemyState_Down;
					}
					else {
						//被ダメージステートに遷移する。
						m_EnemyState = enEnemyState_ReceiveDamage;
					}
				}
				return;
			}
		}

	}
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

void TankEnemy::SearchPlayer()
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

void TankEnemy::MakeAttackCollision()
{
	//攻撃当たり判定用のコリジョンオブジェクトを作成する。
	auto collisionObject = NewGO<CollisionObject>(0);
	//剣のボーンのワールド行列を取得する。
	Matrix matrix = m_modelRender.GetBone(m_Hand)->GetWorldMatrix();
	//ボックス状のコリジョンを作成する。
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 100.0f, 100.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");

}

void TankEnemy::ProcessCommonStateTransition()
{
	//各タイマーを初期化。
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

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

		//攻撃できる距離なら。
		if (IsCanAttack() == true)
		{
			
			//乱数によって、攻撃するか待機させるかを決定する。	
			int ram = rand() % 100;
			if (ram >= 0)
			{
				//攻撃ステートに遷移する。
				m_EnemyState = enEnemyState_Attack;
				//m_isUnderAttack = false;
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
	else if (mikke == true) {
		Vector3 diff2 = targetpos - m_position;
		if (diff2.LengthSq() <= 500.0f * 500.0f * g_gameTime->GetFrameDeltaTime()) {
			if (m_isSearchPlayer == false) {
				mikke = false;
			}
		}

		//エネミーからプレイヤーに向かうベクトルを計算する。
		Vector3 diff1 = targetpos - m_position;
		huntertimer += g_gameTime->GetFrameDeltaTime();
		//ベクトルを正規化する。
		diff1.Normalize();
		//移動速度を設定する。
		m_moveSpeed = diff1 * 150.0f;

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

void TankEnemy::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//待機時間がある程度経過したら。
	if (m_idleTimer >= 0.1f)
	{
	//	alertLevel = 0;
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
		m_idleTimer += g_gameTime->GetFrameDeltaTime();
		//待機時間がある程度経過したら。
		if (m_idleTimer >= 1.0f)
		{
			//他のステートに遷移する。
			ProcessCommonStateTransition();
		}
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
		m_moveSpeed = diff * 150.0f;
	}
}

void TankEnemy::ProcessDownStateTransition()
{
	m_position.y = 15.0f;
	m_modelRender.SetPosition(m_position);
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_hp = 0;
		state = 3;
		m_charaCon.RemoveRigidBoby();
	}
/*	//ダウンアニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		state = 0;
		m_camera->m_camera = 0;
		//自身を削除する。
	//	DeleteGO(this);
	}
	*/
}

void TankEnemy::lookTransition()
{
	//警報アニメーションの再生が終わったら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
	//	alertLevel = 0;
		//他のステートに遷移する。
		ProcessCommonStateTransition();
	}
}

void TankEnemy::alertTransition()
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
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		//被ダメージステートのステート遷移処理。
		ProcessReceiveDamageStateTransition();
		break;
		//即死ステートの時。
	case enEnemyState_Death:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンステートのステート遷移処理。
		ProcessDownStateTransition();
		break;
		//警報ステートの時。
	case enEnemyState_look:
		//警報ステートのステート遷移処理。
		lookTransition();
		break;
		//警戒ステートの時。
	case enEnemyState_alert:
		//警戒ステートのステート遷移処理。
		alertTransition();
		break;
	}
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
		m_modelRender.SetAnimationSpeed(1.8f);
		//攻撃アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//被ダメージステートの時。
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.5f);
		//被ダメージアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//即死ステートの時。
	case enEnemyState_Death:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.1f);
		break;
		//ダウンステートの時。
	case enEnemyState_Down:
		//ダウンアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
		//呼ぶステートの時。
	case enEnemyState_look:
		//呼ぶアニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_look, 0.1f);
		break;
		//警戒ステートの時。
	case enEnemyState_alert:
		//警戒アニメーションを再生。
		m_modelRender.PlayAnimation(enAnimationClip_alert, 0.1f);
		break;
	default:
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
	if (diff.LengthSq() <= 200.0f * 200.0f)
	{
		//攻撃できる！
		return true;
	}
	//攻撃できない。
	return false;
}


///経路
void TankEnemy::Aroute()
{
	//bool isEnd;
	if (m_tankNumber == 0)
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
			//目標地点に近くなかったら
			else
			{
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
					m_moveSpeed = diff * 120.0f;
					//Y座標の移動速度を0にする
					m_moveSpeed.y = 0.0f;
				}
			
		}
	}
	if (m_tankNumber == 1)
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
				m_point = m_enemypath2.GetNextPoint(m_point->s_number);
			}
		}
		//目標地点に近くなかったら
		else
		{
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
				m_moveSpeed = diff * 120.0f;
				//Y座標の移動速度を0にする
				m_moveSpeed.y = 0.0f;
			}

		}
	}
	if (m_tankNumber == 2)
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
		m_moveSpeed = range * 15.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}
	if (m_tankNumber == 3)
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
		m_moveSpeed = range * 15.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}

}
void TankEnemy::Broute()
{

}

void TankEnemy::Render(RenderContext& rc)
{
	if (Weak.Length() <= 3000.0f)
	{
		alertSprite.Draw(rc);
		//モデルを描画する。
		m_modelRender.Draw(rc);
	}

}