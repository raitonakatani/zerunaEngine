#include "stdafx.h"
#include "Menu.h"

bool Menu::Start()
{

	m_menuRender.Init("Assets/sprite/menu.dds", 1600.0f, 900.0f);
	m_mapRender.SetPosition({ 0.0f,0.0f,0.0f });

	return true;
}

void Menu::Update()
{
	m_menuRender.Update();
	//	m_mapRender.Update();
}

void Menu::Render(RenderContext& rc)
{
//	m_menuRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));
	m_menuRender.Draw(rc);
}