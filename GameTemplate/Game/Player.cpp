#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "PAUSE.h"
#include "Game.h"

//EffectEmitter���g�p���邽�߂ɁA�t�@�C�����C���N���[�h����B
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float CHARACON_RADIUS = 30.0f;            //�L�����R���̔��a
	const float CHARACON_HEIGHT = 100.0f;            //�L�����R���̍���
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;   //�ړ����x�̍Œ�l
	const float WALK_MOVESPEED = 1200.0f;            //�����X�e�[�g�̈ړ����x
	const float RUN_MOVESPEED = 400.0f;            //����X�e�[�g�̈ړ����x
	const float STEALTHYSTEP_MOVESPEED = 100.0f;     //�E�ё��X�e�[�g�̈ړ����x
	const float GRAVITY = 1000.0f;                  //�d��
	const float ATTACK_TIME = 1.5f;                 //�A���U�����ł��鎞��
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
	m_animationClipArray[enAnimClip_snake].Load("Assets/animData/player2/snakes.tka");
	m_animationClipArray[enAnimClip_snake].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_StealthySteps].Load("Assets/animData/player2/stealthysteps.tka");
	m_animationClipArray[enAnimClip_StealthySteps].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_FirstAttack].Load("Assets/animData/player2/slashattack.tka");
	m_animationClipArray[enAnimClip_FirstAttack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_PokeAttack].Load("Assets/animData/player2/pokeattack.tka");
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
	//�A�j���[�V�������������B
	InitAnimation();

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/player/player2.tkm", false, m_animationClipArray, enAnimClip_Num);
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�uSword�v�{�[����ID(�ԍ�)���擾����B
	m_sword_jointBoneId = m_modelRender.FindBoneID(L"Sword_joint");


	//�T�E���h��ǂݍ��ށB
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/11yoroi.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/audio/kiru.wav");
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/audio/tuki.wav");
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/audio/playerdamage.wav");
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/audio/playerdown.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/audio/kaiten2.wav");

	//�L�����R��
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);


	m_HPberRender.Init("Assets/sprite/ft.dds", 1600, 900);
	//�\��������W��ݒ肷��B
	m_HPberRender.SetPivot({ 0.0f, 0.5f });
	m_HPberRender.SetPosition({ 0.0f,0.0f ,0.0f });

	m_HPRender.Init("Assets/sprite/HP.dds", 500, 27);
	//�\��������W��ݒ肷��B
	m_HPRender.SetPivot({ 0.0f, 0.5f });
	m_HPRender.SetPosition({ 0.0f,0.0f ,0.0f });
	m_HPRender.Update();
	m_stmnberRender.Init("Assets/sprite/ft.dds", 1600, 900);
	//�\��������W��ݒ肷��B
	m_stmnberRender.SetPivot({ 0.0f, 0.5f });
	m_stmnberRender.SetPosition({ 0.0f,0.0f ,0.0f });

	m_staminaRender.Init("Assets/sprite/stmn.dds", 500, 27);
	//�\��������W��ݒ肷��B
	m_staminaRender.SetPivot({ 0.0f, 0.5f });
	m_staminaRender.SetPosition({ 0.0f,0.0f ,0.0f });
	m_staminaRender.Update();

	m_senseberRender.Init("Assets/sprite/ft.dds", 1600, 900);
	//�\��������W��ݒ肷��B
	m_senseberRender.SetPivot({ 0.0f, 0.5f });
	m_senseberRender.SetPosition({ -800.0f,-45.0f ,0.0f });
	m_senseberRender.Update();

	m_senseRender.Init("Assets/sprite/sp.dds", 500, 27);
	//�\��������W��ݒ肷��B
	m_senseRender.SetPivot({ 0.0f, 0.5f });
	m_senseRender.SetPosition({ 0.0f,0.0f ,0.0f });
	m_senseRender.Update();

	EffectEngine::GetInstance()->ResistEffect(2, u"Assets/effect/efk/blood2.efk");

	m_game = FindGO<Game>("game");

	return true;
}

