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
	//�A�j���[�V������ǂݍ��ށB
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
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/tankEnemy/tank.tkm", m_animationClips, enAnimationClip_Num);

	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_firstPosition = m_position;
	m_position.y = 15.0f;
	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		50.0f,			//���a�B
		140.0f,			//�����B
		m_position		//���W�B
	);

	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_Hand = m_modelRender.FindBoneID(L"LeftHand");
	m_weakness = m_modelRender.FindBoneID(L"Spine");

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");
	m_camera = FindGO<GameCamera>("gameCamera");
	m_tank2 = FindGO<TankEnemy2>("TankEnemy2");

	//�������������B
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

	// �i�r���b�V�����\�z�B
	m_nvmMesh.Init("Assets/modelData/nvm/test2.tkn");

	m_timer = 0.0f;

	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/10houkou.wav");

	alertSprite.Init("Assets/sprite/alert.dds", 64, 64);
	//�\��������W��ݒ肷��B
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
			//���ʉ����Đ�����B
			SE = NewGO<SoundSource>(0);
			SE->Init(10);
			SE->Play(false);
			SE->SetVolume(1.5f);
		}
	}

	//�ǐՏ����B
	Chase();
	//��]�����B
	Rotation();
	//�����蔻��B
	Collision();
	//�U�������B
	Attack();
	//�A�j���[�V�����̍Đ��B
	PlayAnimation();
	//�X�e�[�g�̑J�ڏ����B
	ManageState();
	//�T�[�`
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

	//�x�N�g���̒�����700.0f��菬����������B
	if (diff.LengthSq() <= m_range * m_range && m_hp > 0)
	{
		//���[���h���W�ɕϊ��B
		//���W���G�l�~�[�̏�����ɐݒ肷��B
		Vector4 worldPos = Vector4(m_position.x, m_position.y + 250.0f, m_position.z, 1.0f);

		Matrix matrix;
		matrix.Multiply(g_camera3D->GetViewMatrix(), g_camera3D->GetProjectionMatrix());

		matrix.Apply(worldPos);

		//�J�����̃r���[�s����|����B
		//�J�������W�ɕϊ��B
		worldPos.x = (worldPos.x / worldPos.w);
		worldPos.y = (worldPos.y / worldPos.w);

		//�J�����̃v���W�F�N�V�����s����|����B
		//�X�N���[�����W�ɕϊ��B
		worldPos.x *= FRAME_BUFFER_W / 2;
		worldPos.y *= FRAME_BUFFER_H / 2;

		//�|�W�V�����̐ݒ�B
		alertSprite.SetPosition(Vector3(worldPos.x, worldPos.y, 0.0f));
	}
	else {
		alertSprite.SetPosition({2000.0f,2000.0f,0.0f});
	}
	alertSprite.Update();
	//���f���̍X�V�B
	m_modelRender.Update();
}

void TankEnemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		//m_moveSpeed.x��m_moveSpeed.z�̐�Βl���Ƃ���0.001�ȉ��Ƃ������Ƃ�
		//���̃t���[���ł̓L�����͈ړ����Ă��Ȃ��̂Ő��񂷂�K�v�͂Ȃ��B
		return;
	}
	//atan2��tan�Ƃ̒l���p�x(���W�A���P��)�ɕϊ����Ă����֐��B
	//m_moveSpeed.x / m_moveSpeed.z�̌��ʂ�tan�ƂɂȂ�B
	//atan2���g�p���āA�p�x�����߂Ă���B
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ�������B
		//�t���O��true�ɁB
		isHit = true;
		return 0.0f;
	}
};

void TankEnemy::Chase()
{
	//�v���C���[�������Ă��Ȃ�������B
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

	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_EnemyState != enEnemyState_Chase)
	{
		return;
	}

	
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	m_modelRender.SetPosition(m_position);
}

