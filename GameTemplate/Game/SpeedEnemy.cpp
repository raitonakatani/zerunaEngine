#include "stdafx.h"
#include "SpeedEnemy.h"

#include "Player.h"

#include "CollisionObject.h"

namespace
{
	const float CHARACON_RADIUS = 20.0f;            //�L�����R���̔��a
	const float CHARACON_HEIGHT =45.0f;             //�L�����R���̍���
	const float MODEL_POSITION_Y = 2.5f;            //���f����Y���W
	const float MOVESPEED_MINIMUMVALUE = 0.001f;    //�ړ����x�̍Œ�l
	const float MOVESPEED = 200.0f;                 //�ړ����x
	const float GRAVITY = 1000.0f;                  //�d��
	const float IDLETIMER_MAX = 0.3f;               //�ҋ@�^�C�}�[�̍ő�l
	const float IDLETIMER_DEFAULT = 0.0f;           //�ҋ@�^�C�}�[�̏����l
	const float CHASETIMER_MAX = 1.0f;              //�ǐՃ^�C�}�[�̍ő�l
	const float CHASETIMER_DEFAULT = 0.0f;          //�ǐՃ^�C�}�[�̏����l
	const float SEARCH_RANGE = 300.0f;              //�v���C���[�𔭌��ł��鋗��
	const float SEARCH_ANGLE = 130.0f;              //�v���C���[�𔭌��ł���p�x
	const float ATTACK_RANGE = 45;					//�U���ł��鋗��
	const int   ATTACK_PROBABILITY = 1;             //�U�����s���m��
	const float RECEIVE_DAMAGE = 1;                 //�v���C���[����󂯂�_���[�W
}

SpeedEnemy::SpeedEnemy()
{

}

SpeedEnemy::~SpeedEnemy()
{

}

void SpeedEnemy::InitAnimation()
{
	//�A�j���[�V���������[�h
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/speedenemy/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/speedenemy/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Attack].Load("Assets/animData/speedenemy/attack2.tka");
	m_animationClipArray[enAnimClip_Attack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Shout].Load("Assets/animData/speedenemy/shout.tka");
	m_animationClipArray[enAnimClip_Shout].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/speedenemy/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/speedenemy/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
}

