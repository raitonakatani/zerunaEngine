#include "stdafx.h"
#include "Player.h"

#include "CollisionObject.h"

namespace
{
	const float CHARACON_RADIUS = 20.0f;            //�L�����R���̔��a
	const float CHARACON_HEIGHT = 60.0f;            //�L�����R���̍���
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;   //�ړ����x�̍Œ�l
	const float WALK_MOVESPEED = 60.0f;            //�����X�e�[�g�̈ړ����x
	const float RUN_MOVESPEED = 160.0f;            //����X�e�[�g�̈ړ����x
	const float STEALTHYSTEP_MOVESPEED = 30.0f;     //�E�ё��X�e�[�g�̈ړ����x
	const float GRAVITY = 1000.0f;                  //�d��
	const float ATTACK_TIME = 1.5f;                 //�A���U�����ł��鎞��
	const float AVOIDANCE_SPEED = 100.0f;           //����X�e�[�g�̈ړ����x
}

void Player::InitAnimation()
{
	//�A�j���[�V�����N���b�v�����[�h����B
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
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/firstattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_SecondAttack].Load("Assets/animData/player2/secondattack.tka");
	m_animationClipArray[enAnimClip_SecondAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ThirdAttack].Load("Assets/animData/player2/thirdattack.tka");
	m_animationClipArray[enAnimClip_ThirdAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_PokeAttack].Load("Assets/animData/player2/pokeattack.tka");
	m_animationClipArray[enAnimClip_PokeAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/player2/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/player2/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
}

bool Player::Start()
{
	//�A�j���[�V�������������B
	InitAnimation();

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/player/player2.tkm"
		, m_animationClipArray, enAnimClip_Num
	);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�L�����R��
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, g_vec3Zero);

	//���̃{�[��
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");
	return true;
}

void Player::Update()
{
	//�ړ�����
	Move();
	//��]����
	Rotation();
	//�����蔻�菈��
	Collision();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();
	//�U������
	Attack();
	//�A���U������
	Hit();
	//�������
	Avoidance();

	//���W�A��]�A�傫���̍X�V
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale({ 1.5f,1.5f,1.5 });
	
	//���f���̍X�V
	m_modelRender.Update();
}

void Player::Move()
{
	//�ҋ@�X�e�[�g�A�����X�e�[�g�A����X�e�[�g�ȊO��������
	if (m_playerState != enPlayerState_Run &&
		m_playerState != enPlayerState_Walk &&
		m_playerState != enPlayerState_StealthySteps &&
		m_playerState != enPlayerState_Idle)
	{
		//�Ȃɂ����Ȃ�
		return;
	}
	//�ړ����x
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�X�e�b�N�̓��͗ʂ��擾
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//camera�̑O�����ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//����X�e�[�g��������
	if (m_playerState == enPlayerState_Run)
	{
		m_moveSpeed += cameraForward * lStick_y * RUN_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * RUN_MOVESPEED;
	}

	else if(m_playerState == enPlayerState_StealthySteps){
		m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
	}
	//����ȊO��������
	else {
		m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;
	}

	//�d��
	//m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

	//�L�����R�����g�p���āA���W���X�V����
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//���W�̍X�V
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
	//��]��ݒ肷��
	m_modelRender.SetRotation(m_rotation);
	//�v���C���[�̐��ʃx�N�g�����v�Z����
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	//��_���[�W�X�e�[�g��������
	if (m_playerState == enPlayerState_ReceiveDamage || m_playerState == enPlayerState_Down ||
		m_playerState == enPlayerState_Avoidance)
	{
		//�������Ȃ�
		return;
	}

	//�X�s�[�h�G�l�~�[�̍U���̓����蔻��
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("speedenemy_attack");
	for (auto collision : collisions)
	{
		//�X�s�[�h�G�l�~�[�̍U���ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 1;
			//HP��0��������
			if (m_hp == 0)
			{
				//�_�E���X�e�[�g�Ɉڍs����
				m_playerState = enPlayerState_Down;
			}
			//HP��0�ł͂Ȃ�������
			else
			{
				//��_���[�W�X�e�[�g�Ɉڍs����
				m_playerState = enPlayerState_ReceiveDamage;
			}
		}
	}
}

