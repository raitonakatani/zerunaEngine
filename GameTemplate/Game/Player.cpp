#include "stdafx.h"
#include "Player.h"
#include "PAUSE.h"
#include "Game.h"

// EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

namespace
{
	// Vector3
	const Vector3 HP_SPRITE_POSITION = { -748.0f,420.0f ,0.0f };
	const Vector3 SUTAMINA_SPRITE_POSITION = { -748.0f, 372.0f, 0.0f };
	const Vector3 SP_SPRITE_POSITION = { -748.0f, 325.0f, 0.0f };
	// Vector2
	const Vector2 SPRITE_PIVOT_POSITION = { 0.0f,0.5f };	// �X�v���C�g�����_�[�̃s�{�b�g�B
	// float
	const float SPRITE_WIDTH = 500.0f;						// �X�v���C�g�����_�[�̉��T�C�Y�B
	const float SPRITE_HEIGHT = 27.0f;						// �X�v���C�g�����_�[�̏c�T�C�Y�B
	const float CHARACON_RADIUS = 30.0f;					// �L�����R���̔��a
	const float CHARACON_HEIGHT = 100.0f;					// �L�����R���̍���
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;			// �ړ����x�̍Œ�l
	const float COOLTIME_MOVESPEED = 100.0f;				// �����X�e�[�g�̈ړ����x
	const float WALK_MOVESPEED = 200.0f;					// �����X�e�[�g�̈ړ����x
	const float RUN_MOVESPEED = 400.0f;						// ����X�e�[�g�̈ړ����x
	const float STEALTHYSTEP_MOVESPEED = 100.0f;			// �E�ё��X�e�[�g�̈ړ����x
	const float GRAVITY = 1000.0f;							// �d��
	const float TANKENEMY_ATTACK = 80.0f;					// �^���N�G�l�~�[�̍U���́B
	const float SPPEDENEMY_ATTACK = 18.0f;					// �X�s�[�h�G�l�~�[�̍U���́B
}

void Player::InitAnimation()
{
	// �A�j���[�V�����N���b�v�����[�h����B
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/player2/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Walk].Load("Assets/animData/player2/walk.tka");
	m_animationClipArray[enAnimClip_Walk].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/player2/run.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_snake].Load("Assets/animData/player2/snakes.tka");
	m_animationClipArray[enAnimClip_snake].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_StealthySteps].Load("Assets/animData/player2/stealthysteps.tka");
	m_animationClipArray[enAnimClip_StealthySteps].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/slashattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_PokeAttack].Load("Assets/animData/player2/pokeattack2.tka");
	m_animationClipArray[enAnimClip_PokeAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/player2/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/player2/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Win].Load("Assets/animData/player2/win.tka");
	m_animationClipArray[enAnimClip_Win].SetLoopFlag(false);
}

bool Player::Start()
{
	// �A�j���[�V�������������B
	InitAnimation();

	// ���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/player/player2.tkm", false, m_animationClipArray, enAnimClip_Num);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	// �A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_swordJointBoneId = m_modelRender.FindBoneID(L"Sword_joint");
	m_swordBoneId = m_modelRender.FindBoneID(L"sword");


	// �T�E���h��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/11yoroi.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/audio/kiru.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/audio/tuki.wav");
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/audio/playerdamage.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/audio/playerdown.wav");
	// �L�����R��
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);

	// �G�t�F�N�g��ǂݍ��ށB
	EffectEngine::GetInstance()->ResistEffect(3, u"Assets/effect/efk/blood2.efk");
	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/star.efk");

	// �Q�[���̃N���X��T���Ă����Ă���B
	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{
	// �Q�[�����n�܂��ĂȂ��Ƃ��̏����B
	if (m_game->start == 0) {
		return;
	}

	// ���j���[��ʂ��J���Ă��鎞�̏����B
	if (g_pad[0]->IsTrigger(enButtonStart)		// Start�{�^���������ꂽ�B
		&& m_menu == false)						// ����m_menu==false�̎��B
	{
		m_menu = true;
		m_pause = NewGO<PAUSE>(0, "PASUE");
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		// Start�{�^���������ꂽ�B
		&& m_menu == true)							// ����m_menu==true�̎��B 
	{
		m_menu = false;
	}
	if (m_menu == true)
	{
		return;
	}



	// �ړ�����
	Move();
	// ��]����
	Rotation();
	// �����蔻�菈��
	Collision();
	// �A�j���[�V�����̍Đ�
	PlayAnimation();
	// �e�X�e�[�g�̑J�ڏ���
	ManageState();
	// �U������
	Attack();

	// ���W�A��]�A�傫���̍X�V
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale({ 2.0f,2.0f,2.0 });

	// ���f���̍X�V
	m_modelRender.Update();

	// HP�o�[�B
	if (m_hp >= 100.0f)
	{
		m_hp = 100.0f;
	}
	if (m_hp <= 0) {
		m_hp = 0;
	}
	m_maxHP = m_hp / 100.0f;



	// �X�^�~�i�o�[
	if (m_stamina <= 0.0f)
	{
		m_stamina = 0.0f;
	}
	if (m_stamina == 0.0f)
	{
		m_cooltime = true;
	}
	if (m_stamina >= 200.0f)
	{
		m_stamina = 200.0f;
	}

	// �N�[���^�C���B
	if (m_cooltime == true)
	{
		if (m_stamina >= 50.0f)
		{
			m_cooltime = false;
		}
	}

	// �摜�̑傫���ύX�B
	m_maxStamina = m_stamina / 200.0f;


	// SP�o�[�B
	if (g_pad[0]->IsPress(enButtonY) && m_sp >= 1) {
		--m_sp;
	}
	else if (m_sp <= 200) {
		m_sp += 0.2f;
	}
	m_maxSP = m_sp / 200.0f;


}

