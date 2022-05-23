#include "stdafx.h"
#include "Background.h"
#include "Player.h"

bool Background::Start()
{

	//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/stage1/syougai3.tkm",true);
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


	m_modelRender2.InitModel("Assets/modelData/stage1/syougai3.tkm");
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

	//ModelInitData model;
	//model.m_alphaBlendMode = AlphaBlendMode_Trans;
	//m_model.Init(model);

}
void Background::Render(RenderContext& rc)
{
	if (g_pad[0]->IsPress(enButtonY) && m_player->m_sp >= 1)
	{
		m_modelRender2.Draw(rc);
		g_Light.SetAmbientLight({ .15f,.15f,.15f });
		g_Light.SetLigColor({ 0.4f,0.4f,0.4f });
	}
	else {
		m_modelRender.Draw(rc);
		g_Light.SetAmbientLight({ 0.4f,0.4f,0.4f });
		g_Light.SetLigColor({ 0.6f,0.6f,0.6f });
	}
}