#include "stdafx.h"
#include "TankEnemy.h"
#include "Game.h"
#include "Player.h"
#include "EnemyPath.h"
#include "GameCamera.h"
#include "graphics/effect/EffectEmitter.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include "box.h"


namespace
{
	const float CHARACON_RADIUS = 40.0f;            //�L�����R���̔��a
	const float CHARACON_HEIGHT = 140.0f;           //�L�����R���̍���
	const float MOVESPEED = 150.0f;                 //�ړ����x
	const float ATTACK_RANGE = 200.0f;				//�U���ł��鋗��
	const float RECEIVE_DAMAGE = 50;                //�v���C���[����󂯂�_���[�W
	const float TARGET = 100.0f;					//���̃p�X�܂ł̋�����
}


TankEnemy::TankEnemy()
{

}

TankEnemy::~TankEnemy()
{
	const auto& m_boxs = FindGOs<box>("box");
	for (auto m_box : m_boxs)
	{
		DeleteGO(m_box);
	}

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
	m_modelRender.Init("Assets/modelData/tankEnemy/tank.tkm", false, m_animationClips, enAnimationClip_Num);

	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_position.y = 15.0f;
	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		CHARACON_RADIUS,			//���a�B
		CHARACON_HEIGHT,			//�����B
		m_position					//���W�B
	);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_Hand = m_modelRender.FindBoneID(L"LeftHand");
	m_weakness = m_modelRender.FindBoneID(L"Spine");

	//�N���X��T���Ă����Ă���B
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	//�p�X
	m_enemypath[1].Init("Assets/path/tank/enemypath1.tkl");
	m_enemypath[2].Init("Assets/path/tank/enemypath2.tkl");
	m_enemypath[3].Init("Assets/path/tank/enemypath3.tkl");
	m_enemypath[4].Init("Assets/path/tank/enemypath4.tkl");
	m_enemypath[5].Init("Assets/path/tank/enemypath5.tkl");
	m_enemypath[6].Init("Assets/path/tank/enemypath6.tkl");
	m_enemypath[7].Init("Assets/path/tank/enemypath7.tkl");
	m_enemypath[8].Init("Assets/path/tank/enemypath8.tkl");
	m_enemypath[9].Init("Assets/path/tank/enemypath9.tkl");
	m_enemypath[10].Init("Assets/path/tank/enemypath10.tkl");
	m_enemypath[11].Init("Assets/path/tank/enemypath11.tkl");
	m_enemypath[12].Init("Assets/path/tank/enemypath12.tkl");
	//�|�C���g
	m_point[1] = m_enemypath[1].GetFirstPoint();
	m_point[2] = m_enemypath[2].GetFirstPoint();
	m_point[3] = m_enemypath[3].GetFirstPoint();
	m_point[4] = m_enemypath[4].GetFirstPoint();
	m_point[5] = m_enemypath[5].GetFirstPoint();
	m_point[6] = m_enemypath[6].GetFirstPoint();
	m_point[7] = m_enemypath[7].GetFirstPoint();
	m_point[8] = m_enemypath[8].GetFirstPoint();
	m_point[9] = m_enemypath[9].GetFirstPoint();
	m_point[10] = m_enemypath[10].GetFirstPoint();
	m_point[11] = m_enemypath[11].GetFirstPoint();
	m_point[12] = m_enemypath[12].GetFirstPoint();


	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);


	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//�{�b�N�X�̃C���X�^���X�𐶐�����B
	m_box = NewGO<box>(0, "box");
	m_box->m_position = { m_position.x,250.0f ,m_position.z };

	return true;
}

