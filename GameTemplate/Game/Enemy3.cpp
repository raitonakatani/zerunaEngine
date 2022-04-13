#include "stdafx.h"
#include "Enemy3.h"

#include "Game.h"
#include "Player.h"

//#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"


#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"



Enemy3::Enemy3()
{

}

Enemy3::~Enemy3()
{

}

bool Enemy3::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Damage].Load("Assets/animData/enemy/damage.tka");
	m_animationClips[enAnimationClip_Damage].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Down].Load("Assets/animData/enemy/sibou.tka");
	m_animationClips[enAnimationClip_Down].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/Enemy/enemy.tkm", m_animationClips, enAnimationClip_Num);

	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();

	//�L�����N�^�[�R���g���[���[���������B
	m_charaCon.Init(
		50.0f,			//���a�B
		70.0f,			//�����B
		m_position		//���W�B
	);

	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_Hand = m_modelRender.FindBoneID(L"Mutant:LeftHand");

	m_player = FindGO<Player>("player");
	m_game = FindGO<Game>("game");

	m_enemypath.Init("Assets/path/enemy/enemypath1.tkl");
	m_enemypath2.Init("Assets/path/enemy/enemypath2.tkl");
	m_enemypath3.Init("Assets/path/enemy/enemypath3.tkl");
	m_enemypath4.Init("Assets/path/enemy/enemypath4.tkl");

	m_point = m_enemypath.GetFirstPoint();
	m_point2 = m_enemypath2.GetFirstPoint();
	m_point3 = m_enemypath3.GetFirstPoint();
	m_point4 = m_enemypath4.GetFirstPoint();

	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void Enemy3::Update()
{
	g_Light.SetLigPoint({ m_position.x+100.0f,50.0f,m_position.z });
	
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

	//���f���̍X�V�B
	m_modelRender.Update();
}

void Enemy3::Rotation()
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

void Enemy3::Chase()
{

	//�v���C���[�������Ă��Ȃ�������B
	if (state == 1)
	{
		m_Enemy3State = enEnemy3State_Chase;
		Aroute();
	}

	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_Enemy3State != enEnemy3State_Chase)
	{
		return;
	}

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += 2.5f;
	m_modelRender.SetPosition(modelPosition);
}

void Enemy3::Collision()
{
	//��_���[�W�A���邢�̓_�E���X�e�[�g�̎��́B
	//�����蔻�菈���͂��Ȃ��B
	if (m_Enemy3State == enEnemy3State_ReceiveDamage ||
		m_Enemy3State == enEnemy3State_Down)
	{
		return;
	}



	{
		//�v���C���[�̍U���p�̃R���W�������擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
		//�R���W�����̔z���for���ŉ񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{

				m_hp -= 1;

				//HP��0�ɂȂ�����B
				if (m_hp <= 0)
				{
					//�_�E���X�e�[�g�ɑJ�ڂ���B
					m_Enemy3State = enEnemy3State_Down;
				}
				else {
					//��_���[�W�X�e�[�g�ɑJ�ڂ���B
					m_Enemy3State = enEnemy3State_ReceiveDamage;
				}
				return;
			}
		}

	}
}

void Enemy3::Attack()
{
	//�U���X�e�[�g�łȂ��Ȃ珈�������Ȃ��B
	if (m_Enemy3State != enEnemy3State_Attack)
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

void Enemy3::SearchPlayer()
{
	m_isSearchPlayer = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));

	//�v���C���[�����E���ɋ��Ȃ�������B
	if (Math::PI * 0.45f <= fabsf(angle))
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

void Enemy3::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_Hand)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(100.0f, 30.0f, 30.0f));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy3_attack");
	
}

void Enemy3::ProcessCommonStateTransition()
{
	//�e�^�C�}�[���������B
	m_idleTimer = 0.0f;
	m_chaseTimer = 0.0f;

	//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
	Vector3 diff = m_player->GetPosition() - m_position;

	//�v���C���[����������B
	if (m_isSearchPlayer == true && diff.LengthSq() <= 500.0 * 500.0f)
	{	
		state = 0;
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
			m_isUnderAttack = true;
			//�����ɂ���āA�U�����邩�ҋ@�����邩�����肷��B	
			int ram = rand() % 100;
			if (ram >= 5)
			{
				//�U���X�e�[�g�ɑJ�ڂ���B
				m_Enemy3State = enEnemy3State_Attack;
				//m_isUnderAttack = false;
				return;
			}
			else
			{
				//���@�U���X�e�[�g�ɑJ�ڂ���B
				m_Enemy3State = enEnemy3State_Attack;
				return;
			}

		}
		//�U���ł��Ȃ������Ȃ�B
		else
		{
			m_isUnderAttack = false;
			//�ǐՃX�e�[�g�ɑJ�ڂ���B
			m_Enemy3State = enEnemy3State_Chase;
			return;
		}
	}
	//�v���C���[���������Ȃ���΁B
	else
	{
		state = 1;
	}
}

