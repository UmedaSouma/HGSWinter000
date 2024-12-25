//--------------------------------------------------------------------------------
// 
// ゲームのシーン [scene_game.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
#include "scene/scene_game.h"
#include "scene/scene_result.h"
// game_manager
#include "game_manager/game_manager.h"
// input
#include "jing/input/input_manager.h"
//ecs
#include "ecs/components.h"

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
SceneGame::SceneGame()
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT SceneGame::Init()
{
	// 親クラスの処理
	HRESULT hr = SceneBase::Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// エンティティの生成
	{
		CreatePolygon2D(
			"assets\\images\\dot_house_00.png",
			Vec3(100.0f, 100.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(64.0f, 64.0f, 0.0f),
			Color(0xFFFFFFFF),
			1U, 1U, 1U);

		CreatePolygon2D(
			"assets\\images\\game_bg.png",
			Vec3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
			Color(0xFFFFFFFF),
			1U, 1U, 1U);

	
	}

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void SceneGame::Uninit()
{
	// 親クラスの処理
	SceneBase::Uninit();
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void SceneGame::Update(float deltaTime)
{
	// 親クラスの処理
	SceneBase::Update(deltaTime);

#ifdef _DEBUG
	Jing::InputManager& im = Jing::InputManager::GetInstance(); // 入力処理のマネージャー
	const Jing::Keyboard& keyboard = im.GetKeyboard(); (void)keyboard; // キーボード
	const Jing::Mouse& mouse = im.GetMouse(); (void)mouse; // マウス
	const Jing::Gamepad& gamepad = im.GetGamepad(); (void)gamepad; // ゲームパッド

	// 画面遷移
	if (keyboard.GetTrigger(DIK_RETURN) ||
		gamepad.GetButtonTrigger(Jing::Gamepad::ButtonType::A) ||
		gamepad.GetButtonTrigger(Jing::Gamepad::ButtonType::Start))
	{
		GM.ChangeScene();
	}
#endif
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void SceneGame::Draw() const
{
	// 親クラスの処理
	SceneBase::Draw();
}

//---------------------------------------------------
// エンティティの更新
//---------------------------------------------------
void SceneGame::UpdateSystem(float deltaTime)
{
	// 2D ポリゴンの更新
	auto view = m_registry.view<Polygon2DComp>();
	for (auto entity : view)
	{
		auto& polygon2d = view.get<Polygon2DComp>(entity);

		// 頂点バッファをロック
		Vertex2D* pVtx;
		HRESULT hr = polygon2d.vtxBuf->Lock(0U, 0U, reinterpret_cast<void**>(&pVtx), 0U);
		assert(SUCCEEDED(hr));

		// 対角線の長さを計算
		const float length = sqrtf(polygon2d.size.x * polygon2d.size.x + polygon2d.size.y * polygon2d.size.y) * HALF;

		// 対角線の角度を計算
		const float angle = atan2f(polygon2d.size.x, polygon2d.size.y);

		// 座標の計算
		float u = 1.0f / static_cast<float>(polygon2d.numU);
		float u1 = static_cast<float>(polygon2d.nowNumU) * u;
		float u2 = static_cast<float>(polygon2d.nowNumU + 1) * u;
		float v = 1.0f / static_cast<float>(polygon2d.numV);
		float v1 = static_cast<float>(polygon2d.nowNumV) * v;
		float v2 = static_cast<float>(polygon2d.nowNumV + 1) * v;

		// 頂点座標の設定
		pVtx[0].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z - (D3DX_PI - angle)) * length, polygon2d.pos.y + cosf(polygon2d.rot.z - (D3DX_PI - angle)) * length, 0.0f);
		pVtx[1].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z + (D3DX_PI - angle)) * length, polygon2d.pos.y + cosf(polygon2d.rot.z + (D3DX_PI - angle)) * length, 0.0f);
		pVtx[2].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z - angle) * length, polygon2d.pos.y + cosf(polygon2d.rot.z - angle) * length, 0.0f);
		pVtx[3].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z + angle) * length, polygon2d.pos.y + cosf(polygon2d.rot.z + angle) * length, 0.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = Vec2(u1, v1);
		pVtx[1].tex = Vec2(u2, v1);
		pVtx[2].tex = Vec2(u1, v2);
		pVtx[3].tex = Vec2(u2, v2);

		// 頂点カラーの設定
		for (int i = 0; i < 4; i++)
		{
			pVtx[i].col = polygon2d.col;
		}

		// 頂点バッファをアンロック
		hr = polygon2d.vtxBuf->Unlock();
		assert(SUCCEEDED(hr));
	}
}

//---------------------------------------------------
// エンティティの描画
//---------------------------------------------------
void SceneGame::DrawSystem() const
{
	// 2D ポリゴンの描画
	auto view = m_registry.view<Polygon2DComp>();
	for (auto entity : view)
	{
		const Polygon2DComp& polygon2D = view.get<Polygon2DComp>(entity);

		// デバイスの取得
		auto device = GM.GetDevice();

		Texture texture = m_textures[polygon2D.key].Get();

		// 頂点バッファをデータストリームに設定
		device->SetStreamSource(0U, polygon2D.vtxBuf.Get(), 0U, sizeof(Vertex2D));

		// 頂点フォーマットの設定
		device->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		device->SetTexture(0U, texture);

		// ポリゴンの描画
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0U, 2U);
	}
}

//---------------------------------------------------
// 次のシーンの設定
//---------------------------------------------------
std::shared_ptr<SceneBase> SceneGame::ChangeScene()
{
	// 終了処理
	Uninit();

	std::shared_ptr<SceneBase> nextScene = std::make_shared<SceneResult>();
	return nextScene;
}