void TankEnemy::Update()
{
	if (m_player->GetPlayerHaveIndex() >= 1) {
		return;
	}

	
	if (m_hp > 0) {
		m_box->m_position = { m_position.x,250.0f ,m_position.z };
		if (m_box->m_extract == 1) {
			m_box->m_question = 0;
		}
	}
	else {
		m_box->m_extract = 0;
		m_box->m_question = 0;
	}

	if (m_player->m_down == true && m_hp >= 1)
	{
		m_EnemyState = enEnemyState_look;
	}

	Weak = m_player->GetPosition() - m_position;
	if (Weak.Length() >= 3000.0f)
	{
		return;
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



	//����p�Ǝ��F�����B
	if (alertLevel == 0)
	{
		m_angl = 0.40f;
		m_range = 1200.0f;
	}
	if (alertLevel == 1)
	{
		m_angl = 0.45f;
		m_range = 1500.0f;
	}
	if (alertLevel == 2)
	{
		m_angl = 0.50f;
		m_range = 1800.0f;
	}
	if (alertLevel == 3)
	{
		m_angl = 0.5f;
		m_range = 1800.0f;
	}

	if (m_EnemyState == enEnemyState_alert)
	{
		m_angl = 0.6f;
	}
	if (state == 0 && m_isSearchPlayer == false) {

		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 0.5f) {
			m_EnemyState = enEnemyState_Idle;
		}
		if (m_timer >= 0.5f) {
			m_EnemyState = enEnemyState_Chase;
			state = 1;
		}
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

	}
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

void TankEnemy::Chase()
{
	//�v���C���[�������Ă��Ȃ�������B
	if (state == 1 && alertLevel != 2)
	{
		//�v���C���[�Ƃ̋�����800.0f�ȉ����A�v���C���[�����𗧂ĂĂ�����
		if (Weak.Length() <= 800.0f && m_player->st == 1)
		{
			//��������n���B	
			m_EnemyState = enEnemyState_alert;
			if (alertLevel == 0) {
				m_box->m_question = 1;
				alertLevel = 1;
			}
		}
		else if (alertLevel == 0 || alertLevel == 3) {
			m_box->m_question = 0;
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

void TankEnemy::Collision()
{
	if (m_hp <= 0) {
		return;
	}
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
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(40.0f, 40.0f, 40.0f));
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
			m_player->prok = true;
			state = 2;
			m_hp = 0;
			if (m_isSearchPlayer == false) {
				//�_�E���X�e�[�g(�O)�ɑJ�ڂ���B
				m_EnemyState = enEnemyState_Death;
				return;
			}
			else {
				//�_�E���X�e�[�g(���)�ɑJ�ڂ���B
				m_EnemyState = enEnemyState_Down;
			}
		}
	}

	{

		//�v���C���[�̍U���p�̃R���W�������擾����B
		const auto& collisions5 = g_collisionObjectManager->FindCollisionObjects("player");
		//�R���W�����̔z���for���ŉ񂷁B
		for (auto collision5 : collisions5)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision5->IsHit(collisionObject))
			{
				m_box->m_extract = 0;
				m_box->m_question = 0;
				m_player->SetHitCritical(1);
				m_EnemyState = enEnemyState_Idle;
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
	collisionObject->CreateBox(m_position,
		Quaternion::Identity,
		Vector3(100.0f, 100.0f, 100.0f)
	);


	Matrix matrix = m_modelRender.GetBone(m_Hand)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");

}

void TankEnemy::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������B
	if (m_isSearchPlayer == true && diff.LengthSq() <= m_range * m_range)
	{
		m_box->m_extract = 1;

		if (hakken == 0 && m_player->GetHP() > 0) {
			SoundSource* SE;
			SE = NewGO<SoundSource>(0);
			SE->Init(16);
			SE->Play(false);
			SE->SetVolume(0.8f);
			hakken = 1;
		}

		m_game->Bgmspeed = true;
		state = 0;
		m_timer = 0.0f;
		alertLevel = 2;
		mikke = true;
		targetpos = m_player->GetPosition();

		//�x�N�g���𐳋K������B
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * MOVESPEED;

		Vector3 toPlayerDir = diff;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);

		if (firstlook == 0) {
			SoundSource* SE;
			SE = NewGO<SoundSource>(0);
			SE->Init(10);
			SE->SetVolume(1.5f);
			SE->Play(false);
			SE->SetFrequencyRatio(0.7f);
			m_EnemyState = enEnemyState_look;
			return;
		}

		//�U���ł��鋗���Ȃ�B
		if (IsCanAttack() == true)
		{
			//�U���X�e�[�g�ɑJ�ڂ���B
			m_EnemyState = enEnemyState_Attack;
			return;
		}
		//�U���ł��Ȃ������Ȃ�B
		else
		{
			//�ǐՃX�e�[�g�ɑJ�ڂ���B
			m_EnemyState = enEnemyState_Chase;
			return;
		}
	}
	else if (m_isSearchPlayer == false && mikke == true) {
		Vector3 diff2 = targetpos - m_position;
		if (diff2.LengthSq() <= 100.0f * 100.0f) {
				mikke = false;
		}

		//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
		Vector3 diff1 = targetpos - m_position;
		//�x�N�g���𐳋K������B
		diff1.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff1 * MOVESPEED;

		Vector3 toPlayerDir = diff1;
		m_forward = toPlayerDir;

		m_rotation.SetRotationY(atan2(m_forward.x, m_forward.z));
		m_modelRender.SetRotation(m_rotation);
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		m_box->m_extract = 0;
		m_box->m_extractanim = 0;

		m_game->Bgmspeed = false;
		hakken = 0;
		if (alertLevel == 2)
		{
			alertLevel = 3;
		}
		else {
			alertLevel = 0;
		}
		return;
	}
}

