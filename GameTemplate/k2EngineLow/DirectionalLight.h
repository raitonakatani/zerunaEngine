#pragma once

namespace nsK2EngineLow {

	class DirectionalLight
    {
    public:
        struct Directionlight {
           Vector3 ligDirection;	//ライトの方向。
           float pad0;
           Vector3 ligColor;
           float pad1;
       };
        struct Ambientlight {
            Vector3 ambientColor;
            float pad;
        };
       /// <summary>
       /// ライト構造体。
       /// </summary>
       struct Light {
           Ambientlight ambientlight;
           Directionlight directionLight;		//ディレクションライト。
           Vector3 eyePos;						//視点の位置。
       };

    public:
        DirectionalLight();
        ~DirectionalLight();
    
        void Render(RenderContext& rc);

        /// <summary>
        /// ライトの方向。
        /// </summary>
        /// <param name="light">方向</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.directionLight.ligDirection = light;
        }

        /// <summary>
        /// ライトのカラー。
        /// </summary>
        /// <param name="light">カラー</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.directionLight.ligColor= color;
        }

        Light& GetDirectionalLight()
        {
            return m_light;
        }

    private:
        Light               m_light;  //シーンライト。
        Model               model;
        ModelInitData       m_InitData;
    };

    //グローバル変数。 (extern)
    extern DirectionalLight g_directionalLight;
}