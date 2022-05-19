#include "stdafx.h"
#include "SpeedEnemy.h"

#include "Player.h"

#include "CollisionObject.h"

namespace
{
	const float CHARACON_RADIUS = 20.0f;            //�L�����R���̔��a
	const float CHARACON_HEIGHT = 45.0f;             //�L�����R���̍���
	const float MODEL_POSITION_Y = 10.0f;            //���f����Y���W
	const float MOVESPEED_MINIMUMVALUE = 0.001f;    //�ړ����x�̍Œ�l
	const float MOVESPEED = 200.0f;                 //�ړ����x
	const float GRAVITY = 1000.0f;                  //�d��
	const float IDLETIMER_MAX = 0.3f;               //�ҋ@�^�C�}�[�̍ő�l
	const float IDLETIMER_DEFAULT = 0.0f;           //�ҋ@�^�C�}�[�̏����l
	const float CHASETIMER_MAX = 0.8f;              //�ǐՃ^�C�}�[�̍ő�l
	const float CHASETIMER_DEFAULT = 0.0f;          //�ǐՃ^�C�}�[�̏����l
	const float SEARCH_RANGE = 300.0f;              //�v���C���[�𔭌��ł��鋗��
	const float SEARCH_ANGLE = 130.0f;              //�v���C���[�𔭌��ł���p�x
	const float ATTACK_RANGE = 100;					//�U���ł��鋗��
	const int   ATTACK_PROBABILITY = 0;             //�U�����s���m��
	const float RECEIVE_DAMAGE = 50;                 //�v���C���[����󂯂�_���[�W
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
	m_animationClipArray[enAnimClip_Attack].Load("Assets/animData/speedenemy/attack.tka");
	m_animationClipArray[enAnimClip_Attack].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Shout].Load("Assets/animData/speedenemy/shout.tka");
	m_animationClipArray[enAnimClip_Shout].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_ReceiveDamage].Load("Assets/animData/speedenemy/damage.tka");
	m_animationClipArray[enAnimClip_ReceiveDamage].SetLoopFlag(false);
	m_animationClipArray[enAnimClip_Down].Load("Assets/animData/speedenemy/down.tka");
	m_animationClipArray[enAnimClip_Down].SetLoopFlag(false);
	m_animationClipArray[enAnimationClip_alert].Load("Assets/animData/speedenemy/alert.tka");
	m_animationClipArray[enAnimationClip_alert].SetLoopFlag(false);
}

