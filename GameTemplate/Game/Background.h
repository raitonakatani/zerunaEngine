#pragma once

class Background : public IGameObject
{

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	Model m_model;								//モデル
	PhysicsStaticObject m_physicsStaticObject;	//静的物理オブジェクト・
	BoxCollider m_boxCollider;
	RigidBody m_rigidBody;
};