#pragma once

class Background : public IGameObject
{

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得する。
	/// </summary>
	/// <returns>座標。</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private:
	ModelRender m_modelRender;					//モデル
	Model m_model;
	PhysicsStaticObject m_physicsStaticObject;	//静的物理オブジェクト・
	BoxCollider m_boxCollider;
	RigidBody m_rigidBody;
	ModelInitData initData;
	Vector3		  m_position;
	Quaternion	  m_rotation;
	Vector3		  m_scale;
};