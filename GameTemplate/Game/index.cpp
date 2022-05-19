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
	if (diff.Length() <= 150.0f) 
	{
		win = 1;
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//カウントを+1する。
			m_player->Plusindex();
			win = 2;
			g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
			//効果音を再生する。
			SoundSource* Getse = NewGO<SoundSource>(0);
			Getse->Init(4);
			Getse->Play(false);
			Getse->SetVolume(0.4f);
		}
	}
	else {
		win = 0;
	}

	if (m_player->Getindex() == 2)
	{
		//自信を削除する。
		DeleteGO(this);
	}
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
	//☆を描画する。
	m_modelRender.Draw(rc);
	if (win == 1) {
		m_sprite.Draw(rc);
	}
}