void TankEnemy::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_EnemyState == enEnemyState_ReceiveDamage ||
		m_EnemyState == enEnemyState_Down)
	{
		return;
	}

	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_weakness)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 50.0f, 50.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy");


	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_porkattack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(collisionObject))
		{
			state = 2;
			m_camera->m_camera = 0;
			//�_�E���X�e�[�g�ɑJ�ڂ���B
			m_EnemyState = enEnemyState_Death;
			return;
		}
	}

	{

		//�v���C���[�̍U���p�̃R���W�������擾����B
		const auto& collisions5 = g_collisionObjectManager->FindCollisionObjects("player");
		//�R���W�����̔z���for���ŉ񂷁B
		for (auto collision : collisions5)
		{
			state = 2;
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(collisionObject))
			{
			//	m_camera->m_camera = 1;
				return;
			}
		}


		//�v���C���[�̍U���p�̃R���W�������擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
		//�R���W�����̔z���for���ŉ񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				state = 2;
				if (m_isSearchPlayer == false) {
					m_hp -= 10;
					//HP��0�ɂȂ�����B
					if (m_hp <= 0)
					{
						//�_�E���X�e�[�g�ɑJ�ڂ���B
						m_EnemyState = enEnemyState_Death;
					}
					else {
						//��_���[�W�X�e�[�g�ɑJ�ڂ���B
						m_EnemyState = enEnemyState_ReceiveDamage;
					}
				}
				else {
					m_hp -= 4;

					//HP��0�ɂȂ�����B
					if (m_hp <= 0)
					{
						//�_�E���X�e�[�g�ɑJ�ڂ���B
						m_EnemyState = enEnemyState_Down;
					}
					else {
						//��_���[�W�X�e�[�g�ɑJ�ڂ���B
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
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_EnemyState != enEnemyState_Attack)
	{
		return;
	}

	//�U�����ł���΁B
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
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


	//�v���C���[�����E���ɋ��Ȃ�������B
	if (Math::PI * m_angl <= fabsf(angle))
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 70.0f, playerPosition.z));

	SweepResultWall callback;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�Փ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//�ǂƏՓ˂����I
	if (callback.isHit == true)
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return;
	}

	//�ǂƏՓ˂��ĂȂ��I�I
	//�v���C���[�������t���O��true�ɁB
	m_isSearchPlayer = true;
}

void TankEnemy::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_Hand)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 100.0f, 100.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");

}

void TankEnemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������B
	if (m_isSearchPlayer == true && diff.LengthSq() <= m_range * m_range)
	{
		state = 0;
		m_timer = 0.0f;
		alertTimet = 0.0f;
		alertLevel = 2;
		mikke = true;
		targetpos = m_player->GetPosition();

		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 150.0f;

		Vector3 toPlayerDir = diff;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);

		//�U���ł��鋗���Ȃ�B
		if (IsCanAttack() == true)
		{
			
			//�����ɂ���āA�U�����邩�ҋ@�����邩�����肷��B	
			int ram = rand() % 100;
			if (ram >= 0)
			{
				//�U���X�e�[�g�ɑJ�ڂ���B
				m_EnemyState = enEnemyState_Attack;
				//m_isUnderAttack = false;
				return;
			}
			else
			{
				//���@�U���X�e�[�g�ɑJ�ڂ���B
				m_EnemyState = enEnemyState_Attack;
				return;
			}

		}
		//�U���ł��Ȃ������Ȃ�B
		else
		{
			//�ǐՃX�e�[�g�ɑJ�ڂ���B
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

		//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
		Vector3 diff1 = targetpos - m_position;
		huntertimer += g_gameTime->GetFrameDeltaTime();
		//�x�N�g���𐳋K������B
		diff1.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff1 * 150.0f;

		Vector3 toPlayerDir = diff1;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);
	}
	//�v���C���[���������Ȃ���΁B
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
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.1f)
	{
	//	alertLevel = 0;
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void TankEnemy::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void TankEnemy::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�B
	if (IsCanAttack() == true)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
		return;
	}
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();
	//�ǐՎ��Ԃ�������x�o�߂�����B
	if (m_chaseTimer >= 0.8f)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void TankEnemy::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_idleTimer += g_gameTime->GetFrameDeltaTime();
		//�ҋ@���Ԃ�������x�o�߂�����B
		if (m_idleTimer >= 1.0f)
		{
			//���̃X�e�[�g�ɑJ�ڂ���B
			ProcessCommonStateTransition();
		}
	}
}

void TankEnemy::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_EnemyState = enEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
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
/*	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		state = 0;
		m_camera->m_camera = 0;
		//���g���폜����B
	//	DeleteGO(this);
	}
	*/
}

