#include "stdafx.h"
#include "Player.h"


namespace
{
	const float CHARACON_RADIUS = 20.0f;            //�L�����R���̔��a
	const float CHARACON_HEIGHT = 70.0f;            //�L�����R���̍���
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;   //�ړ����x�̍Œ�l
	const float WALK_MOVE_SPEED = 100.0f;            //�����X�e�[�g�̈ړ����x
	const float RUN_MOVE_SPEED = 300.0f;            //����X�e�[�g�̈ړ����x
	const float STEALTHYSTEP_MOVESPEED = 80.0f;     //�E�ё��X�e�[�g�̈ړ����x
	const float GRAVITY = 2000.0f;                  //�d��
	const float ATTACK_TIMER_SPEED = 50.0f;         //�U���^�C�}�[�̉������x
	const float ATTACK_TIME = 12.0f;                //�A���U�����ł��鎞��
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
	m_animationClipArray[enAnimClip_StealthyStep].Load("Assets/animData/player2/stealthysteps.tka");
	m_animationClipArray[enAnimClip_StealthyStep].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Rolling].Load("Assets/animData/player2/rolling.tka");
	m_animationClipArray[enAnimClip_Rolling].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/firstattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
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

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"Sword_joint");

	//�L�����R��
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, g_vec3Zero);

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
	//�ҋ@�X�e�[�g�A�����X�e�[�g�A����X�e�[�g�A�E�ѕ����X�e�[�g�ȊO��������
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_FirstAttack ||
		m_playerState == enPlayerState_PokeAttack||
		m_playerState == enPlayerState_Avoidance||
		m_playerState == enPlayerState_Down)
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
		m_moveSpeed += cameraForward * lStick_y * RUN_MOVE_SPEED;
		m_moveSpeed += cameraRight * lStick_x * RUN_MOVE_SPEED;
	}

	else if (m_playerState == enPlayerState_StealthyStep) {
		m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
		m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
	}

	//����ȊO��������
	else {
		m_moveSpeed += cameraForward * lStick_y * WALK_MOVE_SPEED;
		m_moveSpeed += cameraRight * lStick_x * WALK_MOVE_SPEED;
	}

	//�d��
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();

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
	//��_���[�W�X�e�[�g���_�E���X�e�[�g��������
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down)
	{
		//�������Ȃ�
		return;
	}

	//�G�l�~�[�i�{�X�j�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//��_���[�W�X�e�[�g�ɑJ�ڂ���B
			m_playerState = enPlayerState_ReceiveDamage;
			return;
		}
	}

	//�G�l�~�[�i�{�X�j�̍U���p�̃R���W�������擾����B
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("enemy3_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			return;
		}
	}
	//�X�s�[�h�G�l�~�[�̍U���̓����蔻��
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("speedenemy_attack");
	for (auto collision : collisions4)
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
	if (m_playerState != enPlayerState_FirstAttack)
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

void Player::MakeAttackCollision()
{
	//�R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);

	Vector3 collisionPosition = m_position;
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,				   //���W�B
		Quaternion::Identity,                                      //��]�B
		Vector3(20.0f, 20.0f, 100.0f)                              //�傫���B
	);
	collisionObject->SetName("player_attack");

	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
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

	//�d��
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	m_moveSpeed = m_forward * AVOIDANCE_SPEED;
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//�����X�e�[�g�̎�
	case Player::enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
	case Player::enPlayerState_StealthyStep:
		m_modelRender.PlayAnimation(enAnimClip_StealthyStep, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//��_���X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.1f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
	case Player::enPlayerState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.1f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Avoidance:
		m_modelRender.PlayAnimation(enAnimClip_Rolling, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//1���ڂ̍U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		m_modelRender.PlayAnimation(enAnimClip_FirstAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.1f);
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
	case Player::enPlayerState_StealthyStep:
		ProcessStealthyStepStateTransition();
		break;
		//��_���X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		ProcessDamageStateTransition();
		break;
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Avoidance:
		ProcessAvoidanceStateTransition();
		break;
		//1���̍U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		ProcessAttackStateTransition();
		break;
	case Player::enPlayerState_PokeAttack:
		ProcessAttackStateTransition();
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

	//RB1�{�^���������ꂽ��
	if (g_pad[0]->IsPress(enButtonRB1))
	{
		//�P���ڂ̍U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_FirstAttack;
		m_isUnderAttack = true;
		return;
	}
	else if (g_pad[0]->IsTrigger(enButtonLB1))
	{
		//�P���ڂ̍U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_PokeAttack;
		m_isUnderAttack = true;
		return;
	}
	else {
		m_isUnderAttack = false;
	}

	//x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		//A�{�^���������ꂽ��
		if (g_pad[0]->IsPress(enButtonA))
		{
			//����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Run;
			return;
		}
		//L�X�e�b�N���������񂾂�
		else if (g_pad[0]->IsPress(enButtonLB3))
		{
			//�E�ё��X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_StealthyStep;
			return;
		}
		//������Ȃ�������
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

void Player::ProcessStealthyStepStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessDamageStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessDownStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���̃X�e�[�g�ɑJ�ڂ���
		DeleteGO(this);
	}
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

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
/*	(void)clipName;
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
	*/
}

void Player::Render(RenderContext& rc)
{
	//�h���[�B
	m_modelRender.Draw(rc);
}