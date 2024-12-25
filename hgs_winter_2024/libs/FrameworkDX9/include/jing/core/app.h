/**********************************************************//**
 * @file app.h
 * @brief アプリケーション
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

 /* インクルード */
#include "jing/jing_config.h"
#include "jing/system/time_manager.h"

/** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief アプリケーションのクラス */
	class App
	{
	public:
		/**
		 * @brief コンストラクタ
		 * @param [in] width ウィンドウの幅
		 * @param [in] height ウィンドウの高さ
		 */
		App(UINT width, UINT height);

		/** @brief デストラクタ */
		~App() = default;

		/**
		 * @brief アプリケーションの実行
		 * @param [in] hInstance インスタンスハンドル
		 * @param [in] nCmdShow 表示状態
		 */
		void Run(HINSTANCE hInstance, int nCmdShow);

		/**
		 * @brief ウィンドウの幅を取得
		 * @return ウィンドウの幅
		 */
		UINT GetWidth() const { return m_width; }

		/**
		 * @brief ウィンドウの高さを取得
		 * @return ウィンドウの高さ
		 */
		UINT GetHeight() const { return m_height; }

		/**
		 * @brief ウィンドウハンドルを取得
		 * @return ウィンドウハンドル
		 */
		HWND GetHWnd() const { return m_hWnd; }

		/**
		 * @brief インスタンスハンドルを取得
		 * @return インスタンスハンドル
		 */
		HINSTANCE GetHInstance() const { return m_hInstance; }

		/**
		 * @brief 実行中かどうかを取得
		 * @return 実行中かどうか
		 */
		bool GetIsRunning() const { return m_isRunning; }

	protected:
		/**
		 * @brief フルスクリーン
		 * @param [in] hWnd ウィンドウハンドル
		 */
		void ToggleFullscreen(HWND hWnd);

	private:
		/** @brief メインループ */
		void MainLoop();

		/**
		 * @brief アプリの初期設定
		 * @param [in] hInstance インスタンスハンドル
		 * @param [in] nCmdShow 表示状態
		 * @return 結果
		 */
		HRESULT InitApp(HINSTANCE hInstance, int nCmdShow);

		/** @brief アプリの終了処理 */
		void UninitApp();

		/**
		 * @brief アプリの更新処理
		 * @param [in] deltaTime 前フレームからの経過時間
		 */
		void UpdateApp(float deltaTime);

		/** @brief アプリの描画処理 */
		void DrawApp();

		/**
		 * @brief ウィンドウの初期設定
		 * @param [in] hInstance インスタンスハンドル
		 * @param [in] nCmdShow 表示状態
		 * @return 結果
		 */
		HRESULT InitWnd(HINSTANCE hInstance);

		/** @brief ウィンドウの終了処理 */
		void UninitWnd();

		/**
		 * @brief 初期設定
		 * @return 結果
		 */
		virtual HRESULT OnInit() = 0;

		/** @brief 終了処理 */
		virtual void OnUninit() = 0;

		/**
		 * @brief 更新処理
		 * @param [in] deltaTime 前フレームからの経過時間
		 */
		virtual void OnUpdate(float deltaTime) = 0;

		/** @brief 描画処理 */
		virtual void OnDraw() = 0;

		/**
		 * @brief メッセージ処理
		 * @param [in] hWnd ウィンドウハンドル
		 * @param [in] msg メッセージ
		 * @param [in] wParam パラメータ
		 * @param [in] lParam パラメータ
		 */
		virtual void OnMsgProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP) = 0;

		/**
		 * @brief ウィンドウプロシージャ
		 * @param [in] hWnd ウィンドウハンドル
		 * @param [in] msg メッセージ
		 * @param [in] wP パラメータ
		 * @param [in] lP パラメータ
		 * @return 結果
		 */
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP);

	private:
		UINT m_width; //!< ウィンドウの幅
		UINT m_height; //!< ウィンドウの高さ
		bool m_isRunning; //!< 実行中かどうか
		bool m_isFullScreen; //!< フルスクリーンかどうか
		HWND m_hWnd; //!< ウィンドウハンドル
		HINSTANCE m_hInstance; //!< インスタンスハンドル
		TimeManager m_timeManager; //!< 時間のマネージャー
	};
}
