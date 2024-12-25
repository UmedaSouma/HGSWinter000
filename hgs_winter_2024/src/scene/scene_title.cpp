//--------------------------------------------------------------------------------
// 
// タイトルのシーン [scene_title.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
#include "scene/scene_title.h"
#include "scene/scene_tutorial.h"
// game_manager
#include "game_manager/game_manager.h"
// input
#include "jing/input/input_manager.h"
// ecs
#include "ecs/components.h"

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
SceneTitle::SceneTitle()
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT SceneTitle::Init()
{
	// 親クラスの処理
	HRESULT hr = SceneBase::Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// 
	{
		// エンティティの作成
		auto entity = m_registry.create();

		// コンポーネントの追加
		m_registry.emplace<Polygon2DComp>(entity); // 2D ポリゴン
		auto& polygon2d = m_registry.get<Polygon2DComp>(entity);

		// パラメータの設定
		polygon2d.key = "assets\\images\\title.png";
		polygon2d.pos = Vec3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF, 0.0f);
		polygon2d.rot = Vec3(0.0f, 0.0f, 0.0f);
		polygon2d.size = Vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		polygon2d.col = Color(0xFFFFFFFF);

		// デバイスの取得
		auto device = GM.GetDevice();

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			device.Get(),
			polygon2d.key.c_str(),
			m_textures[polygon2d.key].GetAddressOf());

		// 頂点バッファの生成
		hr = device->CreateVertexBuffer(
			sizeof(Vertex2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			polygon2d.vtxBuf.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			return E_FAIL;
		}

		// 頂点バッファをロック
		Vertex2D* vtx;
		hr = polygon2d.vtxBuf->Lock(0U, 0U, reinterpret_cast<void**>(&vtx), 0);
		assert(SUCCEEDED(hr));

		// 対角線の長さを計算
		const float length = sqrtf(polygon2d.size.x * polygon2d.size.x + polygon2d.size.y * polygon2d.size.y) * HALF;

		// 対角線の角度を計算
		const float angle = atan2f(polygon2d.size.x, polygon2d.size.y);

		// 座標の計算
		float u = 1.0f / static_cast<float>(1.0f);
		float u1 = static_cast<float>(1) * u;
		float u2 = static_cast<float>(1 + 1) * u;
		float v = 1.0f / static_cast<float>(1.0f);
		float v1 = static_cast<float>(1) * v;
		float v2 = static_cast<float>(1 + 1) * v;

		// 頂点の設定
		{ // 左上
			// 頂点座標の設定
			float rot = polygon2d.rot.z - (D3DX_PI - angle);
			vtx[0].pos.x = polygon2d.pos.x + sinf(rot) * length;
			vtx[0].pos.y = polygon2d.pos.y + cosf(rot) * length;
			vtx[0].pos.z = 0.0f;

			// テクスチャ座標の設定
			vtx[0].tex = Vec2(u1, v1);
		}
		{ // 右上
			// 頂点座標の設定
			float rot = polygon2d.rot.z + (D3DX_PI - angle);
			vtx[1].pos.x = polygon2d.pos.x + sinf(rot) * length;
			vtx[1].pos.y = polygon2d.pos.y + cosf(rot) * length;
			vtx[1].pos.z = 0.0f;

			// テクスチャ座標の設定
			vtx[1].tex = Vec2(u2, v1);
		}
		{ // 左下
			// 頂点座標の設定
			float rot = polygon2d.rot.z - angle;
			vtx[2].pos.x = polygon2d.pos.x + sinf(rot) * length;
			vtx[2].pos.y = polygon2d.pos.y + cosf(rot) * length;
			vtx[2].pos.z = 0.0f;

			// テクスチャ座標の設定
			vtx[2].tex = Vec2(u1, v2);
		}
		{ // 右下
			// 頂点座標の設定
			float rot = polygon2d.rot.z + angle;
			vtx[3].pos.x = polygon2d.pos.x + sinf(rot) * length;
			vtx[3].pos.y = polygon2d.pos.y + cosf(rot) * length;
			vtx[3].pos.z = 0.0f;

			// テクスチャ座標の設定
			vtx[3].tex = Vec2(u2, v2);
		}

		for (int i = 0; i < 4; i++)
		{
			// rhw の設定
			vtx[i].rhw = 1.0f;

			// 頂点カラーの設定
			vtx[i].col = polygon2d.col;
		}

		// 頂点バッファをアンロック
		hr = polygon2d.vtxBuf->Unlock();
		assert(SUCCEEDED(hr));
	}

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void SceneTitle::Uninit()
{
	// 親クラスの処理
	SceneBase::Uninit();
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void SceneTitle::Update(float deltaTime)
{
	// 親クラスの処理
	SceneBase::Update(deltaTime);

	ImGui::Begin("Title");

	ImGui::Text("Title");

	ImGui::End();

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
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void SceneTitle::Draw() const
{
	// 親クラスの処理
	SceneBase::Draw();
}

//---------------------------------------------------
// エンティティの更新
//---------------------------------------------------
void SceneTitle::UpdateSystem(float deltaTime)
{

}

//---------------------------------------------------
// エンティティの描画
//---------------------------------------------------
void SceneTitle::DrawSystem() const
{
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
		device->SetTexture(0, texture);

		// ポリゴンの描画
		device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0U, 2U);
	}
}

//---------------------------------------------------
// 次のシーンの設定
//---------------------------------------------------
std::shared_ptr<SceneBase> SceneTitle::ChangeScene()
{
	// 終了処理
	Uninit();

	std::shared_ptr<SceneBase> nextScene = std::make_shared<SceneTutorial>();
	return nextScene;
}
