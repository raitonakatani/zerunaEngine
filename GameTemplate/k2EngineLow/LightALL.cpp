#include "k2EngineLowPreCompile.h"
#include "LightALL.h"

namespace nsK2EngineLow {
	LightALL g_Light;


	void LightALL::Init()
	{
		// ライトのデータを作成する
/*
		// ディレクションライトを初期化する
		InitDirectionLight(m_light);
		// ポイントライトを初期化する
		InitPointLight(m_light);
		// アンビエントライトを初期化する
		InitAmbientLight(m_light);
*/
//ディレクションライト
//ライトは左側から当たっている
		m_light.directionlight.dirDirection.x = 1.0f;
		m_light.directionlight.dirDirection.y = -1.0f;
		m_light.directionlight.dirDirection.z = 1.0f;
		m_light.directionlight.dirDirection.Normalize();
		// ライトのカラーは灰色
		m_light.directionlight.dirColor.x = 0.8f;
		m_light.directionlight.dirColor.y = 0.8f;
		m_light.directionlight.dirColor.z = 0.8f;



		// 視点の位置を設定する
		//m_light.eyePos =g_camera3D->GetPosition(); 


		//ポイントライト
		// ポイントライトの座標を設定する
		m_light.pointlight.ptPosition.x = 0.0f;
		m_light.pointlight.ptPosition.y = 50.0f;
		m_light.pointlight.ptPosition.z = 50.0f;
		// ポイントライトのカラーを設定する
		m_light.pointlight.ptColor.x = 0.0f;
		m_light.pointlight.ptColor.y = 0.0f;
		m_light.pointlight.ptColor.z = 0.0f;
		// ポイントライトの影響範囲を設定する
		m_light.pointlight.ptRange = 0.0f;

		//スポットライト
		//スポットライトのデータを初期化する
		//初期座標はX = 0、Y = 50、Z = 0にする。
		m_light.spotlight.spPosition.x = 0.0f;
		m_light.spotlight.spPosition.y = 0.0f;
		m_light.spotlight.spPosition.z = 0.0f;
		//スポットライトのカラーを設定。R = 10、G = 10、B = 10にする。
		m_light.spotlight.spColor.x = 5.0f;
		m_light.spotlight.spColor.y = 5.0f;
		m_light.spotlight.spColor.z = 5.0f;
		//射出範囲は300
		m_light.spotlight.spRange = 600.0f;
		//初期方向は斜め下にする。
		m_light.spotlight.spDirection.x = 0.0f;
		m_light.spotlight.spDirection.y = -1.0f;
		m_light.spotlight.spDirection.z = 0.0f;
		//方向データなので、大きさを１にする必要があるので正規化する。
		m_light.spotlight.spDirection.Normalize();
		//射出角度は25度。
		m_light.spotlight.spAngle = Math::DegToRad(50.0f);

		//環境光
		SetAmbientLight({ 0.5f,0.5f,0.5f });

		m_light.eyePos = g_camera3D->GetPosition();

	}


	LightALL::~LightALL()
	{

	}

	void LightALL::Update()
	{
		// 視点の位置を設定する
		m_light.eyePos = g_camera3D->GetPosition();

/*		if (g_pad[0]->IsPress(enButtonX))
		{
			posi--;
		}
		if (g_pad[0]->IsPress(enButtonY))
		{
			posi++;
		}

		m_light.pointlight.ptPosition = { posi, 50.0f, 0.0f };

		Quaternion qRotY;
		qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
		//計算したクォータニオンでライトの方向を回す。
		qRotY.Apply(m_light.spotlight.spDirection);

		//X軸周りの回転クォータニオンを計算する。
		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, m_light.spotlight.spDirection);
		Quaternion qRotX;
		qRotX.SetRotation(rotAxis, g_pad[0]->GetRStickYF() * 0.01f);
		//計算したクォータニオンでライトの方向を回す。
		qRotX.Apply(m_light.spotlight.spDirection);
		*/
	}

/*
	void LightALL::InitDirectionLight(Directionlight& m_light)
	{
		// ライトは右側から当たっている
		m_light.dirDirection.x = 0.0f;
		m_light.dirDirection.y = -0.3f;
		m_light.dirDirection.z = 1.0f;
		m_light.dirDirection.Normalize();

		// ライトのカラーは(*･ω･)/ﾊｰｲ
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

	
	}

	void LightALL::InitPointLight(Pointlight& m_light)
	{
		// ポイントライトの座標を設定する
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 0.0f;

		// ポイントライトのカラーを設定する
		m_light.ptColor.x = 0.5f;
		m_light.ptColor.y = 0.5f;
		m_light.ptColor.z = 0.5f;

		// ポイントライトの影響範囲を設定する
		m_light.ptRange = 100.0f;
	}			

	void LightALL::InitAmbientLight(Ambientlight& m_light)
	{
		// アンビエントライト
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;
	}
	*/

}