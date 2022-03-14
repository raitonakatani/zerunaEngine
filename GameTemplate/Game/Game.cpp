#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Player.h"
#include "Background.h"
#include "TankEnemy.h"
#include "SpeedEnemy.h"
#include "Enemy3.h"
#include "Fade.h"


Game::~Game()
{
//	DeleteGO(m_player);
//	DeleteGO(m_background);
}
bool Game::Start()
{
//	g_camera3D->SetPosition({ 0.0f, 100.0f, -600.0f });

	m_background = NewGO<Background>(0);
//	m_bgModelRendedr.Init("Assets/modelData/karisute/stage.yuka.tkm");
//	m_bgObject.CreateFromModel(m_bgModelRendedr.GetModel(), m_bgModelRendedr.GetWorldMatrix(0));

	m_player = NewGO<Player>(0, "player");

	m_gameCamera = NewGO<GameCamera>(0, "gameCamera");


	//���x�����\�z����B
	m_levelRender.Init("Assets/level/stage.tkl", [&](LevelObjectData& objData) {
		if (objData.EqualObjectName(L"player") == true) {
			//�v���C���[�̃C���X�^���X�𐶐�����B
			m_player = NewGO<Player>(0, "player");
			m_player->SetPosition({ objData.position });
			m_player->SetRotation(objData.rotation);
			m_player->SetScale(objData.scale);
			//true�ɂ���ƁA���x���̕��Ń��f�����ǂݍ��܂�Ȃ��B
			return true;
		}
		return true;
	}

//	m_tank = NewGO<TankEnemy>(0);
<<<<<<< HEAD
//	m_tank->SetPosition({ 0.0f,0.0f,200.0f });
//	m_enemy3 = NewGO<Enemy3>(0);
//	m_enemy3->SetPosition({ 0.0f,0.0f,200.0f });
=======
//	m_tank->SetPosition({ 100.0f,0.0f,-100.0f });
	m_speed = NewGO<SpeedEnemy>(0);
	m_speed->SetPosition({ 100.0f,0.0f,-100.0f });

	m_enemy3 = NewGO<Enemy3>(0);
	m_enemy3->SetPosition({ -2500.0f,0.0f,100.0f });
	m_background = NewGO<Background>(0);
>>>>>>> 92e49d29f36d402a069dceb101d6047e1980e148

	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();


	//�摜��ǂݍ��ށB
	m_spriteRender.Init("Assets/sprite/TENEBRIS.dds", 800, 450, AlphaBlendMode_None);
//	m_pressButton.Init("Assets/sprite/button.dds", 400, 225, AlphaBlendMode_Trans);
//	m_targetRender.Init("Assets/sprite/TENEBRIS.dds", 50, 50);


//	g_camera3D->SetViewAngle(Math::DegToRad(60.0f));
//	g_camera3D->SetPosition(0.0f, 1000.0f, 1000.0f);

//	g_camera3D->SetTarget(0.0f, 400.0f, 0.0f);
//	g_camera3D->SetFar(20000.0f);

//	m_charaRender.Init("Assets/modelData/unityChan.tkm");
//	m_charaRender.SetScale(1.2f, 1.2f, 1.2f);
//	m_targetPointRender.Init("Assets/modelData/light.tkm");
//	m_targetPointRender.SetScale(2.0f, 2.0f, 2.0f);
//	m_targetPointRender.SetShadowCasterFlag(false);
	m_charaCon.Init(10.0f, 10.0f, m_targetPointPosition);
//	m_targetPointPointLight.Init();

	// �i�r���b�V�����\�z�B
	m_nvmMesh.Init("Assets/nvm/test1.tkn");

//	SkyCube* sky = NewGO<SkyCube>(0);
//	sky->SetLuminance(0.2f);
	return true;



	return true;
}

void Game::Update()
{
	//�摜�̍X�V�B
//	m_spriteRender.Update();
//	m_pressButton.Update();

/*	bool isEnd;
	if (g_pad[0]->IsTrigger(enButtonA)) {
		// �p�X����
		m_pathFiding.Execute(
			m_path,							// �\�z���ꂽ�p�X�̊i�[��
			m_nvmMesh,						// �i�r���b�V��
			m_position,						// �J�n���W
			m_targetPointPosition,			// �ړ��ڕW���W
			PhysicsWorld::GetInstance(),	// �����G���W��	
			50.0f,							// AI�G�[�W�F���g�̔��a
			200.0f							// AI�G�[�W�F���g�̍����B
		);
	}
	// �p�X����ړ�����B
	m_position = m_path.Move(
		m_position,
		10.0f,
		isEnd
	);
	m_charaRender.SetPosition(m_position);
	m_charaRender.Update();

	Vector3 move;
	move.x = -g_pad[0]->GetLStickXF() * 10.0f;
	move.y = -10.0f;
	move.z = -g_pad[0]->GetLStickYF() * 10.0f;
	m_targetPointPosition = m_charaCon.Execute(move, 1.0f);
	m_targetPointRender.SetPosition(
		{ m_targetPointPosition.x, m_targetPointPosition.y + 100.0f, m_targetPointPosition.z }
	);
/*	m_targetPointPointLight.SetLigPoint(
		{ m_targetPointPosition.x, m_targetPointPosition.y + 100.0f, m_targetPointPosition.z }
	);
	m_targetPointPointLight.SetLigPointColor({ 100.5f, 100.5f, 100.5f });
	m_targetPointPointLight.SetPointRange(130.0f);
	m_targetPointPointLight.Update();

	m_targetPointRender.Update();

	Vector3 toCameraPos = g_camera3D->GetPosition() - g_camera3D->GetTarget();
	Vector3 newTarget = m_targetPointPosition;
	newTarget.y += 100.0f;
	Vector3 newPos = newTarget + toCameraPos;
	g_camera3D->SetTarget(newTarget);
	g_camera3D->SetPosition(newPos);
	
	
	if (g_pad[0]->IsPress(enButtonLB1))
	{
		m_fps = true;
	}
	else
	{
		m_fps = false;
	}
	*/
}

void Game::Render(RenderContext& rc)
{
	//�摜�̕`��B
	//m_spriteRender.Draw(rc);
	//m_pressButton.Draw(rc);
//	m_bgModelRendedr.Draw(rc);

	
	m_charaRender.Draw(rc);
	m_targetPointRender.Draw(rc);

	if (m_fps == true)
	{
		m_targetRender.Draw(rc);
	}
	else {
		return;
	}
}
