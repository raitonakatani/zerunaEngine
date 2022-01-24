#include "k2EngineLowPreCompile.h"
#include "DirectionalLight.h"

namespace nsK2EngineLow {
	DirectionalLight g_directionalLight;


	DirectionalLight::DirectionalLight()
	{
		// ディレクションライトのデータを設定する
	 // ライトは右側から当たっている
		m_light.directionLight.dirDirection.x = 1.0f;
		m_light.directionLight.dirDirection.y = -1.0f;
		m_light.directionLight.dirDirection.z = -1.0f;
		m_light.directionLight.dirDirection.Normalize();

		// ライトのカラーは白
		m_light.directionLight.dirColor.x = 0.5f;//R 赤
		m_light.directionLight.dirColor.y = 0.5f;//G 緑
		m_light.directionLight.dirColor.z = 0.5f;//B 青

		//視点の位置を設定する。
		m_light.ambientlight.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });//g_camera3D->GetPosition();

		// 環境光
		m_light.ambientlight.ambientColor.x = 0.3f;
		m_light.ambientlight.ambientColor.y = 0.3f;
		m_light.ambientlight.ambientColor.z = 0.3f;
		
		// step-2 ポイントライトの初期座標を設定する
		m_light.pointlight.ptPosition.x = 0.0f;
		m_light.pointlight.ptPosition.y = 50.0f;
		m_light.pointlight.ptPosition.z = 50.0f;
		// step-3 ポイントライトの初期カラーを設定する
		m_light.pointlight.ptColor.x = 0.0f;
		m_light.pointlight.ptColor.y = 0.0f;
		m_light.pointlight.ptColor.z = 0.0f;
		// step-4 ポイントライトの影響範囲を設定する
		m_light.pointlight.ptRange = 100.0f;



		m_light.spotlight.spPosition.x = 0.0f;
		m_light.spotlight.spPosition.y = 0.0f;
		m_light.spotlight.spPosition.y = 50.0f;
		//ライトのカラーを設定。R = 10、G = 10、B = 10にする。
		m_light.spotlight.spColor.x = 10.0f;
		m_light.spotlight.spColor.y = 10.0f;
		m_light.spotlight.spColor.z = 10.0f;
		
		//初期方向は斜め下にする。
		m_light.spotlight.spDirection.x = 1.0f;
		m_light.spotlight.spDirection.y = -1.0f;
		m_light.spotlight.spDirection.z = 1.0f;
		//方向データなので、大きさを１にする必要があるので正規化する。
		m_light.spotlight.spDirection.Normalize();
		
		//射出範囲は300
		m_light.spotlight.spRange = 300.0f;
		//射出角度は25度。
		m_light.spotlight.spAngle = Math::DegToRad(25.0f);

	}


	DirectionalLight::~DirectionalLight()
	{
		
	}	
}