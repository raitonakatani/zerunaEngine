#pragma once

namespace nsK2EngineLow {


	class LightALL
	{
        /// <summary>
       /// ライト構造体。
       /// </summary>
    public:

        struct Directionlight {
            Vector3 dirDirection;	//ライトの方向。
            float pad0;
            Vector3 dirColor;
            float pad1;
        };
        struct Pointlight {
            Vector3 ptPosition;		//位置。
            float pad2;				//パディング。
            Vector3 ptColor;		//カラー。
            float ptRange;			//影響範囲。
        };
        struct Spotlight {
            Vector3 spPosition;		//位置。
            float pad3;				//パディング。
            Vector3 spColor;		//カラー。
            float spRange;			//影響範囲。
            Vector3 spDirection;	//射出方向。
            float spAngle;			//射出角度。
        };
        struct Ambientlight {
            Vector3 ambientLight;
            float pad;
        };

        struct Light {

            Pointlight pointlight; //ポイントライト。
            Ambientlight ambientlight; //アンビエントライト。(環境光)
            Directionlight directionlight; //ディレクションライト。
            Spotlight spotlight; //スポットライト。
            Vector3 eyePos;			//視点の位置。
        };


    public:
        LightALL() {}
        ~LightALL();

        void Update();

      //       void InitDirectionLight(Directionlight& m_light);
      //       void InitPointLight(Pointlight& m_light);
      //       void InitAmbientLight(Ambientlight& m_light);
        void Init();

               /// <summary>
               /// ディレクションライトの方向。
               /// </summary>
               /// <param name="light">方向</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.directionlight.dirDirection = light;
        }
        /// <summary>
        /// ディレクションライトのカラー。
        /// </summary>
        /// <param name="light">カラー</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.directionlight.dirColor = color;
        }



        /// <summary>
         /// ポイントライトの場所。
         /// </summary>
         /// <param name="light">場所</param>
        void SetLigPoint(const Vector3& lightposi)
        {
            m_light.pointlight.ptPosition = lightposi;
        }

        /// <summary>
         /// ポイントライトのカラー。
         /// </summary>
         /// <param name="light">カラー</param>
        void SetLigPointColor(const Vector3& lightcola)
        {
            m_light.pointlight.ptColor = lightcola;
        }
        /// <summary>
        /// 範囲を設定。
        /// </summary>
        /// <param name="range"></param>
        void SetPointRange(float range)
        {
            m_light.pointlight.ptRange = range;
        }

        /// <summary>
         /// スポットライトの回転。
         /// </summary>
         /// <param name="light">回転</param>
        void SetLigSpotRot(const Vector3& lightrot)
        {
            m_light.spotlight.spDirection = lightrot;
        }

        /// <summary>
        /// スポットライトの場所。
        /// </summary>
        /// <param name="light">場所</param>
        void SetLigSpot(const Vector3& posi)
        {
            m_light.spotlight.spPosition = posi;
        }

        

        Light& GetDirectionalLight()
        {
            return m_light;
        }



    private:
        Light               m_light;  //シーンライト。
   
        float posi = 0.0f;

        ModelRender         m_modelRender;
        ModelInitData       m_InitData;
    };
    //グローバル変数。 (extern)
    extern LightALL g_Light;
};

