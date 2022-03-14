#pragma once

//�΂˃J�������g�p�������ꍇ�́ASpringCamera���C���N���[�h����B
//#include "camera/SpringCamera.h"

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

private:
	Vector3 toCameraPosOld;
	Vector3 m_toCameraPos = Vector3::One;	//�����_���王�_�Ɍ������x�N�g���B
//	Vector3 m_toCameraPos;	//�����_���王�_�Ɍ������x�N�g���B
	Player* m_player = nullptr;
	Vector3 m_Pos;	//�����_���王�_�Ɍ������x�N�g���B
//	Vector3 target;



	

private:
	Vector3 toCameraPosOld2;
	Vector3 m_toCameraPos2;	//�����_���王�_�Ɍ������x�N�g���B
//	Vector3 target2;

};