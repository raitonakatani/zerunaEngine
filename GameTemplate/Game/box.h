#pragma once
class box : public IGameObject
{
public:
	//////////////////////////////////////
	// メンバ関数。
	//////////////////////////////////////
	box() {}
	~box() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//////////////////////////////////////
	// メンバ変数。
	//////////////////////////////////////
	enum EnAnimationClip {
		enAnimationClip_extract,		//!アニメーション。
		enAnimationClip_extractidle,	//待機アニメーション。
		enAnimationClip_question,		//?アニメーション。
		enAnimationClip_questionidle,	//待機アニメーション。
		enAnimationClip_Num,			//アニメーションクリップの数。
	};
	ModelRender m_modelRender;
	ModelRender m_modelRender2;
	AnimationClip m_animClips[enAnimationClip_Num];		//アニメーションクリップ。

	Vector3 m_position;
	int m_extract = 0;		//見つけた
	int m_extractanim = 0;
	int m_question = 0;		//疑問
	int m_questionanim = 0;
};


