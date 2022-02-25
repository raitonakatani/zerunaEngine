#include "stdafx.h"
#include "Player.h"

Player::Player()
{

}

Player::~Player()
{
	
}

bool Player::Start()
{
	//アニメーションクリップをロードする。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	animationClips[enAnimationClip_run].Load("Assets/animData/run.tka");
	animationClips[enAnimationClip_run].SetLoopFlag(true);

	// 番号を指定して、efkファイルを読み込む。
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/syoukann.efk");
	

	//ユニティちゃんのモデルを読み込む。
	m_modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);
	
	//キャラクターコントローラーを初期化。
	m_charaCon.Init(20.0f, 100.0f, m_position);

	return true;
}

void Player::Update()
{
	//移動処理。
	Move();

	//回転処理。
	Turn();

	//ステート管理。
	ManageState();

	//アニメーションの再生。
	PlayAnimation();

	//絵描きさんの更新処理。
	m_modelRender.Update();

}


void Player::Move()
{
	//このフレームの移動量を求める。
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//カメラの前方方向と右方向を取得。
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	m_moveSpeed += cameraForward * lStick_y * 400.0f;	//奥方向への移動速度を加算。
	m_moveSpeed += cameraRight * lStick_x * 400.0f;		//右方向への移動速度を加算。
	if (g_pad[0]->IsTrigger(enButtonA) //Aボタンが押されたら
		&& m_charaCon.IsOnGround()  //かつ、地面に居たら
		|| (GetAsyncKeyState(VK_SPACE) != 0) && m_charaCon.IsOnGround()
		) {
		//ジャンプする。
		m_moveSpeed.y = 350.0f;	//上方向に速度を設定して、

	}
	if (g_pad[0]->IsTrigger(enButtonA) && m_charaCon.IsJump() && secondjump == 0      /*Aボタンが押されたら*/
		|| (GetAsyncKeyState(VK_SPACE) != 0) && m_charaCon.IsJump() && secondjump == 0)  //かつ、ジャンプ中だったら
	{
		m_moveSpeed.y = 350.0f;	//上方向に速度を設定して、
		PlusSECONDJUMP();
	}
	m_moveSpeed.y -= 600.0f * g_gameTime->GetFrameDeltaTime();

	if (g_pad[0]->IsPress(enButtonX))
	{
		m_moveSpeed += cameraForward * lStick_y * 500.0f;	//奥方向への移動速度を加算。
		m_moveSpeed += cameraRight * lStick_x * 500.0f;		//右方向への移動速度を加算。
	}

	//キャラクターコントローラーを使用して、座標を更新。

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//地面についた。
		m_moveSpeed.y = 0.0f;
		secondjump = 0;
	}

	//座標を設定。
	m_modelRender.SetPosition(m_position);
}

void Player::Turn()
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
}

//ステート管理。
void Player::ManageState()
{
	//地面に付いていなかったら。
	if (m_charaCon.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		playerState = 1;
		return;

	}

	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//ステートを2(歩き)にする。
		playerState = 2;

		if (g_pad[0]->IsPress(enButtonX))
		{
			playerState = 3;
		}
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else
	{
		//ステートを0(待機)にする。
		playerState = 0;

	}
}

//アニメーションの再生。
void Player::PlayAnimation()
{
	//switch文。
	switch (playerState) {
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
		//プレイヤーステートが2(歩き)だったら。
	case 2:
		//歩きアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Walk);
		break;
		//プレイヤーステートが3(走る)だったら。
	case 3:
		//走るアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_run);
		break;
	}
}

//描画処理。
void Player::Render(RenderContext& rc)
{
	//プレイヤーを描画する。
	m_modelRender.Draw(rc);
}