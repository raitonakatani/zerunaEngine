#include "stdafx.h"
#include "box.h"


namespace
{
	//const int END_STATE = 0;							//処理終わり
	const int START_STATE = 1;							//処理開始
	const float ANIMATION_SPEED = 1.0f;					//アニメーションスピード
}


bool box::Start()
{
	//アニメーションクリップのロード。
	m_animClips[enAnimationClip_extract].Load("Assets/modelData/alert/exclamation.tka");
	m_animClips[enAnimationClip_extract].SetLoopFlag(true);
	m_animClips[enAnimationClip_extractidle].Load("Assets/modelData/alert/exclamation.idle.tka");
	m_animClips[enAnimationClip_extractidle].SetLoopFlag(true);
	m_animClips[enAnimationClip_question].Load("Assets/modelData/alert/question.tka");
	m_animClips[enAnimationClip_question].SetLoopFlag(true);
	m_animClips[enAnimationClip_questionidle].Load("Assets/modelData/alert/question.idle.tka");
	m_animClips[enAnimationClip_questionidle].SetLoopFlag(true);



	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/alert/exclamation.tkm", true, m_animClips, enAnimationClip_Num);
	m_modelRender.SetScale(Vector3::One);
	m_modelRender.SetPosition(m_position);
	//モデルの更新。
	m_modelRender.Update();

	m_modelRender2.Init("Assets/modelData/alert/question.tkm", true, m_animClips, enAnimationClip_Num);
	m_modelRender2.SetScale(Vector3::One);
	m_modelRender2.SetPosition(m_position);
	//モデルの更新。
	m_modelRender2.Update();


	return true;
}

void box::Update()
{
	//アニメーションを再生する。
	if (m_extract== START_STATE) {
		//エクスクラメーションマーク(!)
			m_modelRender.PlayAnimation(enAnimationClip_extract);
			m_modelRender.SetAnimationSpeed(ANIMATION_SPEED);
	}
	else if (m_question == START_STATE) {
		//クエスチョンマーク(?)
			m_modelRender2.PlayAnimation(enAnimationClip_question);
			m_modelRender2.SetAnimationSpeed(ANIMATION_SPEED);
	}


	m_modelRender.SetPosition(m_position);
	//モデルの更新。
	m_modelRender.Update();

	m_modelRender2.SetPosition(m_position);
	//モデルの更新。
	m_modelRender2.Update();
}

void box::Render(RenderContext& rc)
{
	//モデルの描画。
	if (m_extract == START_STATE) {
		m_modelRender.Draw(rc);
	}
	if (m_question == START_STATE) {
		m_modelRender2.Draw(rc);
	}
}