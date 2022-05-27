#include "stdafx.h"
#include "box.h"


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
	m_modelRender.SetScale({ 1.0f,1.0f,1.0f });
	m_modelRender.SetPosition(m_position);
	//モデルの更新。
	m_modelRender.Update();

	m_modelRender2.Init("Assets/modelData/alert/question.tkm", true, m_animClips, enAnimationClip_Num);
	m_modelRender2.SetScale({ 1.0f,1.0f,1.0f });
	m_modelRender2.SetPosition(m_position);
	//モデルの更新。
	m_modelRender2.Update();


	return true;
}

void box::Update()
{
	//アニメーションを再生する。
	if (m_extract==1) {
	
		//エクスクラメーションマーク(!)
		if (m_extract == 1 && m_extractanim == 0) {
			m_modelRender.PlayAnimation(enAnimationClip_extract);
			m_modelRender.SetAnimationSpeed(0.8f);
			if (m_modelRender.IsPlayingAnimation() == false) {
				m_extractanim = 1;
			}
		}
		 if (m_extractanim == 1) {
			m_modelRender.PlayAnimation(enAnimationClip_extractidle);
			m_modelRender.SetAnimationSpeed(0.8f);
		}
	}
	else if (m_question == 1 && m_questionanim ==0) {
		//クエスチョンマーク(?)
		if (m_question == 1) {
			m_modelRender2.PlayAnimation(enAnimationClip_question);
			m_modelRender2.SetAnimationSpeed(0.8f);
			if (m_modelRender2.IsPlayingAnimation() == false) {
				m_questionanim= 1;
			}
		}
		 if (m_questionanim == 1) {
			m_modelRender2.PlayAnimation(enAnimationClip_questionidle);
			m_modelRender2.SetAnimationSpeed(0.8f);
		}
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
	if (m_extract == 1) {
		m_modelRender.Draw(rc);
	}
	if (m_question == 1) {
		m_modelRender2.Draw(rc);
	}
}