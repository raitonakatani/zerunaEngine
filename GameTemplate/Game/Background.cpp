#include "stdafx.h"
#include "Background.h"
#include "Player.h"
#include "GameCamera.h"

namespace
{
	const int PLAYER_SP = 1;									//プレイヤーのSP
	const float YPOSITION = 300.0f;								//スポットライトのY座標
	//弱点攻撃時
	const Vector3 SPOTCOLOR = { 10.0f,10.0f,10.0f };			//スポットライトのカラー
	const Vector3 AMBIENTLIGHT = { 0.2f,0.2f,0.2f };			//環境光のカラー
	const Vector3 DIRECTIONCOLOR = { 0.3f,0.3f,0.3f };			//ディレクションライトのカラー
	//通常時
	const Vector3 AMBIENTLIGHT_NORMAL = { 0.4f,0.4f,0.4f };		//環境光のカラー
	const Vector3 DIRECTIONCOLOR_NORMAL = { 0.6f,0.6f,0.6f };	//ディレクションライトのカラー
}

bool Background::Start()
{
	//通常モデル
	m_modelRender.Init("Assets/modelData/stage1/syougai4.tkm", true);
	//モデルの座標を設定。
	m_modelRender.SetPosition(m_position);
	//モデルの回転を設定。
	m_modelRender.SetRotation(m_rotation);
	//モデルの大きさを設定。
	m_modelRender.SetScale(m_scale);
	//モデルの更新処理。
	m_modelRender.Update();
	//静的物理オブジェクトの初期化処理。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	m_physicsStaticObject.GetbtCollisionObject()->setUserIndex(enCollisionAttr_Wall);


	//透視モデル
	m_modelRender2.InitModel("Assets/modelData/stage1/syougai4.tkm");
	//モデルの座標を設定。
	m_modelRender2.SetPosition(m_position);
	//モデルの回転を設定。
	m_modelRender2.SetRotation(m_rotation);
	//モデルの大きさを設定。
	m_modelRender2.SetScale(m_scale);
	//モデルの更新処理。
	m_modelRender2.Update();
	//当たり判定の可視化。
//	PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_player = FindGO<Player>("player");

	return true;
}
void Background::Update()
{
	//弱点攻撃時のライトの処理。
	if (m_player->prok == true && m_player->GetPlayerHaveIndex() == 0)
	{
		g_Light.SetLigSpot({ m_player->GetPosition().x,YPOSITION ,m_player->GetPosition().z });
		g_Light.SetLigSpotColor(SPOTCOLOR);


		g_Light.SetAmbientLight(AMBIENTLIGHT);
		g_Light.SetLigColor(DIRECTIONCOLOR);
	}
	if (m_player->prok == false && m_player->GetPlayerHaveIndex() == 0) {
		g_Light.SetLigSpotColor(Vector3::Zero);
	}
}
void Background::Render(RenderContext& rc)
{
	//モデルの描画。
	//透視モデルのオンオフ。
	if (g_pad[0]->IsPress(enButtonY) && m_player->GetSP() >= PLAYER_SP || m_player->prok == true)
	{
		m_modelRender2.Draw(rc);
		g_Light.SetAmbientLight(AMBIENTLIGHT);
		g_Light.SetLigColor(DIRECTIONCOLOR);
	}
	else {
		m_modelRender.Draw(rc);
		g_Light.SetAmbientLight(AMBIENTLIGHT_NORMAL);
		g_Light.SetLigColor(DIRECTIONCOLOR_NORMAL);
	}
}