void TankEnemy::lookTransition()
{
	//�x��A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
	//	alertLevel = 0;
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void TankEnemy::alertTransition()
{
	if (m_modelRender.IsPlayingAnimation() == true)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
		Vector3 diff = m_player->GetPosition() - m_position;
		if (m_isSearchPlayer == true && diff.LengthSq() <= m_range * m_range)
		{
			ProcessCommonStateTransition();
		}
	}

	//�x��A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		if (alertLevel == 1) {
			alertLevel = 0;
		}
			//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}


void TankEnemy::ManageState()
{
	switch (m_EnemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�����X�e�[�g�̎��B
	case enEnemyState_Death:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
		//�x��X�e�[�g�̎��B
	case enEnemyState_look:
		//�x��X�e�[�g�̃X�e�[�g�J�ڏ����B
		lookTransition();
		break;
		//�x���X�e�[�g�̎��B
	case enEnemyState_alert:
		//�x���X�e�[�g�̃X�e�[�g�J�ڏ����B
		alertTransition();
		break;
	}
}

void TankEnemy::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_EnemyState)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemyState_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemyState_Chase:
		m_modelRender.SetAnimationSpeed(1.4f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enEnemyState_Attack:
		m_modelRender.SetAnimationSpeed(1.8f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemyState_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.5f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�����X�e�[�g�̎��B
	case enEnemyState_Death:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Death, 0.1f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemyState_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
		//�ĂԃX�e�[�g�̎��B
	case enEnemyState_look:
		//�ĂԃA�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_look, 0.1f);
		break;
		//�x���X�e�[�g�̎��B
	case enEnemyState_alert:
		//�x���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_alert, 0.1f);
		break;
	default:
		break;
	}
}

void TankEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�̎��B
	if (wcscmp(eventName, L"attack_start") == 0) {
		//�U���������true�ɂ���B
		m_isUnderAttack = true;
	}
	//�L�[�̖��O���uattack_end�v�̎��B
	else if (wcscmp(eventName, L"attack_end") == 0) {
		//�U���������false�ɂ���B
		m_isUnderAttack = false;
	}
}

const bool TankEnemy::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.LengthSq() <= 200.0f * 200.0f)
	{
		//�U���ł���I
		return true;
	}
	//�U���ł��Ȃ��B
	return false;
}


///�o�H
void TankEnemy::Aroute()
{
	//bool isEnd;
	if (m_tankNumber == 0)
	{
			//�ڕW�n�_�܂ł̃x�N�g��
			Vector3 diff = m_point->s_position - m_position;
			//�ڕW�n�_�ɋ߂�������
			if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
			{
				//�Ō�̖ڕW�n�_��������
				if (m_point->s_number == m_enemypath.GetPointListSize() - 1)
				{
					//�ŏ��̖ڕW�n�_��
					m_point = m_enemypath.GetFirstPoint();
				}
				//�Ō�̖ڕW�n�_�ł͂Ȃ�������
				else
				{
					//���̖ڕW�n�_��
					m_point = m_enemypath.GetNextPoint(m_point->s_number);
				}
			}
			//�ڕW�n�_�ɋ߂��Ȃ�������
			else
			{
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓G�l�~�[�̍��W�B
				start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
				//�I�_�͎��̃p�X�̍��W�B
				end.setOrigin(btVector3(m_point->s_position.x, m_point->s_position.y + 70.0f, m_point->s_position.z));

				SweepResultWall callback;
				//�R���C�_�[���n�_����I�_�܂œ������āB
				//�Փ˂��邩�ǂ����𒲂ׂ�B
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
				//�ǂƏՓ˂����I
				if (callback.isHit == true)
				{
					m_point = m_enemypath.GetNextPoint(m_point->s_number);
					return;
				}
				else {
					//���K��
					diff.Normalize();
					//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
					m_moveSpeed = diff * 120.0f;
					//Y���W�̈ړ����x��0�ɂ���
					m_moveSpeed.y = 0.0f;
				}
			
		}
	}
	if (m_tankNumber == 1)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point2->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point2->s_number == m_enemypath2.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point2 = m_enemypath2.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point = m_enemypath2.GetNextPoint(m_point->s_number);
			}
		}
		//�ڕW�n�_�ɋ߂��Ȃ�������
		else
		{
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point2->s_position.x, m_point2->s_position.y + 70.0f, m_point2->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point2 = m_enemypath2.GetNextPoint(m_point2->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 120.0f;
				//Y���W�̈ړ����x��0�ɂ���
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
		//���f����`�悷��B
		m_modelRender.Draw(rc);
	}

}