void Player::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ��Ȃ�
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_SecondAttack &&
		m_playerState != enPlayerState_ThirdAttack)
	{
		//�U�����������Ȃ�
		return;
	}

	//�U�����蒆�ł���΁B
	if (m_isUnderAttack == true)
	{
		//MakeAttackEffect();
		//�U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
}

void Player::Hit()
{
	//1���ڂ̍U���X�e�[�g��������
	if (m_playerState == enPlayerState_FirstAttack)
	{
		//2���ڂ̍U���t���O��true�ɂ���
		m_secondAttackFlag = true;
	}
	//2���ڂ̍U���X�e�[�g��������
	if (m_playerState == enPlayerState_SecondAttack)
	{
		//3���ڂ̍U���t���O��true�ɂ���
		m_thirdAttackFlag = true;
	}

	//�Q���ڂ̍U���t���O��true��������
	if (m_secondAttackFlag == true)
	{
		//�Q���ڂ̍U���^�C�}�[�𑝉�������
		m_secondAttackTimer += g_gameTime->GetFrameDeltaTime();
		//�Q���ڂ̍U���^�C�}�[���J�n����Ă�����
		if (m_secondAttackTimer > 0.001f && m_secondAttackTimer < ATTACK_TIME)
		{
			//�Q���ڂ̍U���X�e�[�g�ֈڍs����
			m_attackState = enAttackState_SecondAttack;

		}
		else
		{
			//�P���ڂ̍U���X�e�[�g�ֈڍs����
			m_attackState = enAttackState_FirstAttack();
			//�Q���ڂ̍U���^�C�}�[������������
			m_secondAttackTimer = 0.0f;
			//�Q���ڂ̍U���t���O��false�ɂ���
			m_secondAttackFlag = false;
		}
	}
	//�R���ڂ̍U���t���O��true��������
	if (m_thirdAttackFlag == true)
	{
		//�R���ڂ̍U���^�C�}�[�𑝉�������
		m_thirdAttackTimer += g_gameTime->GetFrameDeltaTime();
		//�R���ڂ̍U���^�C�}�[�������Ă�����
		if (m_thirdAttackTimer > 0.001f && m_thirdAttackTimer < ATTACK_TIME)
		{
			//�R���ڂ̍U���X�e�[�g�ֈڍs����
			m_attackState = enAttackState_ThirdAttack;
		}
		else
		{
			//�P���ڂ̍U���X�e�[�g�ֈڍs����
			m_attackState = enAttackState_FirstAttack();
			//�Q���ڂ̍U���^�C�}�[�ƂR���ڂ̍U���^�C�}�[������������
			m_secondAttackTimer = 0.0f;
			m_thirdAttackTimer = 0.0f;
			//�Q���ڂ̍U���t���O�ƂR���ڂ̍U���t���O��false�ɂ���
			m_secondAttackFlag = false;
			m_thirdAttackFlag = false;
		}
	}
}

void Player::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//���W���v���C���[�̏����O�ɐݒ肷��B
	collisionPosition += m_forward * 100.0f;
	collisionPosition.y += 100.0f;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,				   //���W�B
		Quaternion::Identity,                                      //��]�B
		Vector3(10.0f, 10.0f, 60.0f)                              //�傫���B
	);

	Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
	//���̃{�[���̃��[���h�s����R���W�����ɓK�p������
	collisionObject->SetName("player_attack");
	collisionObject->SetWorldMatrix(matrix);
}

