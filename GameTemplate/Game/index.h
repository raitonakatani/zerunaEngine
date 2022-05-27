#pragma once
class Player;

	//☆クラス。
class index : public IGameObject
{
public:
	index();
	~index();
	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);
	//回転処理。
	void Rotation();

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


	int win = 0;

private:
	//ここからメンバ変数。
	ModelRender m_modelRender;            //モデルレンダー。
	Vector3						m_position;							//座標。
	Quaternion					m_rotation;
	Vector3						m_scale;
	Vector3 m_firstPosition;             //最初の座標。
	int moveState = 0;                   //上に移動か下に移動か。
	Quaternion rotation;                //クォータニオン。
	Player* m_player;                   //プレイヤー。
	SoundSource* se;                    //se

	float light = 0.4f;
	float light2 = 0.6f;
	float light3 = 0.0f;
	FontRender m_font;
	SpriteRender m_sprite;
	PhysicsStaticObject m_physicsStaticObject;	//静的物理オブジェクト・
};

