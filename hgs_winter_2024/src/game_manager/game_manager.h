/**********************************************************//**
 * @file game_manager.h
 * @brief ゲームマネージャー
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

/* インクルード */
#include "jing/renderer/renderer.h"
#include "scene/scene_base.h"

/* マクロ定義 */
#ifdef GM
#undef GM
#endif
#define GM (GameManager::GetInstance()) //!< ゲームマネージャーのインスタンス

/** @brief ゲームマネージャー */
class GameManager final : public Jing::SingletonBase<GameManager>
{
public:
	/**
	 * @brief 初期設定
	 * @param [in] hInstance インスタンスハンドル
	 * @param [in] hWnd ウィンドウハンドル
	 * @param [in] isWindowed ウィンドウモード
	 * @return 結果
	 */
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL isWindowed);

	/** @brief 終了処理 */
	void Uninit();

	/**
	 * @brief 更新処理
	 * @param in] deltaTime 前フレームからの経過時間
	 */
	void Update(float deltaTime);

	/** @brief 描画処理 */
	void Draw() const;

	/** @brief シーンの切替 */
	void ChangeScene();

	/**
	 * @brief ウィンドウハンドルを取得
	 * @return ウィンドウハンドル
	 */
	HWND GetWindowHandle() const { return m_hWnd; }

	/**
	 * @brief インスタンスハンドルを取得
	 * @return インスタンスハンドル
	 */
	HINSTANCE GetInstanceHandle() const { return m_hInstance; }

	/**
	 * @brief レンダラを取得
	 * @return レンダラ
	 */
	Jing::Renderer& GetRenderer() { return m_renderer; }

	/**
	 * @brief シーンを取得
	 * @return シーン
	 */
	std::shared_ptr<SceneBase> GetScene() const { return m_scene; }

	/**
	 * @brief デバイスを取得
	 * @return デバイス
	 */
	ComPtr<IDirect3DDevice9> GetDevice() const { return m_renderer.GetDevice(); }

private:
	/** @brief コンストラクタ */
	GameManager();

	/** @brief デストラクタ */
	~GameManager() = default;

	/** @brief デフォルトのコピーコンストラクタを削除 */
	GameManager(const GameManager&) = delete;

	/** @brief デフォルトのコピー代入演算子を削除 */
	void operator=(const GameManager&) = delete;

private:
	HWND m_hWnd; //!< ウィンドウハンドル
	HINSTANCE m_hInstance; //!< インスタンスハンドル
	Jing::Renderer m_renderer; //!< レンダラー
	std::shared_ptr<SceneBase> m_scene; //!< シーン

private:
	friend class SingletonBase<GameManager>;
};
