#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/karisute/stage3.tkm");
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
	//�����蔻��̉����B
//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	return true;
}
void Background::Update()
{

}
void Background::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}