bool SpeedEnemy::Start()
{
	//�A�j���[�V�����̏�����
	InitAnimation();

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm",
		m_animationClipArray,
		enAnimClip_Num);

	//�L�����R���̏�����
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�p���`�̃{�[��
	m_punchBoneId = m_modelRender.FindBoneID(L"LeftHand");

	m_player = FindGO<Player>("player");

	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void SpeedEnemy::Update()
{
	//�ǐՏ���
	Chase();
	//��]����
	Rotation();
	//�����蔻�菈��
	Collision();
	//�U������
	Attack();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();

	//���W�Ɗg�嗦�Ɖ�]��ݒ肷��
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(m_scale);
	m_modelRender.SetRotation(m_rotation);

	//���f���̍X�V
	m_modelRender.Update();
}

void SpeedEnemy::Chase()
{
	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_speedEnemyState != enSpeedEnemyState_Chase)
	{
		//�������Ȃ�
		return;
	}

	Vector3 diff = m_player->GetPosition() - m_position;
	//���K��
	diff.Normalize();
	//�ړ����x
	m_moveSpeed.x = diff.x * MOVESPEED;
	m_moveSpeed.z = diff.z * MOVESPEED;

	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}

	Vector3 modelPosition = m_position;
	//������Ƃ������f���̍��W��������B
	modelPosition.y += MODEL_POSITION_Y;
	m_modelRender.SetPosition(modelPosition);
}

void SpeedEnemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < MOVESPEED_MINIMUMVALUE
		&& fabsf(m_moveSpeed.z) < MOVESPEED_MINIMUMVALUE) {
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

void SpeedEnemy::Collision()
{
	//��_���[�W�X�e�[�g�ƃ_�E���X�e�[�g��������
	if (m_speedEnemyState == enSpeedEnemyState_ReceiveDamage ||
		m_speedEnemyState == enSpeedEnemyState_Down)
	{
		//�������Ȃ�
		return;
	}

	//�v���C���[�̍U���̓����蔻��
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision : collisions)
	{
		//�v���C���[�̍U���ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= RECEIVE_DAMAGE;
			//HP��0��������
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g�Ɉڍs����
				m_speedEnemyState = enSpeedEnemyState_Down;
			}
			//HP��0�ł͂Ȃ�������
			else
			{
				//��_���[�W�X�e�[�g�Ɉڍs����
				m_speedEnemyState = enSpeedEnemyState_ReceiveDamage;
			}
		}
	}
}

void SpeedEnemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ�������
	if (m_speedEnemyState != enSpeedEnemyState_Attack)
	{
		//�������Ȃ�
		return;
	}
	//�U������������
	if (m_isUnderAttack == true)
	{
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
}

void SpeedEnemy::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 100.0f;
	collisionPosition.y += 100.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,				  //���W�B
		Quaternion::Identity,                                     //��]�B
		Vector3(30.0f, 30.0f, 30.0f)                              //�傫���B
	);

	Matrix matrix = m_modelRender.GetBone(m_punchBoneId)->GetWorldMatrix();
	//���̃{�[���̃��[���h�s����R���W�����ɓK�p������
	collisionObject->SetName("speedenemy_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void SpeedEnemy::PlayAnimation()
{
	switch (m_speedEnemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.6f);
		break;
		//�ǐՃX�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
		m_modelRender.SetAnimationSpeed(1.4f);
		break;
		//�U���X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimClip_Attack, 0.2f);
		m_modelRender.SetAnimationSpeed(3.0f);
		break;
		//���уX�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Shout:
		m_modelRender.PlayAnimation(enAnimClip_Shout, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//��_���[�W�X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//�_�E���X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.2f);
		m_modelRender.SetAnimationSpeed(1.5f);
		break;
		//����ȊO�̎�
	default:
		break;
	}
}

void SpeedEnemy::ManageState()
{
	switch (m_speedEnemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//�ǐՃX�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//�U���X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
		//���уX�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Shout:
		ProcessShoutStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎�
	case SpeedEnemy::enSpeedEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//����ȊO�̎�
	default:
		break;
	}
}

void SpeedEnemy::ProcessCommonStateTransition()
{
	//�ҋ@�^�C�}�[�ƒǐՃ^�C�}�[������������
	m_idleTimer = IDLETIMER_DEFAULT;
	m_chaseTimer = CHASETIMER_DEFAULT;

	//�v���C���[�𔭌�������
	if (SearchPlayer() == true)
	{
		//�U���ł���Ȃ�
		if (IsCanAttack() == true)
		{
			int ram = rand() % 100;
			if (ram >= ATTACK_PROBABILITY)
			{
				//�U���X�e�[�g�Ɉڍs����
				m_speedEnemyState = enSpeedEnemyState_Attack;
				m_isUnderAttack = false;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g�Ɉڍs����
				m_speedEnemyState = enSpeedEnemyState_Idle;
				return;
			}
		}
		//�U���ł��Ȃ��Ȃ�
		else
		{
			//�ǐՃX�e�[�g�Ɉڍs����
			m_speedEnemyState = enSpeedEnemyState_Chase;
			return;
		}
	}
	//�v���C���[�𔭌��ł��Ȃ�������
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_speedEnemyState = enSpeedEnemyState_Idle;
		return;
	}
}

void SpeedEnemy::ProcessIdleStateTransition()
{
	//�ҋ@�^�C�}�[���J�E���g������
	m_idleTimer += g_gameTime->GetFrameDeltaTime();

	//�ҋ@�^�C�}�[��0.7f���傫��������
	if (m_idleTimer >= IDLETIMER_MAX)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ����Ɉڍs����
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessChaseStateTransition()
{
	//�U���ł��鋗���Ȃ�
	if (IsCanAttack() == true)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ����Ɉڍs����
		ProcessCommonStateTransition();
		return;
	}

	//�ǐՃ^�C�}�[���J�E���g������
	m_chaseTimer += g_gameTime->GetFrameDeltaTime();

	//�ǐՃ^�C�}�[��1.0f���傫��������
	if (m_chaseTimer >= CHASETIMER_MAX)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ����Ɉڍs����
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I����Ă�����
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ����Ɉڍs����
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessShoutStateTransition()
{
	//�A�j���[�V�����̍Đ����I����Ă�����
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ����Ɉڍs����
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessReceiveDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I����Ă�����
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ����Ɉڍs����
		ProcessCommonStateTransition();
		return;
	}
}

void SpeedEnemy::ProcessDownStateTransition()
{
	//�A�j���[�V�����̍Đ����I����Ă�����
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���g���폜����
		DeleteGO(this);
		return;
	}
}

const bool SpeedEnemy::SearchPlayer() const
{
	//�v���C���[�Ɍ������x�N�g��
	Vector3 diff = m_player->GetPosition() - m_position;
	//�v���C���[�ɂ�����x�߂�������B
	if (diff.LengthSq() <= SEARCH_RANGE * SEARCH_RANGE)
	{
		//�G�l�~�[����v���C���[�Ɍ������x�N�g���𐳋K������B
		diff.Normalize();
		//�G�l�~�[�̐��ʂ̃x�N�g���ƁA�G�l�~�[����v���C���[�Ɍ������x�N�g���́B
		//����(cos��)�����߂�B
		float cos = m_forward.Dot(diff);
		//����(cos��)����p�x(��)�����߂�B
		float angle = acosf(cos);
		//�p�x(��)��150����菬������΁B
		if (angle <= (Math::PI / 180.0f) * SEARCH_ANGLE)
		{
			//�v���C���[���������I
			return true;
		}
	}
	//�v���C���[���������Ȃ������B
	return false;
}

const bool SpeedEnemy::IsCanAttack() const
{
	//�v���C���[�Ɍ������x�N�g��
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

void SpeedEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U���t���O��true�ɂ���
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//�U���t���O��false�ɂ���
		m_isUnderAttack = false;
	}
}

void SpeedEnemy::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
}