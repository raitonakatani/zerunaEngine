#include "k2EngineLowPreCompile.h"
#include "DirectionalLight.h"

namespace nsK2EngineLow {
	DirectionalLight g_directionalLight;


	DirectionalLight::DirectionalLight()
	{

		//ライトは右側から当たっている。
		m_light.directionLight.ligDirection.x = 1.0f;
		m_light.directionLight.ligDirection.y = -1.0f;
		m_light.directionLight.ligDirection.z = 1.0f;
		m_light.directionLight.ligDirection.Normalize();
		//ライトのカラーは白。
		m_light.directionLight.ligColor.x = .5f;
		m_light.directionLight.ligColor.y = .5f;
		m_light.directionLight.ligColor.z = .5f;


		//環境光
		//ライトのカラーは白。
		m_light.ambientlight.ambientColor.x = 2.0f;
		m_light.ambientlight.ambientColor.y = 2.0f;
		m_light.ambientlight.ambientColor.z = 2.0f;
		///

		//視点の位置を設定する。
		m_light.eyePos = (Vector3{ 0.0f, 0.0f, 1.0f });//g_camera3D->GetPosition();

	//	m_light.eyePos = g_camera3D->GetPosition();
	//	m_light.numPointLight = 0;


//		m_InitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
		//
//		m_InitData.m_fxFilePath = "Assets/shader/model.fx";
		//
		//
//		m_InitData.m_expandConstantBuffer = &directionLig;
//		m_InitData.m_expandConstantBufferSize = sizeof(directionLig);

		
//		model.Init(m_InitData);
		
//		auto& renderContext = g_graphicsEngine->GetRenderContext();
	}


	DirectionalLight::~DirectionalLight()
	{

	}

	void DirectionalLight::Render(RenderContext& rc)
	{
		//モデルをドロー
		model.Draw(rc);
	}
	
}