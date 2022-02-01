#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

	m_modelRender.Init("Assets/modelData/bg/bg.tkm");
	
	// 静的物理オブジェクトを作成。
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