void Player::Update()
{


	if (g_pad[0]->IsTrigger(enButtonStart)		//Start�{�^���������ꂽ�B
		&& m_menu == false)					//����m_menu==false�̎��B
	{
		m_menu = true;
		m_pause = NewGO<PAUSE>(0, "PASUE");
	}
	else if (g_pad[0]->IsTrigger(enButtonStart)		//Start�{�^���������ꂽ�B
		&& m_menu == true)					//����m_menu==true�̎��B 
	{
		m_menu = false;
	}


	if (m_menu == true)
	{
		return;
	}



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

	//���W�A��]�A�傫���̍X�V
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(m_scale);
	m_modelRender.SetScale({ 2.0f,2.0f,2.0 });
	
	//���f���̍X�V
	m_modelRender.Update();


	//HP�o�[
/*	if (m_hp <= 0.0f)
	{
		m_hp = 0.0f;
	}
*/
	if (m_hp >= 100.0f)
	{
		m_hp = 100.0f;
	}
	if (m_hp <= 0) {
		m_hp = 0;
	}
	life = m_hp / 100.0f;
	m_HPRender.SetScale({ life, 1.0f, 0.0f });
//	m_HPRender.SetPivot({ 0.0f, 0.5f });
	m_HPRender.SetPosition({ -748.0f,420.0f ,0.0f });
	m_HPRender.Update();



	if (m_sutamina <= 0.0f)
	{
		m_sutamina = 0.0f;
	}
	if (m_sutamina == 0.0f)
	{
		COOLtime = true;
	}
	if (m_sutamina >= 200.0f)
	{
		m_sutamina = 200.0f;
	}

	if (COOLtime == true)
	{
		cooltime += g_gameTime->GetFrameDeltaTime();
		if (m_sutamina >= 50.0f)
		{
			COOLtime = false;
		}
	}
	else {
		cooltime = 0.0f;
	}

	m_hurusutamina = m_sutamina / 200.0f;
	m_staminaRender.SetScale({ m_hurusutamina,1.0f,0.0f });
	m_staminaRender.SetPosition({ -748.0f,372.0f ,0.0f });
	//m_staminaRender.SetPivot({ 0.0f, 0.5f });
	m_staminaRender.Update();

	if (g_pad[0]->IsPress(enButtonY) && m_sp >= 1) {
		--m_sp;
	}
	else if(m_sp <= 150){
		m_sp += 0.2f;
	}

	m_hurusp = m_sp / 150.0f;
	m_senseRender.SetScale({ m_hurusp,1.0f,0.0f });
	m_senseRender.SetPosition({ -748.0f,325.0f ,0.0f });
	//m_staminaRender.SetPivot({ 0.0f, 0.5f });
	m_senseRender.Update();


	m_HPberRender.SetPosition({ -800.0f,45.0f ,0.0f });
	m_HPberRender.Update();
	m_stmnberRender.SetPosition({ -800.0f,0.0f ,0.0f });
	m_stmnberRender.Update();
}

