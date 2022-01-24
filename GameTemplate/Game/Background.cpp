#include "stdafx.h"
#include "Background.h"

bool Background::Start()
{
/*	initData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
	// シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/model.fx";
	// 頂点シェーダーのエントリーポイント
	initData.m_vsEntryPointFunc = "VSMain";
	m_model.Init(initData);

	m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());
*/

	m_modelRender.Init("Assets/modelData/bg/bg.tkm");
	
	// 静的物理オブジェクトを作成。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	return true;
}
void Background::Update()
{
	//m_modelRender.Update();
}
void Background::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}