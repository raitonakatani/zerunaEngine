#pragma once
class box : public IGameObject
{
public:
	//////////////////////////////////////
	// �����o�֐��B
	//////////////////////////////////////
	box() {}
	~box() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//////////////////////////////////////
	// �����o�ϐ��B
	//////////////////////////////////////
	enum EnAnimationClip {
		enAnimationClip_extract,		//!�A�j���[�V�����B
		enAnimationClip_extractidle,	//�ҋ@�A�j���[�V�����B
		enAnimationClip_question,		//?�A�j���[�V�����B
		enAnimationClip_questionidle,	//�ҋ@�A�j���[�V�����B
		enAnimationClip_Num,			//�A�j���[�V�����N���b�v�̐��B
	};
	ModelRender m_modelRender;
	ModelRender m_modelRender2;
	AnimationClip m_animClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B

	Vector3 m_position;
	int m_extract = 0;		//������
	int m_extractanim = 0;
	int m_question = 0;		//�^��
	int m_questionanim = 0;
};


