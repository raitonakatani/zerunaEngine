#pragma once

class Menu :public IGameObject
{
public:
	Menu() {}
	~Menu() {}

	bool Start();
	//�X�V�����B
	void Update();
	//�`�揈���B
	void Render(RenderContext& rc);

private:
	SpriteRender m_menuRender;
	SpriteRender m_mapRender;

};

