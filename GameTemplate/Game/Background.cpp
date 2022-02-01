#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

	m_modelRender.Init("Assets/modelData/bg/bg.tkm");
	
	// �ÓI�����I�u�W�F�N�g���쐬�B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

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