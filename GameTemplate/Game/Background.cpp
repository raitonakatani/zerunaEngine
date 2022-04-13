#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/karisute/stage3.tkm");
	//モデルの座標を設定。
	m_modelRender.SetPosition(m_position);
	//モデルの回転を設定。
	m_modelRender.SetRotation(m_rotation);
	//モデルの大きさを設定。
	m_modelRender.SetScale(m_scale);
	//モデルの更新処理。
	m_modelRender.Update();
	//静的物理オブジェクトの初期化処理。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);
	//当たり判定の可視化。
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