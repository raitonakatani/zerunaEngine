#pragma once
namespace nsK2EngineLow {


    class PointLight
    {
    public:
        struct Pointlight {
            // step-1 ライト構造体にポイントライト用のメンバ変数を追加する
            Vector3 ptPosition;		//位置。
            float pad2;					//パディング。
            Vector3 ptColor;			//カラー。
            float ptRange;				//影響範囲。
        };
        Pointlight pointlight;



    };

    //グローバル変数。 (extern)
    extern PointLight g_PointLight;
};