#pragma once
// �v���C���[�N���X�B
class Player : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	// �A�j���[�V�������������B
	void InitAnimation();

private:
	// �A�j���[�V�����N���b�v�̔ԍ���\���񋓌^�B
	enum EnAnimationClip {
		enAnimClip_Idle,	// 0 : �ҋ@�A�j���[�V�����A
		enAnimClip_Run,		// 1 : ����A�j���[�V�����B
		enAnimClip_Num,		// 2 :�A�j���[�V�����N���b�v�̐��B
	};
	Vector3					m_position;					// ���W
	Skeleton m_skeleton;	// �X�P���g��
	Quaternion				m_rotation;					// ��]
	Vector3					m_scale = g_vec3One;		// �g�嗦
	Animation				m_animation;				// �A�j���[�V����
	AnimationClip m_animationClipArray[enAnimClip_Num];	// �A�j���[�V�����N���b�v
	ModelRender				m_modelRender;
	CharacterController m_charaCon;	// �L�����R���B
};