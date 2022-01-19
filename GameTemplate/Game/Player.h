#pragma once
// プレイヤークラス。
class Player : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	// アニメーションを初期化。
	void InitAnimation();

private:
	// アニメーションクリップの番号を表す列挙型。
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : 待機アニメーション、
		enAnimClip_Run,		// 1 : 走りアニメーション。
		enAnimClip_Num,		// 2 :アニメーションクリップの数。
	};
	Vector3					m_position;					// 座標
	Skeleton m_skeleton;	// スケルトン
	Quaternion				m_rotation;					// 回転
	Vector3					m_scale = g_vec3One;		// 拡大率
	Animation				m_animation;				// アニメーション
	AnimationClip m_animationClipArray[enAnimClip_Num];	// アニメーションクリップ
	ModelRender				m_modelRender;
	CharacterController m_charaCon;	// キャラコン。
};