#pragma once


class Player;

//�Q�[�����̃J�����𐧌䂷��B
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/////////////////////////////////////
	//�����o�ϐ�
	/////////////////////////////////////
	
	bool FPS = false;
	Vector3 target;
	int m_camera = 0;
	Vector3 posi;
	float m_timer = 0.0f;
	int rook = 0;
	int drow = 0;


private:
	Vector3					m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g���B
	Player*					m_player = nullptr;				//�v���C���[
	Vector3					m_forward;						//�O�����̃x�N�g��	
	Quaternion              m_rotation;						//��]
	Vector3					pos2;							//�J�����̈ʒu(���_)
	Vector3					toCameraPosOld2;				//�����_���王�_�Ɍ������x�N�g���B
	Vector3					m_toCameraPos2;					//�����_���王�_�Ɍ������x�N�g���B

};