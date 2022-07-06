#include "stdafx.h"
#include "Floor.h"

bool Floor::Start()
{

	//	m_modelRender.Init("Assets/modelData/stage/RPG.stage.tkm");

	m_modelRender.Init("Assets/modelData/stage1/yuka2.tkm",true);
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
	
	return true;
}
void Floor::Update()
{
}
void Floor::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	
}