#include "k2EngineLowPreCompile.h"
#include "SpriteRender.h"

namespace nsK2EngineLow {

	void SpriteRender::Init(const char* filePath, const float w, const float h, AlphaBlendMode alphaBlendMode)
	{
		SpriteInitData InitData;
		//�e�N�X�`����mainRenderTarget�̃J���[�o�b�t�@�B
		InitData.m_ddsFilePath[0] = filePath;
		InitData.m_fxFilePath = "Assets/shader/sprite.fx";
		InitData.m_width = static_cast<UINT>(w);
		InitData.m_height = static_cast<UINT>(h);
		InitData.m_alphaBlendMode = alphaBlendMode;
		InitData.m_colorBufferFormat[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//���m�N���p�̃V�F�[�_�[���w�肷��B
		//�������I�u�W�F�N�g���g���āA�X�v���C�g������������B
		m_sprite.Init(InitData);
	}

	void SpriteRender::Draw(RenderContext& rc)
	{
		g_renderingEngine.AddRenderObject(this);
	}
}
