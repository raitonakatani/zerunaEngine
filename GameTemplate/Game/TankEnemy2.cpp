#include "stdafx.h"
#include "TankEnemy2.h"
#include "TankEnemy.h"

#include "Game.h"
#include "Player.h"
#include "EnemyPath.h"
#include "GameCamera.h"

#include "graphics/effect/EffectEmitter.h"


#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



TankEnemy2::TankEnemy2()
{

}

TankEnemy2::~TankEnemy2()
{

}

bool TankEnemy2::Start()
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
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/tankEnemy/tank.tkm", false, m_animationClips, enAnimationClip_Num);

	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	m_firstPosition = m_position;

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		60.0f,			//���a�B
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
//	m_nvmMesh.Init("Assets/nvm/test1.tkn");

	m_timer = 0.0f;

	return true;
}

void TankEnemy2::Update()
{


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

	Weak = m_player->GetPosition() - m_position;

	if (m_state == 0 && m_isSearchPlayer == false) {

		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 1.0f) {
			m_EnemyState = enEnemyState_Idle;
		}
		if (m_timer >= 1.0f) {
			m_EnemyState = enEnemyState_Chase;
			m_state = 1;
		}
	}

	if (m_camera->m_camera == 1)
	{
	//	m_EnemyState = enEnemyState_Idle;
	}

	//���f���̍X�V�B
	m_modelRender.Update();
}

void TankEnemy2::Rotation()
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

void TankEnemy2::Chase()
{
	//�v���C���[�������Ă��Ȃ�������B
	if (m_state == 1)
	{
		m_EnemyState = enEnemyState_Chase;
		Aroute();
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

void TankEnemy2::Collision()
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
	collisionObject->SetName("enemy2");


	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_porkattack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(collisionObject))
		{
			m_state = 2;
			m_hp = 0;
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
			m_state = 2;
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(collisionObject))
			{
				//m_camera->m_camera = 1;
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
				m_state = 2;
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

void TankEnemy2::Attack()
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

void TankEnemy2::SearchPlayer()
{
	m_isSearchPlayer = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));

	//�v���C���[�����E���ɋ��Ȃ�������B
	if (Math::PI * 0.40f <= fabsf(angle))
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

void TankEnemy2::MakeAttackCollision()
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

void TankEnemy2::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������B
	if (m_isSearchPlayer == true && diff.LengthSq() <= 1000.0 * 1000.0f)
	{
		m_state = 0;
		m_timer = 0.0f;

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
			if (ram >= 5)
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
	//�v���C���[���������Ȃ���΁B
	else
	{
		return;
	}
}

void TankEnemy2::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void TankEnemy2::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void TankEnemy2::ProcessChaseStateTransition()
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

void TankEnemy2::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void TankEnemy2::ProcessReceiveDamageStateTransition()
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

void TankEnemy2::ProcessDownStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_state = 3;
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

void TankEnemy2::ManageState()
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
	}
}

void TankEnemy2::PlayAnimation()
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
	default:
		break;
	}
}

void TankEnemy2::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

const bool TankEnemy2::IsCanAttack() const
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

void TankEnemy2::Render(RenderContext& rc)
{
	if (Weak.Length() <= 3000.0f)
	{
		//���f����`�悷��B
		m_modelRender.Draw(rc);
	}
}


///�o�H
void TankEnemy2::Aroute()
{

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
				m_point2 = m_enemypath2.GetNextPoint(m_point2->s_number);
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
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point4->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point4->s_number == m_enemypath4.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point4 = m_enemypath4.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point4 = m_enemypath4.GetNextPoint(m_point4->s_number);
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
			end.setOrigin(btVector3(m_point4->s_position.x, m_point4->s_position.y + 70.0f, m_point4->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point4 = m_enemypath4.GetNextPoint(m_point4->s_number);
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
}
void TankEnemy2::Broute()
{

}