void Player::Move()
{
	// �ҋ@�X�e�[�g�A�����X�e�[�g�A����X�e�[�g�ȊO��������
	if (m_playerState != enPlayerState_Run &&
		m_playerState != enPlayerState_Walk &&
		m_playerState != enPlayerState_StealthySteps &&
		m_playerState != enPlayerState_snake &&
		m_playerState != enPlayerState_Idle ||
		prok == true)
	{
		// �Ȃɂ����Ȃ�
		return;
	}
	// �ړ����x
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	// �X�e�b�N�̓��͗ʂ��擾
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	// camera�̑O�����ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	// XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();


	// �N���A���̏����B
	if (GetPlayerHaveIndex() == 3)
	{
		m_playerState = enPlayerState_Win;
	}

	// �ʏ펞�̏����B
	if (GetPlayerHaveIndex() <= 2) {
		m_startTimer += g_gameTime->GetFrameDeltaTime();

		// �Q�[���J�n���̏����B
		if (m_startTimer >= 1.0f && m_startTimer <= 2.7f)
		{
			m_moveSpeed.z += WALK_MOVESPEED;
		}
		// �ʏ펞�̏����B
		else if (m_startTimer > 2.7f) {
			// �N�[���^�C���̏����B
			if (m_cooltime == true)
			{
				m_moveSpeed += cameraForward * lStick_y * COOLTIME_MOVESPEED;				// �������ւ̈ړ����x�����Z�B
				m_moveSpeed += cameraRight * lStick_x * COOLTIME_MOVESPEED;					// �E�����ւ̈ړ����x�����Z�B
				m_stamina++;
			}
			if (m_cooltime == false)
			{
				// ����X�e�[�g��������
				if (m_playerState == enPlayerState_Run)
				{
					m_moveSpeed += cameraForward * lStick_y * RUN_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * RUN_MOVESPEED;
					m_stamina -= 1.5f;
				}
				// ���Ⴊ�݃X�e�[�g��������
				else if (m_playerState == enPlayerState_StealthySteps || m_playerState == enPlayerState_snake) {
					m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
					m_stamina -= 0.5f;
				}
				// ����ȊO��������
				else {
					m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;
					m_stamina++;
				}
			}
		}
	}
	// �d��
	m_moveSpeed.y -= GRAVITY * g_gameTime->GetFrameDeltaTime();
	// �L�����R�����g�p���āA���W���X�V����
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	// ���W�̍X�V
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
	// ��]��ݒ肷��
	m_modelRender.SetRotation(m_rotation);
	// �v���C���[�̐��ʃx�N�g�����v�Z����
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}

