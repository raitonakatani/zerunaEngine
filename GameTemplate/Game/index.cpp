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

	//宝箱のモデルを読み込む。
	m_modelRender.Init("Assets/modelData/treasure/treasure.tkm", false);
	m_modelRender.SetScale({ 6.0f,6.0f,6.0f });
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	//静的物理オブジェクトの初期化処理。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//プレイヤーのオブジェクトを探して持ってくる。
	m_player = FindGO<Player>("player");

	return(true);
}

void index::Update()
{

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);

	//絵描きさんの更新処理。
	m_modelRender.Update();


	//プレイヤーから宝箱に向かうベクトルを計算。
	Vector3 diff = m_player->GetPosition() - m_position;
	//ベクトルの長さが150.0fより小さかったら。
	if (diff.Length() < 2000.0f && m_player->GetPlayerHaveIndex() < 4)
	{
		//ライトの調整。
		if (light >= 0.15f) {
			light -= 0.0025f;
			light2 -= 0.0025f;
			light3 += 0.2f;
		}
		//周りを暗くする。
		//宝箱にスポットライトを当てる。
		m_player->SetPlayerHaveIndex(1);
		g_Light.SetLigSpot({ m_position.x,200.0f ,m_position.z });
		g_Light.SetLigSpotColor({ light3,light3,light3 });
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
	}
	if (diff.Length() >= 2000.0f) {
		if (g_pad[0]->IsPress(enButtonY)) {
			return;
		}
		//ライトの調整。
		if (light < 0.4f) {
			light += 0.0025f;
			light2 += 0.0025f;
			light3 -= 0.2f;
		}
		if (light >= 0.4f) {
			m_player->SetPlayerHaveIndex(0);
		}
		//周りを明るくする。
		//スポットライトを消す。
		g_Light.SetAmbientLight({ light,light,light });
		g_Light.SetLigColor({ light2,light2,light2 });
		g_Light.SetLigSpotColor({ light3,light3,light3 });
	}

	if (diff.Length() <= 1500.0f && m_player->GetPlayerHaveIndex() == 1)
	{
		m_player->SetPlayerHaveIndex(2);
	}

	if (diff.Length() <= 200.0f && m_player->GetPlayerHaveIndex() == 2)
	{
		m_player->SetPlayerHaveIndex(3);
	}
}

void index::Render(RenderContext& rc)
{
	//宝箱を描画する。
	m_modelRender.Draw(rc);
}