void Player::Avoidance()
{
	//����X�e�[�g�ȊO��������
	if (m_playerState != enPlayerState_Avoidance)
	{
		//�������Ȃ�
		return;
	}
	m_moveSpeed = m_forward * AVOIDANCE_SPEED;
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.5f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//�����X�e�[�g�̎�
	case Player::enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//�E�ё��X�e�[�g�̎�
	case Player::enPlayerState_StealthySteps:
		m_modelRender.PlayAnimation(enAnimClip_StealthySteps, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Avoidance:
		m_modelRender.PlayAnimation(enAnimClip_Rolling, 0.1f);
		m_modelRender.SetAnimationSpeed(1.5f);
		break;
		//1���ڂ̍U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		m_modelRender.PlayAnimation(enAnimClip_FirstAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//�Q���ڂ̍U���X�e�[�g�̎�
	case Player::enPlayerState_SecondAttack:
		m_modelRender.PlayAnimation(enAnimClip_SecondAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.4f);
		break;
		//�R���ڂ̍U���X�e�[�g�̎�
	case Player::enPlayerState_ThirdAttack:
		m_modelRender.PlayAnimation(enAnimClip_ThirdAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.6f);
		break;
		//�˂��U���X�e�[�g�̎�
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//��_���[�W�X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//�_�E���X�e�[�g�̎�
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
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
		//�����X�e�[�g�̎�
	case Player::enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		ProcessRunStateTransition();
		break;
		//�E�ё��X�e�[�g�̎�
	case Player::enPlayerState_StealthySteps:
		ProcessStealthyStepsStateTransition();
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Avoidance:
		ProcessAvoidanceStateTransition();
		break;
		//1���̍U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		ProcessAttackStateTransition();
		break;
		//2���ڂ̍U���X�e�[�g�̎�
	case Player::enPlayerState_SecondAttack:
		ProcessAttackStateTransition();
		break;
		//3���ڂ̍U���X�e�[�g�̎�
	case Player::enPlayerState_ThirdAttack:
		ProcessAttackStateTransition();
		break;
		//�˂��U���X�e�[�g�̎�
	case Player::enPlayerState_PokeAttack:
		ProcessAttackStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎�
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	default:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	//B�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//����X�e�[�g�ֈڍs����
		m_playerState = enPlayerState_Avoidance;
		return;
	}

	//RB1�{�^���������ꂽ�灕�U���X�e�[�g�P��������
	if (g_pad[0]->IsTrigger(enButtonRB1) && m_attackState == enAttackState_FirstAttack)
	{
		//�P���ڂ̍U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_FirstAttack;

		return;
	}

	//RB1�{�^���������ꂽ�灕�U���X�e�[�g�Q��������
	if (g_pad[0]->IsTrigger(enButtonRB1) && m_attackState == enAttackState_SecondAttack)
	{
		//�Q���ڂ̍U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_SecondAttack;
		return;
	}

	//RB1�{�^���������ꂽ�灕�U���X�e�[�g�R��������
	if (g_pad[0]->IsTrigger(enButtonRB1) && m_attackState == enAttackState_ThirdAttack)
	{
		//�R���ڂ̍U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_ThirdAttack;
		return;
	}

	//RB2�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonRB2))
	{
		//�˂��U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_PokeAttack;
		return;
	}

	//x��z�̈ړ����x����������
	else if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		//A�{�^���������ꂽ��
		if (g_pad[0]->IsPress(enButtonA))
		{
			//����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Run;
			return;
		}
		//X�{�^���������ꂽ��
		if (g_pad[0]->IsPress(enButtonX))
		{
			//�E�ё��X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_StealthySteps;
			return;
		}
		//�ǂ̃{�^����������Ȃ�������
		else
		{
			//�����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Walk;
			return;
		}
	}

	//x��z�̈ړ����x���Ȃ�������
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Idle;
		return;

	}
}

void Player::ProcessIdleStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void  Player::ProcessWalkStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessStealthyStepsStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessAvoidanceStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		DeleteGO(this);
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
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

void Player::Render(RenderContext& rc)
{
	//�h���[�B
	m_modelRender.Draw(rc);
}