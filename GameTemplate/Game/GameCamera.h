#pragma once

//�΂˃J�������g�p�������ꍇ�́ASpringCamera���C���N���[�h����B
//#include "camera/SpringCamera.h"

class fastPlayer;
class Player;
class WarpCounter;

//�Q�[�����̃J�����𐧌䂷��B
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

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
private:
	void DeathCamera();
	void FightCamera();
private:

	fastPlayer* m_fastplayer = nullptr;	//�v���C���[�B
	Player* m_player = nullptr;
	WarpCounter* warpCounter = nullptr;
	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
	Vector3 m_Pos;	//�����_���王�_�Ɍ������x�N�g���B
//	SpringCamera m_springCamera;	//�΂˃J�����B

	Vector3 target;
	Vector3 pos;
	Vector3 posi;

	float YZIKU = 200.0f;
	float ZZIKU = -200.0f;
};