void Enemy3::ProcessIdleStateTransition()
{
	m_idleTimer += g_gameTime->GetFrameDeltaTime();
	//�ҋ@���Ԃ�������x�o�߂�����B
	if (m_idleTimer >= 0.9f)
	{
		//���̃X�e�[�g�֑J�ڂ���B
		ProcessCommonStateTransition();
	}

}

void Enemy3::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���B
	ProcessCommonStateTransition();
}

void Enemy3::ProcessChaseStateTransition()
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

void Enemy3::ProcessAttackStateTransition()
{
	//�U���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���B
		ProcessCommonStateTransition();
	}
}

void Enemy3::ProcessReceiveDamageStateTransition()
{
	//��_���[�W�A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_Enemy3State = enEnemy3State_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 150.0f;
	}
}

void Enemy3::ProcessDownStateTransition()
{
	//�_�E���A�j���[�V�����̍Đ����I�������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}

void Enemy3::ManageState()
{
	switch (m_Enemy3State)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemy3State_Idle:
		//�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemy3State_Chase:
		//�ǐՃX�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g�̎��B
	case enEnemy3State_Attack:
		//�U���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemy3State_ReceiveDamage:
		//��_���[�W�X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemy3State_Down:
		//�_�E���X�e�[�g�̃X�e�[�g�J�ڏ����B
		ProcessDownStateTransition();
		break;
	}
}

void Enemy3::PlayAnimation()
{
	m_modelRender.SetAnimationSpeed(1.0f);
	switch (m_Enemy3State)
	{
		//�ҋ@�X�e�[�g�̎��B
	case enEnemy3State_Idle:
		//�ҋ@�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.5f);
		break;
		//�ǐՃX�e�[�g�̎��B
	case enEnemy3State_Chase:
		m_modelRender.SetAnimationSpeed(1.6f);
		//����A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.1f);
		break;
		//�U���X�e�[�g�̎��B
	case enEnemy3State_Attack:
		m_modelRender.SetAnimationSpeed(1.3f);
		//�U���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.1f);
		break;
		//��_���[�W�X�e�[�g�̎��B
	case enEnemy3State_ReceiveDamage:
		m_modelRender.SetAnimationSpeed(1.5f);
		//��_���[�W�A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Damage, 0.1f);
		break;
		//�_�E���X�e�[�g�̎��B
	case enEnemy3State_Down:
		//�_�E���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_Down, 0.1f);
		break;
	default:
		break;
	}
}

void Enemy3::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

const bool Enemy3::IsCanAttack() const
{
	Vector3 diff = m_player->GetPosition() - m_position;
	//�G�l�~�[�ƃv���C���[�̋������߂�������B
	if (diff.LengthSq() <= 100.0f * 100.0f)
	{
		//�U���ł���I
		return true;
	}
	//�U���ł��Ȃ��B
	return false;
}

void Enemy3::Render(RenderContext& rc)
{
		//���f����`�悷��B
		m_modelRender.Draw(rc);
}

///�o�H
void Enemy3::Aroute()
{


	if (m_enemyNumber == 0)
	{
		Vector3 diff = m_point->s_position - m_position;
		if (diff.Length() <= 50.0f) {
			if (m_point->s_number == m_enemypath.GetPointListSize() - 1) {
				m_point = m_enemypath.GetFirstPoint();
			}
			else {
				m_point = m_enemypath.GetNextPoint(m_point->s_number);
			}

		}

		Vector3 range = m_point->s_position - m_position;
		m_moveSpeed = range * 20.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}

	if (m_enemyNumber == 1)
	{
		Vector3 diff = m_point2->s_position - m_position;
		if (diff.Length() <= 50.0f) {
			if (m_point2->s_number == m_enemypath2.GetPointListSize() - 1) {
				m_point2 = m_enemypath2.GetFirstPoint();
			}
			else {
				m_point2 = m_enemypath2.GetNextPoint(m_point2->s_number);
			}

		}
		Vector3 range = m_point2->s_position - m_position;
		m_moveSpeed = range * 20.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}
	if (m_enemyNumber == 2)
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
		m_moveSpeed = range * 20.0f * g_gameTime->GetFrameDeltaTime();;
		m_moveSpeed.y = 0.0f;
	}
	if (m_enemyNumber == 3)
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