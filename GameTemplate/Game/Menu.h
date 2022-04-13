#pragma once

class Menu :public IGameObject
{
public:
	Menu() {}
	~Menu() {}

	bool Start();
	//更新処理。
	void Update();
	//描画処理。
	void Render(RenderContext& rc);

private:
	SpriteRender m_menuRender;
	SpriteRender m_mapRender;

};

