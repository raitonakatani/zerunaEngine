#pragma once

namespace nsK2EngineLow {

    class DirectionalLight
    {
    public:
        /// <summary>
        /// ライト構造体。
        /// </summary>
       
         
        struct Light {

            // ディレクションライト用のメンバ
            Vector3 dirDirection;   // ライトの方向
            float pad0;
            Vector3 dirColor;       // ライトのカラー
            float pad1;

            // step-1 ライト構造体にポイントライト用のメンバ変数を追加する
            Vector3 ptPosition;		//位置。
            float pad2;					//パディング。
            Vector3 ptColor;			//カラー。
            float ptRange;				//影響範囲。

               // step-1 ライト構造体にスポットライト用のメンバ変数を追加
            Vector3 spPosition;		//位置。
            float pad3;					//パディング。
            Vector3 spColor;			//カラー。
            float spRange;				//影響範囲。
            Vector3 spDirection;	//射出方向。
            float spAngle;				//射出角度。

            Vector3 eyePos;         // 視点の位置

            Vector3 ambientLight;   // アンビエントライト
        };
        
        
       /*
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
        //    float pad3;
            Vector3 ambientLight;
        };
        /// <summary>
        /// ライト構造体。
        /// </summary>
        struct Light {
            Pointlight pointlight; //ポイントライト。
            Ambientlight ambientlight; //アンビエントライト。(環境光)
            Directionlight directionLight; //ディレクションライト。
            Spotlight spotlight; //スポットライト。
            Vector3 eyePos;			//視点の位置。
        };
       */

    public:
        DirectionalLight();
        ~DirectionalLight();

 //       void InitDirectionLight(Light& m_light);
//        void InitPointLight(Light& m_light);
 //       void InitAmbientLight(Light& m_light);


        /// <summary>
        /// ライトの方向。
        /// </summary>
        /// <param name="light">方向</param>
        void SetLigDirection(const Vector3& light)
        {
            m_light.dirDirection = light;
        }
        /// <summary>
        /// ライトのカラー。
        /// </summary>
        /// <param name="light">カラー</param>
        void SetLigColor(const Vector3& color)
        {
            m_light.dirColor = color;
        }

        void SetLigSpot(const Vector3& posi)
        {
            m_light.spPosition = posi;
        }


        /// <summary>
         /// ライトの場所。
         /// </summary>
         /// <param name="light">場所</param>
        void SetLigPoint(const Vector3& lightposi)
        {
            m_light.ptPosition = lightposi;
        }

        Light& GetDirectionalLight()
        {
            return m_light;
        }

        Light& GetPointLight()
        {
            return m_light;
        }


    private:
        Light               m_light;  //シーンライト。
        ModelRender         m_modelRender;
        ModelInitData       m_InitData;
    };
    //グローバル変数。 (extern)
    extern DirectionalLight g_directionalLight;
};