void Player::Collision()
{
	// ��_���[�W�X�e�[�g��������
	if (m_playerState == enPlayerState_ReceiveDamage|| m_playerState == enPlayerState_Down)
	{
		// �������Ȃ�
		return;
	}
	// �G�l�~�[�i�^���N�j�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	// �R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		// �R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= TANKENEMY_ATTACK;
			if (m_hp <= 0)
			{
				// �_�E���X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_Down;
			}
			else {
				// ��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}
	// �G�l�~�[�i�X�s�[�h�j�̍U���p�̃R���W�������擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("Speedenemy_attack");
	// �R���W�����̔z���for���ŉ񂷁B
	for (auto collision2 : collisions2)
	{
		// �R���W�����ƃL�����R�����Փ˂�����B
		if (collision2->IsHit(m_charaCon))
		{
			m_hp -= SPPEDENEMY_ATTACK;
			if (m_hp <= 0)
			{
				// �_�E���X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_Down;
			}
			else {
				// ��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}
}

void Player::Attack()
{
	// �U���X�e�[�g�ł͂Ȃ��Ȃ�
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_PokeAttack)
	{
		// �U�����������Ȃ�
		return;
	}
	// �U�����蒆�ł���΁B
	if (m_isUnderAttack == true)
	{
		// �U���p�̃R���W�������쐬����B
		MakeAttackCollision();
	}
}

void Player::MakeAttackCollision()
{
	// �R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	Vector3 collisionPosition = m_position;
	// �{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(collisionPosition,				   // ���W�B
		Quaternion::Identity,                                      // ��]�B
		Vector3(20.0f, 20.0f, 120.0f)                              // �傫���B
	);
	Matrix matrix = m_modelRender.GetBone(m_swordJointBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);
	if (m_playerState == enPlayerState_PokeAttack) {
		// �˂��U����������B
		collisionObject->SetName("player_porkattack");
	}
	else {
		// �ʏ�U����������B
		collisionObject->SetName("player_attack");
	}
}


void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		// �ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		// �����X�e�[�g�̎�
	case Player::enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		// ����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		// ���Ⴊ�݃X�e�[�g�̎�
	case Player::enPlayerState_snake:
		m_modelRender.PlayAnimation(enAnimClip_snake, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// �E�ё��X�e�[�g�̎�
	case Player::enPlayerState_StealthySteps:
		m_modelRender.PlayAnimation(enAnimClip_StealthySteps, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// �U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		m_modelRender.PlayAnimation(enAnimClip_FirstAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		// �˂��U���X�e�[�g�̎�
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.4f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// ��_���[�W�X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		m_modelRender.PlayAnimation(enAnimClip_ReceiveDamage, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// �_�E���X�e�[�g�̎�
	case Player::enPlayerState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		// �_�E���X�e�[�g�̎�
	case Player::enPlayerState_Win:
		m_modelRender.PlayAnimation(enAnimClip_Win, 0.2f);
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
		// �ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
		// �����X�e�[�g�̎�
	case Player::enPlayerState_Walk:
		ProcessWalkStateTransition();
		break;
		// ����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		ProcessRunStateTransition();
		break;
		// ���Ⴊ�݃X�e�[�g�̎�
	case Player::enPlayerState_snake:
		ProcesssnakeStateTransition();
		break;
		// �E�ё��X�e�[�g�̎�
	case Player::enPlayerState_StealthySteps:
		ProcessStealthyStepsStateTransition();
		break;
		// 1���̍U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		ProcessAttackStateTransition();
		break;
		// �˂��U���X�e�[�g�̎�
	case Player::enPlayerState_PokeAttack:
		ProcessAttackStateTransition();
		break;
		// ��_���[�W�X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		// �_�E���X�e�[�g�̎�
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
		// �����X�e�[�g�̎�
	case Player::enPlayerState_Win:
		ProcessWinStateTransition();
		break;
	default:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (prok == true)
	{
		m_playerState = enPlayerState_Idle;
		return;
	}
	if (m_startTimer >= 1.0f && m_startTimer <= 2.7f)
	{
		m_playerState = enPlayerState_Walk;
		return;
	}

	// RB1�{�^���������ꂽ�灕�U���X�e�[�g�P��������
	if (g_pad[0]->IsTrigger(enButtonRB1))
	{
		// �U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_FirstAttack;
		m_isUnderAttack = false;
		return;
	}

	if (m_playerState != enPlayerState_PokeAttack && g_pad[0]->IsTrigger(enButtonRB2))
	{
		// �R���W�����I�u�W�F�N�g���쐬����B
		auto collisionObject = NewGO<CollisionObject>(0);
		Vector3 collisionPosition = m_position;
		collisionPosition += m_forward * 100.0f;
		collisionPosition.y += 100.0f;
		// �{�b�N�X��̃R���W�������쐬����B
		collisionObject->CreateBox(collisionPosition,		 // ���W�B
			m_rotation,                                      // ��]�B
			Vector3(40.0f, 30.0f, 140.0f)                    // �傫���B
		);
		collisionObject->SetName("player");
		// �˂��U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_PokeAttack;
		return;
	}

	// x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{

		if (m_cooltime == true)
		{
			// �����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Walk;
			return;
		}
		else {
			// A�{�^���������ꂽ��
			if (g_pad[0]->IsPress(enButtonA))
			{
				// ����X�e�[�g�ֈڍs����
				m_playerState = enPlayerState_Run;
				st = 1;
				return;
			}
			// X�{�^���������ꂽ��
			if (g_pad[0]->IsPress(enButtonX))
			{
				// �E�ё��X�e�[�g�ֈڍs����
				m_playerState = enPlayerState_StealthySteps;
				return;
			}

			// ������Ȃ�������
			else
			{
				// �����X�e�[�g�ֈڍs����
				m_playerState = enPlayerState_Walk;
			}
		}
	}
	// x��z�̈ړ����x���Ȃ�������
	else
	{
		// X�{�^���������ꂽ��
		if (g_pad[0]->IsPress(enButtonX))
		{
			// �E�ё��X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_snake;
			return;
		}
		else {
			// �ҋ@�X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_Idle;
		}
		st = 0;
		return;
	}
}

void Player::ProcessIdleStateTransition()
{
	// ���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void  Player::ProcessWalkStateTransition()
{
	st = 0;
	// ���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	// ���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcesssnakeStateTransition()
{
	// ���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessStealthyStepsStateTransition()
{
	// ���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	// �A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		// ���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	if (m_hitSE == 0) {
		// ���ʉ����Đ�����B
		SoundSource* damagese = NewGO<SoundSource>(0);
		damagese->Init(13);
		damagese->Play(false);
		damagese->SetVolume(0.6f);
		m_hitSE = 1;
	}
	// �A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_hitSE = 0;
		// ���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	m_downTimer += g_gameTime->GetFrameDeltaTime();
	// �A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_down = true;
	}
	if (m_hitSE == 0) {
		// ���ʉ����Đ�����B
		SoundSource* damagese = NewGO<SoundSource>(0);
		damagese->Init(13);
		damagese->Play(false);
		damagese->SetVolume(0.8f);
		m_hitSE = 1;
	}
	if (m_deathSE == 0 && m_downTimer >= 1.2f) {
		// ���ʉ����Đ�����B
		SoundSource* downse = NewGO<SoundSource>(0);
		downse->Init(14);
		downse->Play(false);
		downse->SetVolume(0.8f);
		m_deathSE = 1;
	}
	if (m_deathSE == 1 && m_downTimer >= 2.5f) {
		// ���ʉ����Đ�����B
		SoundSource* downse = NewGO<SoundSource>(0);
		downse->Init(14);
		downse->Play(false);
		downse->SetVolume(0.8f);
		m_deathSE = 2;
	}
}


void Player::ProcessWinStateTransition()
{
	// �A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false) {
		SetPlayerHaveIndex(4);
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		// ���ʉ����Đ�����B
		SoundSource* slashse = NewGO<SoundSource>(0);
		slashse->Init(11);
		slashse->Play(false);
		slashse->SetVolume(0.6f);
		// �U���t���O��true�ɂ���
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		// �U���t���O��false�ɂ���
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"porkattack_start") == 0)
	{
		if (m_critical == 1) {
			// �G�t�F�N�g���Đ�����B
			Matrix matrix = m_modelRender.GetBone(m_swordBoneId)->GetWorldMatrix();
			Vector3 m_effectPosition = m_position;
			m_effectPosition.y += 100.0f;
			// �G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(2);
			m_effectEmitter->SetPosition(m_effectPosition);
			// �G�t�F�N�g�̑傫����ݒ肷��B
			m_effectEmitter->SetScale(m_scale * 5.0f);
			m_effectEmitter->Play();
			m_effectEmitter->SetWorldMatrix(matrix);
		}
	}
	else if (wcscmp(eventName, L"porkattack") == 0)
	{
		// ���ʉ����Đ�����B
		SoundSource* slashse = NewGO<SoundSource>(0);
		slashse->Init(12);
		slashse->Play(false);
		slashse->SetVolume(0.6f);
		// �U���t���O��true�ɂ���
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"porkattack_end") == 0)
	{
		if (prok == true) {
			// �G�t�F�N�g���Đ�����B
			Matrix matrix = m_modelRender.GetBone(m_swordJointBoneId)->GetWorldMatrix();
			Vector3 m_effectPosition = m_position;
			m_effectPosition.y += 100.0f;
			// �G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(3);
			m_effectEmitter->SetPosition(m_effectPosition);
			// �G�t�F�N�g�̑傫����ݒ肷��B
			m_effectEmitter->SetScale(m_scale * 50.0f);
			m_effectEmitter->Play();
			m_effectEmitter->SetWorldMatrix(matrix);
		}
		m_critical = 0;
		// �U���t���O��false�ɂ���
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"Run_step") == 0) {
		// �����B
		// ���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.8f);
	}

	if (wcscmp(eventName, L"Walk_step") == 0) {
		// �����B
		// ���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.5f);
	}

	if (wcscmp(eventName, L"snake_step") == 0) {
		// �����B
		// ���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.3f);
	}
}

void Player::Render(RenderContext& rc)
{
	if (m_game->start == 0) {
		return;
	}
	// ���f�����h���[�B
	m_modelRender.Draw(rc);
}