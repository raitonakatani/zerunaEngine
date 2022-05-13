#include "stdafx.h"
#include "Floor.h"

bool Floor::Start()
{

	//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/stage1/yuka.tkm");
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
	
	return true;
}
void Floor::Update()
{

	//	ModelInitData m_model;
	//	m_model.m_alphaBlendMode = AlphaBlendMode_Trans;

}
void Floor::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	
}