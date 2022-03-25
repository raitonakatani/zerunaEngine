#pragma once

class Background : public IGameObject
{

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;					//���f��
	Model m_model;
	PhysicsStaticObject m_physicsStaticObject;	//�ÓI�����I�u�W�F�N�g�E
	BoxCollider m_boxCollider;
	RigidBody m_rigidBody;
	ModelInitData initData;
};