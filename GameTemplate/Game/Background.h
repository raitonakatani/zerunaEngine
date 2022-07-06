#pragma once

class Player;

class Background : public IGameObject
{

public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	//座標を設定。
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/// <summary>
	/// 回転を設定。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 大きさを設定。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
private:
	ModelRender					m_modelRender;					//ステージ
	ModelRender					m_modelRender2;					//透視モデル
	Player*						m_player;						//プレイヤー
	PhysicsStaticObject			m_physicsStaticObject;			//静的物理オブジェクト・
	Vector3						m_position;						//座標。
	Quaternion					m_rotation;						//回転。
	Vector3						m_scale;						//大きさ。

};