//--------------------------------------------------------------------------------
// 
// シーンの基底 [scene_base.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
#include "scene/scene_base.h"
#include "scene/scene_game.h"
// game_manager
#include "game_manager/game_manager.h"
// ecs
#include "ecs/components.h"

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
SceneBase::SceneBase()
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT SceneBase::Init()
{
	// テクスチャの読み込み
	{
		// テクスチャのキー
		std::vector<std::string> keys =
		{
			"assets\\images\\title.png",
			"assets\\images\\game_bg.png",
			"assets\\images\\result.png",
			"assets\\images\\dot_house_00.png",
		};

		// デバイスの取得
		auto device = GM.GetDevice();

		// テクスチャの読み込み
		for (const auto& key : keys)
		{
			// テクスチャの読み込み
			HRESULT hr = D3DXCreateTextureFromFile(
				device.Get(),
				key.c_str(),
				m_textures[key].GetAddressOf());
			if (FAILED(hr))
			{
				assert(false && "テクスチャの読み込みに失敗！");
				return hr;
			}
		}
	}	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void SceneBase::Uninit()
{
	// レジストリのクリア
	m_registry.clear();
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void SceneBase::Update(float deltaTime)
{
	// シーンの更新
	UpdateSystem(deltaTime);
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void SceneBase::Draw() const
{
	// シーンの描画
	DrawSystem();
}

//---------------------------------------------------
// 次のシーンの設定
//---------------------------------------------------
std::shared_ptr<SceneBase> SceneBase::ChangeScene()
{
	std::cout << "次のシーンがありません！" << std::endl;
	return shared_from_this();
}

//---------------------------------------------------
// 2D ポリゴンの作成
//---------------------------------------------------
entt::entity SceneBase::CreatePolygon2D(
	const std::string& key,
	const Vec3& pos,
	const Vec3& rot,
	const Vec3& size,
	const Color& col,
	UINT numU,
	UINT numV,
	UINT interval)
{
	// エンティティの作成
	auto entity = m_registry.create();

	// コンポーネントの追加
	m_registry.emplace<Polygon2DComp>(entity); // 2D ポリゴン

	// 2D ポリゴンの初期設定
	{
		auto& polygon2d = m_registry.get<Polygon2DComp>(entity);

		// パラメータの設定
		polygon2d.key = key;
		polygon2d.pos = pos;
		polygon2d.rot = rot;
		polygon2d.size = size;
		polygon2d.col = col;
		polygon2d.numU = numU;
		polygon2d.numV = numV;
		polygon2d.interval = interval;

		// デバイスの取得
		auto device = GM.GetDevice();

		if (m_textures.find(polygon2d.key) == m_textures.end())
		{
			assert(false && "テクスチャが見つかりません！");
			return entity;
		}

		// 頂点バッファの生成
		HRESULT hr = device->CreateVertexBuffer(
			sizeof(Vertex2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			polygon2d.vtxBuf.GetAddressOf(),
			nullptr);
		if (FAILED(hr))
		{
			assert(false && "頂点バッファの生成に失敗！");
			return entity;
		}

		// 頂点バッファをロック
		Vertex2D* pVtx;
		hr = polygon2d.vtxBuf->Lock(0U, 0U, reinterpret_cast<void**>(&pVtx), 0U);
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

		for (int i = 0; i < 4; i++)
		{
			// rhw の設定
			pVtx[i].rhw = 1.0f;

			// 頂点カラーの設定
			pVtx[i].col = polygon2d.col;
		}

		// 頂点バッファをアンロック
		hr = polygon2d.vtxBuf->Unlock();
		assert(SUCCEEDED(hr));
	}

	return entity;
}
