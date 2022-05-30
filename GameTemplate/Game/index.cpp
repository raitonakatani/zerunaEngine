#include "stdafx.h"
#include "index.h"
#include "Player.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

index::index()
{

}

index::~index()
{

}

bool index::Start()
{
	m_position.y = -150.0f;

	//☆のモデルを読み込む。
	m_modelRender.Init("Assets/modelData/treasure/treasure.tkm",false);
	m_modelRender.SetScale({ 6.0f,6.0f,6.0f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	//静的物理オブジェクトの初期化処理。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//Portion
	//プレイヤーのオブジェクトを探して持ってくる。
	m_player = FindGO<Player>("player");

	m_sprite.Init("Assets/sprite/Title/button.dds", 800, 400);
	m_sprite.SetPosition(Vector3(0.0f, -150.0f, 0.0f));
	m_sprite.Update();

	return(true);
}

void index::Update()
{

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);

	//絵描きさんの更新処理。
	m_modelRender.Update();


	//プレイヤーから☆に向かうベクトルを計算。
	Vector3 diff = m_player->GetPosition() - m_position;
	//ベクトルの長さが150.0fより小さかったら。
	if (diff.Length() < 2000.0f && m_player->index <4)
	{
		if (light >= 0.15f) {
			light -= 0.0025f;
			light2 -= 0.0025f;
			light3 += 0.15f;
		}

		m_player->index = 1;
		g_Light.SetLigSpot({ m_position.x,200.0f ,m_position.z });
		g_Light.SetLigSpotColor({light3,light3,light3 });
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
	}
	if(diff.Length() >= 2000.0f){
			if (light < 0.4f) {
			light += 0.005f;
			light2 += 0.005f;
			light3 -= 0.15f;
		}
			if (light >= 0.4f) {
				m_player->index = 0;
			}
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
		g_Light.SetLigSpotColor({ light3,light3,light3 });
	}

	if (diff.Length() <= 1000.0f && m_player->index == 1)
	{
		m_player->index = 2;
	}

	if (diff.Length() <= 200.0f && m_player->index == 2)
	{
		m_player->index = 3;
	}

	//wchar_t wcsbuf[256];
	//swprintf_s(wcsbuf, 256, L"%d", float(light));
	//m_font.SetText(wcsbuf);
	////m_font.SetText(L"Death");
	////表示する座標を設定する。
	//m_font.SetPosition(Vector3{ 0.0f,0.0f,0.0f });
	////文字の大きさを変える。
	//m_font.SetScale(3.0f);

}

void index::Rotation()
{
	//回転を加算する。
	rotation.AddRotationDegY(2.0f);

	//絵描きさんに回転を教える。
	m_modelRender.SetRotation(rotation);
}

void index::Render(RenderContext& rc)
{
	m_font.Draw(rc);
	//☆を描画する。
	m_modelRender.Draw(rc);
	/*if (win == 1) {
		m_sprite.Draw(rc);
	}*/
}