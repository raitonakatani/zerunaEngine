#include "k2EngineLowPreCompile.h"
#include "RenderingEngine.h"


namespace nsK2EngineLow {

    RenderingEngine g_renderEngine;

    void RenderingEngine::SpriteRenderDraw(RenderContext& rc)
    {

        for (auto& renderObj : m_Spriterenders) {
            renderObj->OnRender2D(rc);
        }
        //
        m_Spriterenders.clear();

    }

    void RenderingEngine::Update()
    {
        // �V�[�����C�g�̍X�V�B
        g_Light.Update();
    }

}