bool SpeedEnemy::Start()
{
	//�A�j���[�V�����̏�����
	InitAnimation();

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm",
		false,
		m_animationClipArray,
		enAnimClip_Num);
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_position.y = 15.0f;
	//�L�����R���̏�����
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);

	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�p���`�̃{�[��
	m_punchBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");

	m_player = FindGO<Player>("player");

	m_enemypath.Init("Assets/path/speed/enemypath1.tkl");
	m_enemypath2.Init("Assets/path/speed/enemypath2.tkl");
	m_enemypath3.Init("Assets/path/speed/enemypath3.tkl");
	m_enemypath4.Init("Assets/path/speed/enemypath4.tkl");
	m_enemypath5.Init("Assets/path/speed/enemypath5.tkl");
	m_enemypath6.Init("Assets/path/speed/enemypath6.tkl");
	m_enemypath7.Init("Assets/path/speed/enemypath7.tkl");
	m_enemypath8.Init("Assets/path/speed/enemypath8.tkl");
	m_enemypath9.Init("Assets/path/speed/enemypath9.tkl");
	m_enemypath10.Init("Assets/path/speed/enemypath10.tkl");
	m_enemypath11.Init("Assets/path/speed/enemypath11.tkl");
	m_enemypath12.Init("Assets/path/speed/enemypath12.tkl");
	m_enemypath13.Init("Assets/path/speed/enemypath13.tkl");
	
	m_point = m_enemypath.GetFirstPoint();
	m_point2 = m_enemypath2.GetFirstPoint();
	m_point3 = m_enemypath3.GetFirstPoint();
	m_point4 = m_enemypath4.GetFirstPoint();
	m_point5 = m_enemypath5.GetFirstPoint();
	m_point6 = m_enemypath6.GetFirstPoint();
	m_point7 = m_enemypath7.GetFirstPoint();
	m_point8 = m_enemypath8.GetFirstPoint();
	m_point9 = m_enemypath9.GetFirstPoint();
	m_point10 = m_enemypath10.GetFirstPoint();
	m_point11 = m_enemypath11.GetFirstPoint();
	m_point12 = m_enemypath12.GetFirstPoint();
	m_point13 = m_enemypath13.GetFirstPoint();

	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);

	alertSprite.Init("Assets/sprite/alert.dds", 64, 64);
	//�\��������W��ݒ肷��B
	alertSprite.SetPosition({ 0.0f,0.0f ,0.0f });

	//�������������B
	srand((unsigned)time(NULL));
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
	return true;
}

void SpeedEnemy::Update()
{
	Weak = m_player->GetPosition() - m_position;
	if (Weak.Length() >= 3000.0f)
	{
		return;
	}

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

	if (state == 0 && m_isSearchPlayer == false) {

		m_timer += g_gameTime->GetFrameDeltaTime();
		if (m_timer < 0.5f) {
			m_speedEnemyState = enSpeedEnemyState_Idle;
		}
		if (m_timer >= 0.5f) {
			m_speedEnemyState = enSpeedEnemyState_Chase;
			state = 1;
		}
	}

	Vector3 diff = m_player->GetPosition() - m_position;

	//�x�N�g���̒�����700.0f��菬����������B
	if (diff.LengthSq() <= 1500.0f * 1500.0f && m_hp > 0)
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
		alertSprite.SetPosition({ 2000.0f,2000.0f,0.0f });
	}
	alertSprite.Update();

	//���f���̍X�V
	m_modelRender.Update();
}

void SpeedEnemy::Chase()
{
	//�v���C���[�������Ă��Ȃ�������B
	if (state == 1)
	{
		if (Weak.Length() <= 800.0f && m_player->st == 1 || Weak.Length() <= 800.0f && m_player->st == 2)
		{
			m_speedEnemyState = enSpeedEnemyState_alert;

			if (alertLevel == 0) {
				alertLevel = 1;
			}
			//	if (alertLevel == 3)
			//	{
			//		return;
			//	}
		}
		else if (alertLevel == 0 || alertLevel == 3) {
			m_speedEnemyState = enSpeedEnemyState_Chase;
			Aroute();
		}
	}

	//�ǐՃX�e�[�g�łȂ��Ȃ�A�ǐՏ����͂��Ȃ��B
	if (m_speedEnemyState != enSpeedEnemyState_Chase)
	{
		//�������Ȃ�
		return;
	}
	
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	if (m_charaCon.IsOnGround()) {
		//�n�ʂɂ����B
		m_moveSpeed.y = 0.0f;
	}
	m_modelRender.SetPosition(m_position);

	//Vector3 modelPosition = m_position;
	////������Ƃ������f���̍��W��������B
	//modelPosition.y += MODEL_POSITION_Y;
	//m_modelRender.SetPosition(modelPosition);
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

void SpeedEnemy::Collision()
{
	//��_���[�W�X�e�[�g�ƃ_�E���X�e�[�g��������
	if (m_speedEnemyState == enSpeedEnemyState_ReceiveDamage ||
		m_speedEnemyState == enSpeedEnemyState_Down)
	{
		//�������Ȃ�
		return;
	}

	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
//	Matrix matrix = m_modelRender.GetBone(m_weakness)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, Quaternion::Identity, Vector3(50.0f, 50.0f, 50.0f));
//	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy");


	//�v���C���[�̍U���p�̃R���W�������擾����B
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("player_porkattack");
	//�R���W�����̔z���for���ŉ񂷁B
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(collisionObject))
		{
			//state = 2;
			//m_camera->m_camera = 0;
			//�_�E���X�e�[�g�ɑJ�ڂ���B
			m_speedEnemyState = enSpeedEnemyState_Down;
			return;
		}
	}

	//�v���C���[�̍U���̓����蔻��
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("player_attack");
	for (auto collision2 : collisions2)
	{
		//�v���C���[�̍U���ƃL�����R�����Փ˂�����
		if (collision2->IsHit(m_charaCon))
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
	collisionObject->SetName("Speedenemy_attack");
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
		m_modelRender.SetAnimationSpeed(1.6f);
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
		//�x���X�e�[�g�̎��B
	case SpeedEnemy::enSpeedEnemyState_alert:
		//�x���A�j���[�V�������Đ��B
		m_modelRender.PlayAnimation(enAnimationClip_alert, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
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
		//�x���X�e�[�g�̎��B
	case SpeedEnemy::enSpeedEnemyState_alert:
		//�x���X�e�[�g�̃X�e�[�g�J�ڏ����B
		alertTransition();
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

		//�U���ł���Ȃ�
		if (IsCanAttack() == true)
		{
			int ram = rand() % 100;
			if (ram >= ATTACK_PROBABILITY)
			{
				//�U���X�e�[�g�Ɉڍs����
				m_speedEnemyState = enSpeedEnemyState_Attack;
				//m_isUnderAttack = false;
				return;
			}
			else
			{
				//�ҋ@�X�e�[�g�Ɉڍs����
				m_speedEnemyState = enSpeedEnemyState_Attack;
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
	else if (mikke == true) {
		Vector3 diff2 = targetpos - m_position;
		if (diff2.LengthSq() <= 500.0f * 500.0f * g_gameTime->GetFrameDeltaTime()) {
			if (m_isSearchPlayer == false) {
				mikke = false;
			}
		}

		//�G�l�~�[����v���C���[�Ɍ������x�N�g�����v�Z����B
		Vector3 diff1 = targetpos - m_position;
	//	huntertimer += g_gameTime->GetFrameDeltaTime();
		//�x�N�g���𐳋K������B
		diff1.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff1 * 200.0f;

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

	//�ǐՃ^�C�}�[��0.8f���傫��������
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
		//�U�����ꂽ�狗���֌W�����ɁA��芸�����ǐՂ�����B
		m_speedEnemyState = enSpeedEnemyState_Chase;
		Vector3 diff = m_player->GetPosition() - m_position;
		diff.Normalize();
		//�ړ����x��ݒ肷��B
		m_moveSpeed = diff * 250.0f;
	}
}

void SpeedEnemy::ProcessDownStateTransition()
{
	m_position.y = 15.0f;
	m_modelRender.SetPosition(m_position);
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		m_hp = 0;
		state = 3;
		m_charaCon.RemoveRigidBoby();
	}
	////�A�j���[�V�����̍Đ����I����Ă�����
	//if (m_modelRender.IsPlayingAnimation() == false)
	//{
	//	//���g���폜����
	//	DeleteGO(this);
	//	return;
	//}
}

void SpeedEnemy::alertTransition()
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

//void SpeedEnemy::lookTransition()
//{
//	//�x��A�j���[�V�����̍Đ����I�������B
//	if (m_modelRender.IsPlayingAnimation() == false)
//	{
//		//	alertLevel = 0;
//			//���̃X�e�[�g�ɑJ�ڂ���B
//		ProcessCommonStateTransition();
//	}
//}

void SpeedEnemy::SearchPlayer()
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


///�o�H
void SpeedEnemy::Aroute()
{

	if (m_speedNumber == 0)
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
		else {
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
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}

	if (m_speedNumber == 1)
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
		else {
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
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 2)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point3->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point3->s_number == m_enemypath3.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point3 = m_enemypath3.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point3 = m_enemypath3.GetNextPoint(m_point3->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point3->s_position.x, m_point3->s_position.y + 70.0f, m_point3->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point3 = m_enemypath3.GetNextPoint(m_point3->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 3)
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
		else {
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
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
		if (m_speedNumber == 4)
		{
			//�ڕW�n�_�܂ł̃x�N�g��
			Vector3 diff = m_point5->s_position - m_position;
			//�ڕW�n�_�ɋ߂�������
			if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
			{
				//�Ō�̖ڕW�n�_��������
				if (m_point5->s_number == m_enemypath5.GetPointListSize() - 1)
				{
					//�ŏ��̖ڕW�n�_��
					m_point5 = m_enemypath5.GetFirstPoint();
				}
				//�Ō�̖ڕW�n�_�ł͂Ȃ�������
				else
				{
					//���̖ڕW�n�_��
					m_point5 = m_enemypath5.GetNextPoint(m_point5->s_number);
				}
			}
			else {
				btTransform start, end;
				start.setIdentity();
				end.setIdentity();
				//�n�_�̓G�l�~�[�̍��W�B
				start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
				//�I�_�͎��̃p�X�̍��W�B
				end.setOrigin(btVector3(m_point5->s_position.x, m_point5->s_position.y + 70.0f, m_point5->s_position.z));

				SweepResultWall callback;
				//�R���C�_�[���n�_����I�_�܂œ������āB
				//�Փ˂��邩�ǂ����𒲂ׂ�B
				PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
				//�ǂƏՓ˂����I
				if (callback.isHit == true)
				{
					m_point5 = m_enemypath5.GetNextPoint(m_point5->s_number);
					return;
				}
				else {
					//���K��
					diff.Normalize();
					//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
					m_moveSpeed = diff * 200.0f;
					//Y���W�̈ړ����x��0�ɂ���
					m_moveSpeed.y = 0.0f;
				}
			}
		}
	if (m_speedNumber == 5)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point6->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point6->s_number == m_enemypath6.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point6 = m_enemypath6.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point6 = m_enemypath6.GetNextPoint(m_point6->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point6->s_position.x, m_point6->s_position.y + 70.0f, m_point6->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point6 = m_enemypath6.GetNextPoint(m_point6->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 6)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point7->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point7->s_number == m_enemypath7.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point7 = m_enemypath7.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point7 = m_enemypath7.GetNextPoint(m_point7->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point7->s_position.x, m_point7->s_position.y + 70.0f, m_point7->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point7 = m_enemypath7.GetNextPoint(m_point7->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 7)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point8->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point8->s_number == m_enemypath8.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point8 = m_enemypath8.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point8 = m_enemypath8.GetNextPoint(m_point8->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point8->s_position.x, m_point8->s_position.y + 70.0f, m_point8->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point8 = m_enemypath8.GetNextPoint(m_point8->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 8)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point9->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point9->s_number == m_enemypath9.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point9 = m_enemypath9.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point9 = m_enemypath9.GetNextPoint(m_point9->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point9->s_position.x, m_point9->s_position.y + 70.0f, m_point9->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point9 = m_enemypath9.GetNextPoint(m_point9->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 9)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point10->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point10->s_number == m_enemypath10.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point10 = m_enemypath10.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point10 = m_enemypath10.GetNextPoint(m_point10->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point10->s_position.x, m_point10->s_position.y + 70.0f, m_point10->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point10 = m_enemypath10.GetNextPoint(m_point10->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 10)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point11->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point11->s_number == m_enemypath11.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point11 = m_enemypath11.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point11 = m_enemypath11.GetNextPoint(m_point11->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point11->s_position.x, m_point11->s_position.y + 70.0f, m_point11->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point11 = m_enemypath11.GetNextPoint(m_point11->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 11)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point12->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point12->s_number == m_enemypath12.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point12 = m_enemypath12.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point12 = m_enemypath12.GetNextPoint(m_point12->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point12->s_position.x, m_point12->s_position.y + 70.0f, m_point12->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point12 = m_enemypath12.GetNextPoint(m_point12->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}
	if (m_speedNumber == 12)
	{
		//�ڕW�n�_�܂ł̃x�N�g��
		Vector3 diff = m_point13->s_position - m_position;
		//�ڕW�n�_�ɋ߂�������
		if (diff.LengthSq() <= 100.0f * 100.0f * g_gameTime->GetFrameDeltaTime())
		{
			//�Ō�̖ڕW�n�_��������
			if (m_point13->s_number == m_enemypath13.GetPointListSize() - 1)
			{
				//�ŏ��̖ڕW�n�_��
				m_point13 = m_enemypath13.GetFirstPoint();
			}
			//�Ō�̖ڕW�n�_�ł͂Ȃ�������
			else
			{
				//���̖ڕW�n�_��
				m_point13 = m_enemypath13.GetNextPoint(m_point13->s_number);
			}
		}
		else {
			btTransform start, end;
			start.setIdentity();
			end.setIdentity();
			//�n�_�̓G�l�~�[�̍��W�B
			start.setOrigin(btVector3(m_position.x, m_position.y + 70.0f, m_position.z));
			//�I�_�͎��̃p�X�̍��W�B
			end.setOrigin(btVector3(m_point13->s_position.x, m_point13->s_position.y + 70.0f, m_point13->s_position.z));

			SweepResultWall callback;
			//�R���C�_�[���n�_����I�_�܂œ������āB
			//�Փ˂��邩�ǂ����𒲂ׂ�B
			PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
			//�ǂƏՓ˂����I
			if (callback.isHit == true)
			{
				m_point13 = m_enemypath13.GetNextPoint(m_point13->s_number);
				return;
			}
			else {
				//���K��
				diff.Normalize();
				//�ڕW�n�_�Ɍ������x�N�g���~�ړ����x
				m_moveSpeed = diff * 200.0f;
				//Y���W�̈ړ����x��0�ɂ���
				m_moveSpeed.y = 0.0f;
			}
		}
	}

}

void SpeedEnemy::Render(RenderContext& rc)
{
	if (Weak.Length() <= 3000.0f)
	{
		//���f���̕`��
		m_modelRender.Draw(rc);
	}

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;
	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y + 150.0f, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + 150.0f, playerPosition.z));

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
	else {

		if (Weak.Length() <= 1500.0f)
		{
			alertSprite.Draw(rc);
		}
	}
}