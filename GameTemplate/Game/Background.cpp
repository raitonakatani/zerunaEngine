#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/karisute/stage.tkm");
	
//	m_modelRender.SetPosition({ 0.0f,-900.0f,0.0f });
//	m_modelRender.SetScale({ 0.5f,0.5f,0.5 });
//	m_modelRender.Update();

	// 静的物理オブジェクトを作成。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);

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