void Player::Move()
{
	//�ҋ@�X�e�[�g�A�����X�e�[�g�A����X�e�[�g�ȊO��������
	if (m_playerState != enPlayerState_Run &&
		m_playerState != enPlayerState_Walk &&
		m_playerState != enPlayerState_StealthySteps &&
		m_playerState != enPlayerState_snake&&
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


	if (index == 2) {
			m_moveSpeed.z += 200.0f;
	//		m_playerState = enPlayerState_Walk;
	//		return;
	}
	if (index == 3)
	{
		//m_moveSpeed.z = 0.0f;
		//index = 4;
		m_playerState = enPlayerState_Win;
	//	return;
	}

	if (index == 0 || index == 1) {
		m_startwalk += g_gameTime->GetFrameDeltaTime();
		if (m_startwalk >= 1.0f && m_startwalk <= 2.5f)
		{
			m_moveSpeed.z += 200.0f;
			//	m_playerState = enPlayerState_Walk;
			//	return;
		}
		else if (m_startwalk > 2.5f) {
			if (COOLtime == true)
			{
				m_moveSpeed += cameraForward * lStick_y * 100.0f;	//�������ւ̈ړ����x�����Z�B
				m_moveSpeed += cameraRight * lStick_x * 100.0f;		//�E�����ւ̈ړ����x�����Z�B
				m_sutamina++;
			}
			if (COOLtime == false)
			{
				//����X�e�[�g��������
				if (m_playerState == enPlayerState_Run)
				{
					m_moveSpeed += cameraForward * lStick_y * RUN_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * RUN_MOVESPEED;
					m_sutamina -= 1.5f;
				}

				else if (m_playerState == enPlayerState_StealthySteps || m_playerState == enPlayerState_snake) {
					m_moveSpeed += cameraForward * lStick_y * STEALTHYSTEP_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * STEALTHYSTEP_MOVESPEED;
					m_sutamina -= 0.5f;
				}
				//����ȊO��������
				else {
					m_moveSpeed += cameraForward * lStick_y * WALK_MOVESPEED;
					m_moveSpeed += cameraRight * lStick_x * WALK_MOVESPEED;
					m_sutamina++;
				}
			}
		}
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
	//��_���[�W�X�e�[�g��������
	if (m_playerState == enPlayerState_ReceiveDamage)
	{
		//�������Ȃ�
		return;
	}

	//�G�l�~�[�i�^���N�j�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 80.0f;

			if (m_hp <= 0)
			{
				m_playerState = enPlayerState_Down;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}

	//�G�l�~�[�i�X�s�[�h�j�̍U���p�̃R���W�������擾����B
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("Speedenemy_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision2 : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision2->IsHit(m_charaCon))
		{
			m_hp -= 18.0f;

			if (m_hp <= 0)
			{
				m_playerState = enPlayerState_Down;
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}

	//�G�l�~�[�i3�j�̍U���p�̃R���W�������擾����B
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("enemy3_attack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			m_hp -= 40.0f;
			if (m_hp <= 0)
			{
				m_playerState = enPlayerState_Down;
				//SoundSource* downse = NewGO<SoundSource>(0);
				//downse->Init(14);
				//downse->Play(false);
				//downse->SetVolume(0.6f);
			}
			else {
				//��_���[�W�X�e�[�g�ɑJ�ڂ���B
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}

}

void Player::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ��Ȃ�
	if (m_playerState != enPlayerState_FirstAttack &&
		m_playerState != enPlayerState_PokeAttack)
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
		Vector3(20.0f, 20.0f, 120.0f)                              //�傫���B
	);

	Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
	collisionObject->SetWorldMatrix(matrix);

	if (m_playerState == enPlayerState_PokeAttack){


		collisionObject->SetName("player_porkattack");
	}
	else {
		collisionObject->SetName("player_attack");
	}
}


void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//�����X�e�[�g�̎�
	case Player::enPlayerState_Walk:
		m_modelRender.PlayAnimation(enAnimClip_Walk, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//���Ⴊ�݃X�e�[�g�̎�
	case Player::enPlayerState_snake:
		m_modelRender.PlayAnimation(enAnimClip_snake, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//�E�ё��X�e�[�g�̎�
	case Player::enPlayerState_StealthySteps:
		m_modelRender.PlayAnimation(enAnimClip_StealthySteps, 0.2f);
		m_modelRender.SetAnimationSpeed(1.2f);
		break;
		//�U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
		m_modelRender.PlayAnimation(enAnimClip_FirstAttack, 0.2f);
		m_modelRender.SetAnimationSpeed(1.3f);
		break;
		//�˂��U���X�e�[�g�̎�
	case Player::enPlayerState_PokeAttack:
		m_modelRender.PlayAnimation(enAnimClip_PokeAttack, 0.4f);
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
		//�_�E���X�e�[�g�̎�
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
		//���Ⴊ�݃X�e�[�g�̎�
	case Player::enPlayerState_snake:
		ProcesssnakeStateTransition();
		break;
		//�E�ё��X�e�[�g�̎�
	case Player::enPlayerState_StealthySteps:
		ProcessStealthyStepsStateTransition();
		break;
		//1���̍U���X�e�[�g�̎�
	case Player::enPlayerState_FirstAttack:
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
		//�����X�e�[�g�̎�
	case Player::enPlayerState_Win:
		ProcessWinStateTransition();
		break;
	default:
		break;
	}
}

void Player::ProcessCommonStateTransition()
{

	//RB1�{�^���������ꂽ�灕�U���X�e�[�g�P��������
	if (g_pad[0]->IsPress(enButtonRB1))
	{

		//�R���W�����I�u�W�F�N�g���쐬����B
		auto collisionObject = NewGO<CollisionObject>(0);

		Vector3 collisionPosition =m_position;
		collisionPosition += m_forward * 100.0f;
		collisionPosition.y += 100.0f;
		//�{�b�N�X��̃R���W�������쐬����B
		collisionObject->CreateBox(collisionPosition,				   //���W�B
			m_rotation,                                      //��]�B
			Vector3(60.0f, 30.0f, 180.0f)                              //�傫���B
		);
		collisionObject->SetName("player");


		if (critical == 1)
		{
			//�˂��U���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_PokeAttack;
		}
		else {
			//�U���X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_FirstAttack;
			m_isUnderAttack = false;
		}

		return;
	}

	if (g_pad[0]->IsPress(enButtonRB2))
	{
		//�R���W�����I�u�W�F�N�g���쐬����B
		auto collisionObject = NewGO<CollisionObject>(0);

		Vector3 collisionPosition = m_position;
		collisionPosition += m_forward * 100.0f;
		collisionPosition.y += 100.0f;
		//�{�b�N�X��̃R���W�������쐬����B
		collisionObject->CreateBox(collisionPosition,				   //���W�B
			m_rotation,                                      //��]�B
			Vector3(60.0f, 30.0f, 180.0f)                              //�傫���B
		);
		collisionObject->SetName("player");

		/*camera = FindGO<GameCamera>("gameCamera");
		camera->m_camera = 1;*/

		//prok = true;

		//�˂��U���X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_PokeAttack;

		return;
	}


	//x��z�̈ړ����x����������
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE || fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		if (COOLtime == true)
		{
			//�����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Walk;
			st = 0;
			return;
		}
		//A�{�^���������ꂽ��
		if (COOLtime == false && g_pad[0]->IsPress(enButtonA))
		{
			//����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Run;
			st = 1;
			return;
		}
		//X�{�^���������ꂽ��
		if (g_pad[0]->IsPress(enButtonX))
		{
			//�E�ё��X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_StealthySteps;
	//		m_sutamina = m_sutamina - 0.5f;
			st = 3;
			return;
		}
		//������Ȃ�������
		else
		{
			//�����X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_Walk;
			st = 0;
		}
	}

	//x��z�̈ړ����x���Ȃ�������
	else
	{
		//X�{�^���������ꂽ��
		if (g_pad[0]->IsPress(enButtonX))
		{
			//�E�ё��X�e�[�g�ֈڍs����
			m_playerState = enPlayerState_snake;
		//	m_sutamina = m_sutamina - 0.5f;
			return;
		}
		else{
			//�ҋ@�X�e�[�g�Ɉڍs����
			m_playerState = enPlayerState_Idle;
		}
		st = 0;

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

void Player::ProcesssnakeStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessStealthyStepsStateTransition()
{
	//���̃X�e�[�g�ɑJ�ڂ���
	ProcessCommonStateTransition();
}

void Player::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
	/*	if (prok == true) {
			prok = false;
		}*/
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessReceiveDamageStateTransition()
{
	if (m_Hitse == 0) {
		SoundSource* damagese = NewGO<SoundSource>(0);
		damagese->Init(13);
		damagese->Play(false);
		damagese->SetVolume(0.6f);
		m_Hitse = 1;
	}
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_Hitse = 0;
		//���̃X�e�[�g�ɑJ�ڂ���
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	m_downtimer += g_gameTime->GetFrameDeltaTime();
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_down = true;
	}
	if (m_Hitse == 0) {
		SoundSource* damagese = NewGO<SoundSource>(0);
		damagese->Init(13);
		damagese->Play(false);
		damagese->SetVolume(0.8f);
		m_Hitse = 1;
	}
	if (m_deathse == 0 && m_downtimer >=1.2f) {
		SoundSource* downse = NewGO<SoundSource>(0);
		downse->Init(14);
		downse->Play(false);
		downse->SetVolume(0.8f);
		m_deathse = 1;
	}
	if (m_deathse == 1 && m_downtimer >= 2.5f) {
		SoundSource* downse = NewGO<SoundSource>(0);
		downse->Init(14);
		downse->Play(false);
		downse->SetVolume(0.8f);
		m_deathse = 2;
	}
}


void Player::ProcessWinStateTransition()
{
	if(m_modelRender.IsPlayingAnimation() == false){
		//ProcessCommonStateTransition();
		index = 4;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		SoundSource* slashse = NewGO<SoundSource>(0);
		slashse->Init(11);
		slashse->Play(false);
		slashse->SetVolume(0.6f);
		//�U���t���O��true�ɂ���
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"attack_end") == 0)
	{
		//�U���t���O��false�ɂ���
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"porkattack_start") == 0)
	{
		SoundSource* slashse = NewGO<SoundSource>(0);
		slashse->Init(12);
		slashse->Play(false);
		slashse->SetVolume(0.6f);
		//�U���t���O��true�ɂ���
		m_isUnderAttack = true;
	}
	else if (wcscmp(eventName, L"porkattack_end") == 0)
	{
		if (prok == true) {
			Matrix matrix = m_modelRender.GetBone(m_sword_jointBoneId)->GetWorldMatrix();
			Vector3 m_effectPosition = m_position;
			m_effectPosition.y += 100.0f;
			//�G�t�F�N�g�̃I�u�W�F�N�g���쐬����B
			m_effectEmitter = NewGO <EffectEmitter>(0);
			m_effectEmitter->Init(2);
			m_effectEmitter->SetPosition(m_effectPosition);
			//�G�t�F�N�g�̑傫����ݒ肷��B
			m_effectEmitter->SetScale(m_scale * 50.0f);
			m_effectEmitter->Play();
			m_effectEmitter->SetWorldMatrix(matrix);
		}
		//�U���t���O��false�ɂ���
		m_isUnderAttack = false;
	}

	if (wcscmp(eventName, L"Run_step") == 0) {
		//�����B
		 //���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.8f);
	}

	if (wcscmp(eventName, L"Walk_step") == 0) {
		//�����B
		 //���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.5f);
	}

	if (wcscmp(eventName, L"snake_step") == 0) {
		//�����B
		 //���ʉ����Đ�����B
		SoundSource* stepse = NewGO<SoundSource>(0);
		stepse->Init(9);
		stepse->Play(false);
		stepse->SetVolume(0.3f);
	}

}

void Player::Render(RenderContext& rc)
{
	if (m_startwalk>=2.5f) {
		if (m_alpha < 1.0f && m_down == false && index <= 2) {
			m_alpha += 0.01f;
		}
		else if (m_alpha > 0.0f && m_down == true ||
			m_alpha > 0.0f  && index >= 3) {
			m_alpha -= 0.003f;
		}
		if (m_menu == false) {
			//�摜��`�ʂ���B
			//HP
			m_HPRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_HPberRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_HPRender.Draw(rc);
			m_HPberRender.Draw(rc);
			//�X�^�~�i
			m_staminaRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_stmnberRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_staminaRender.Draw(rc);
			m_stmnberRender.Draw(rc);
			//SP
			m_senseRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_senseberRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
			m_senseRender.Draw(rc);
			m_senseberRender.Draw(rc);
		}
	}
	//�h���[�B
	m_modelRender.Draw(rc);
}