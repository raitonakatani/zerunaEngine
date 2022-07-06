#include "stdafx.h"
#include "Background.h"
#include "Player.h"
#include "GameCamera.h"

namespace
{
	const int PLAYER_SP = 1;									//�v���C���[��SP
	const float YPOSITION = 300.0f;								//�X�|�b�g���C�g��Y���W
	//��_�U����
	const Vector3 SPOTCOLOR = { 10.0f,10.0f,10.0f };			//�X�|�b�g���C�g�̃J���[
	const Vector3 AMBIENTLIGHT = { 0.2f,0.2f,0.2f };			//�����̃J���[
	const Vector3 DIRECTIONCOLOR = { 0.3f,0.3f,0.3f };			//�f�B���N�V�������C�g�̃J���[
	//�ʏ펞
	const Vector3 AMBIENTLIGHT_NORMAL = { 0.4f,0.4f,0.4f };		//�����̃J���[
	const Vector3 DIRECTIONCOLOR_NORMAL = { 0.6f,0.6f,0.6f };	//�f�B���N�V�������C�g�̃J���[
}

bool Background::Start()
{
	//�ʏ탂�f��
	m_modelRender.Init("Assets/modelData/stage1/syougai4.tkm", true);
	//���f���̍��W��ݒ�B
	m_modelRender.SetPosition(m_position);
	//���f���̉�]��ݒ�B
	m_modelRender.SetRotation(m_rotation);
	//���f���̑傫����ݒ�B
	m_modelRender.SetScale(m_scale);
	//���f���̍X�V�����B
	m_modelRender.Update();
	//�ÓI�����I�u�W�F�N�g�̏����������B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);


	//�������f��
	m_modelRender2.InitModel("Assets/modelData/stage1/syougai4.tkm");
	//���f���̍��W��ݒ�B
	m_modelRender2.SetPosition(m_position);
	//���f���̉�]��ݒ�B
	m_modelRender2.SetRotation(m_rotation);
	//���f���̑傫����ݒ�B
	m_modelRender2.SetScale(m_scale);
	//���f���̍X�V�����B
	m_modelRender2.Update();
	//�����蔻��̉����B
//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_player = FindGO<Player>("player");

	return true;
}
void Background::Update()
{
	//��_�U�����̃��C�g�̏����B
	if (m_player->prok == true && m_player->GetPlayerHaveIndex() == 0)
	{
		g_Light.SetLigSpot({ m_player->GetPosition().x,YPOSITION ,m_player->GetPosition().z });
		g_Light.SetLigSpotColor(SPOTCOLOR);


		g_Light.SetAmbientLight(AMBIENTLIGHT);
		g_Light.SetLigColor(DIRECTIONCOLOR);
	}
	if (m_player->prok == false && m_player->GetPlayerHaveIndex() == 0) {
		g_Light.SetLigSpotColor(Vector3::Zero);
	}
}
void Background::Render(RenderContext& rc)
{
	//���f���̕`��B
	//�������f���̃I���I�t�B
	if (g_pad[0]->IsPress(enButtonY) && m_player->GetSP() >= PLAYER_SP || m_player->prok == true)
	{
		m_modelRender2.Draw(rc);
		g_Light.SetAmbientLight(AMBIENTLIGHT);
		g_Light.SetLigColor(DIRECTIONCOLOR);
	}
	else {
		m_modelRender.Draw(rc);
		g_Light.SetAmbientLight(AMBIENTLIGHT_NORMAL);
		g_Light.SetLigColor(DIRECTIONCOLOR_NORMAL);
	}
}