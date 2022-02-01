#include "k2EngineLowPreCompile.h"
#include "DirectionalLight.h"

namespace nsK2EngineLow {
	DirectionalLight g_directionalLight;


	DirectionalLight::DirectionalLight()
	{
			// ライトのデータを作成する

/*		// ディレクションライトを初期化する
		InitDirectionLight(m_light);
		// ポイントライトを初期化する
		InitPointLight(m_light);
		// アンビエントライトを初期化する
		InitAmbientLight(m_light);
*/
		// ライトは右側から当たっている
		m_light.dirDirection.x = 1.0f;
		m_light.dirDirection.y = -1.0f;
		m_light.dirDirection.z = -1.0f;
		m_light.dirDirection.Normalize();

		// ライトのカラーは白
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

		// 視点の位置を設定する
		m_light.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });

		// ポイントライトの座標を設定する
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;

		// ポイントライトのカラーを設定する
		m_light.ptColor.x = 10.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;

		// ポイントライトの影響範囲を設定する
		m_light.ptRange = 100.0f;

		// アンビエントライト
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;


		// step-2 スポットライトのデータを初期化する
	  //初期座標はX = 0、Y = 50、Z = 0にする。
		m_light.spPosition.x = 0.0f;
		m_light.spPosition.y = 50.0f;
		m_light.spPosition.z = 0.0f;
		//ライトのカラーを設定。R = 10、G = 10、B = 10にする。
		m_light.spColor.x = 10.0f;
		m_light.spColor.y = 10.0f;
		m_light.spColor.z = 10.0f;

		//初期方向は斜め下にする。
		m_light.spDirection.x = 1.0f;
		m_light.spDirection.y = -1.0f;
		m_light.spDirection.z = 1.0f;
		//方向データなので、大きさを１にする必要があるので正規化する。
		m_light.spDirection.Normalize();

		//射出範囲は300
		m_light.spRange = 300.0f;
		//射出角度は25度。
		m_light.spAngle = Math::DegToRad(25.0f);
	}


	DirectionalLight::~DirectionalLight()
	{

	}

	/*
	void DirectionalLight::InitDirectionLight(Light& m_light)
	{
		// ライトは右側から当たっている
		m_light.dirDirection.x = 1.0f;
		m_light.dirDirection.y = -1.0f;
		m_light.dirDirection.z = -1.0f;
		m_light.dirDirection.Normalize();

		// ライトのカラーは白
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

		// 視点の位置を設定する
		m_light.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });
	}

	void DirectionalLight::InitPointLight(Light& m_light)
	{
		// ポイントライトの座標を設定する
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;

		// ポイントライトのカラーを設定する
		m_light.ptColor.x = 0.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;

		// ポイントライトの影響範囲を設定する
		m_light.ptRange = 100.0f;
	}

	void DirectionalLight::InitAmbientLight(Light& m_light)
	{
		// アンビエントライト
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;
	}
	*/
}