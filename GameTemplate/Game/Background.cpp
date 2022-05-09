#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

	//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/stage/syougai.tkm");
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


	m_modelRender2.InitModel("Assets/modelData/stage/syougai.tkm");
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
	if (g_pad[0]->IsPress(enButtonY))
	{
		m_modelRender2.Draw(rc);
		g_Light.SetAmbientLight({ .1f,.1f,.1f });
		g_Light.SetLigColor({ 0.3f,0.3f,0.3f });
	}
	else {
		m_modelRender.Draw(rc);
		g_Light.SetAmbientLight({ 0.5f,0.5f,0.5f });
		g_Light.SetLigColor({ 0.8f,0.8f,0.8f });
	}
}