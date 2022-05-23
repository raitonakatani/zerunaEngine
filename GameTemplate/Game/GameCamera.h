#pragma once

//�΂˃J�������g�p�������ꍇ�́ASpringCamera���C���N���[�h����B
//#include "camera/SpringCamera.h"

class Player;
class TankEnemy;

//�Q�[�����̃J�����𐧌䂷��B
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	/// <summary>
/// ���W��ݒ�B
/// </summary>
/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_Pos = position;
	}
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
	Vector3 toCameraPosOld;
	Vector3 m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g���B
//	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
	Player* m_player = nullptr;
	Vector3 m_Pos;	//�����_���王�_�Ɍ������x�N�g���B
//	Vector3 target;
	TankEnemy* m_tank;
	Vector3 m_position;
	Vector3 m_forward;
	Quaternion              m_rotation;                                 //��]
	Vector3 pos2;

	

private:
	Vector3 toCameraPosOld2;
	Vector3 m_toCameraPos2;	//�����_���王�_�Ɍ������x�N�g���B
//	Vector3 target2;

};