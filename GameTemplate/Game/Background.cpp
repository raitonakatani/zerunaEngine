#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{

	//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/stage/syougai.tkm");
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


	m_modelRender2.InitModel("Assets/modelData/stage/syougai.tkm");
	//モデルの座標を設定。
	m_modelRender2.SetPosition(m_position);
	//モデルの回転を設定。
	m_modelRender2.SetRotation(m_rotation);
	//モデルの大きさを設定。
	m_modelRender2.SetScale(m_scale);
	//モデルの更新処理。
	m_modelRender2.Update();

	//当たり判定の可視化。
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