void TankEnemy::ProcessIdleStateTransition()
{
	//�v���C���[�������ĂȂ��Ȃ�B
	if (m_player->GetHitCritical() == 0)
	{
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
	if (m_chaseTimer >= 0.2f)
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
		if (m_player->GetHP() >= 1) {
			m_EnemyState = enEnemyState_Chase;
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
		m_moveSpeed = diff * MOVESPEED;
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
}

void TankEnemy::lookTransition()
{
	firstlook = 1;
	//�x��A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
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
		alertLevel = 0;
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
		//�ǐՃA�j���[�V�������Đ��B
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
		m_modelRender.SetAnimationSpeed(0.8f);
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
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//���K�X�e�[�g�̎��B
	case enEnemyState_alert:
		//���K�A�j���[�V�������Đ��B
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
	if (diff.LengthSq() <= ATTACK_RANGE * ATTACK_RANGE)
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
	//�G�l�~�[�̐������J��Ԃ��B
	for (int i = 1; i <= m_game->GettankSize(); i++)
	{
		if (m_tankNumber == i-1) {

			//�ڕW�n�_�܂ł̃x�N�g��
			Vector3 diff = m_point[i]->s_position - m_position;
			//�ڕW�n�_�ɋ߂�������
			if (diff.LengthSq() <= TARGET * TARGET * g_gameTime->GetFrameDeltaTime())
			{
				//�Ō�̖ڕW�n�_��������
				if (m_point[i]->s_number == m_enemypath[i].GetPointListSize() - 1)
				{
					//�ŏ��̖ڕW�n�_��
					m_point[i] = m_enemypath[i].GetFirstPoint();
				}
				//�Ō�̖ڕW�n�_�ł͂Ȃ�������
				else
				{
					//���̖ڕW�n�_��
					m_point[i] = m_enemypath[i].GetNextPoint(m_point[i]->s_number);
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
				end.setOrigin(btVector3(m_point[i]->s_position.x, m_point[i]->s_position.y + 70.0f, m_point[i]->s_position.z));

				SweepResultWall callback;
				//�R���C�_�[���n�_����I�_�܂œ������āB
				//�Փ˂��邩�ǂ����𒲂ׂ�B
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
				//�ǂƏՓ˂����I
				if (callback.isHit == true)
				{
					m_point[i] = m_enemypath[i].GetNextPoint(m_point[i]->s_number);
					return;
				}
				else {
					//���K��
					diff.Normalize();
					//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
					m_moveSpeed = diff * MOVESPEED;
				}

			}

		}
	}
}

void TankEnemy::Render(RenderContext& rc)
{
	if (Weak.Length() <= 3000.0f)
	{
		//���f����`�悷��B
		m_modelRender.Draw(rc);
	}
}