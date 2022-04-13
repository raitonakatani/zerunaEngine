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
	m_modelRender.Init("Assets/modelData/Portion/Portion.tkm");
	//m_modelRender.SetScale(100.0f, 100.0f, 100.0f);
	//Portion
	//プレイヤーのオブジェクトを探して持ってくる。
	m_player = FindGO<Player>("player");

	return(true);
}

void index::Update()
{

	//回転処理。
	Rotation();

	//絵描きさんに座標を教える。
	m_modelRender.SetPosition(m_position);

	//絵描きさんの更新処理。
	m_modelRender.Update();


	//プレイヤーから☆に向かうベクトルを計算。
	Vector3 diff = m_player->GetPosition() - m_position;
	//ベクトルの長さが120.0fより小さかったら。
	if (diff.Length() <= 120.0f)
	{
		//カウントを+1する。
		m_player->Plusindex();

		g_soundEngine->ResistWaveFileBank(4, "Assets/sound/4get.wav");
		//効果音を再生する。
		SoundSource* Getse = NewGO<SoundSource>(0);
		Getse->Init(4);
		Getse->Play(false);
		Getse->SetVolume(0.8f);

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
}