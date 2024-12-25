//--------------------------------------------------------------------------------
// 
// ゲームのマネージャー (管理者) [game_manager.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// game_manager
#include "game_manager/game_manager.h"
// scene
#include "scene/scene_title.h"
#include "scene/scene_tutorial.h"
#include "scene/scene_game.h"
#include "scene/scene_result.h"

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT GameManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL isWindowed)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;

	// ランダムの種を設定する
	srand(static_cast<UINT>(time(nullptr)));

	// レンダラーの初期設定
	HRESULT hr = m_renderer.Init(m_hWnd, isWindowed);
	if (FAILED(hr))
	{
		assert(false && "レンダラーの初期設定に失敗！");
		return E_FAIL;
	}

	// シーン
	{
		// シーンの生成
		m_scene = std::make_unique<SceneTitle>();
		if (!m_scene)
		{
			assert(false && "シーンの生成に失敗！");
			return E_FAIL;
		}

		// シーンの初期設定
		hr = m_scene->Init();
		if (FAILED(hr))
		{
			assert(false && "シーンの初期設定に失敗！");
			return E_FAIL;
		}
	}

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void GameManager::Uninit()
{
	// シーンの終了処理
	if (m_scene)
	{
		m_scene->Uninit();
	}

	// レンダラーの終了処理
	m_renderer.Uninit();
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void GameManager::Update(float deltaTime)
{
	// レンダラーの更新
	m_renderer.Update(deltaTime);

	// シーンの更新
	m_scene->Update(deltaTime);

	// Debug
	{
		ImGui::Begin("Debug");

		ImGui::Text("test");

		ImGui::End();
	}
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void GameManager::Draw() const
{
	// シーンの描画
	m_renderer.Draw([this]() { m_scene->Draw(); });
}

//---------------------------------------------------
// シーンの変更
//---------------------------------------------------
void GameManager::ChangeScene()
{
	// シーンの変更
	m_scene = m_scene->ChangeScene();

	// シーンの初期設定
	HRESULT hr = m_scene->Init();
	if (FAILED(hr))
	{
		assert(false && "シーンの初期設定に失敗。");
		return;
	}
}

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
GameManager::GameManager()
	: m_hWnd(nullptr)
	, m_hInstance(nullptr)
{
	/* DO_NOTHING */
}
