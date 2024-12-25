/**********************************************************//**
 * @file input_manager.h
 * @brief 入力処理のマネージャー
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/* インクルード */
#include "jing/input/keyboard.h"
#include "jing/input/mouse.h"
#include "jing/input/gamepad.h"

/** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief 入力処理のマネージャークラス */
	class InputManager
	{
	public:
		/** @brief デストラクタ */
		~InputManager();

		/**
		 * @brief 初期設定
		 * @param [in] hInstance インスタンスハンドル
		 * @param [in] hWnd ウィンドウハンドル
		 * @return 結果
		 */
		HRESULT Init(HINSTANCE hInstance, HWND hWnd);

		/** @brief 終了処理 */
		void Uninit();

		/** @brief 更新処理 */
		void Update();

		/**
		 * @brief キーボードの取得
		 * @return キーボード
		 */
		const Keyboard& GetKeyboard() const;

		/**
		 * @brief マウスの取得
		 * @return マウス
		 */
		const Mouse& GetMouse() const;

		/**
		 * @brief ゲームパッドの取得
		 * @return ゲームパッド
		 */
		const Gamepad& GetGamepad() const;

		/**
		 * @brief インスタンスの取得
		 * @return インスタンス
		 */
		static InputManager& GetInstance();

	private:
		/** @brief 実装の前方宣言 */
		class Impl;

	private:
		/** @brief コンストラクタ */
		InputManager();

		/** @brief デフォルトのコピーコンストラクタを削除 */
		InputManager(const InputManager&) = delete;

		/** @brief デフォルトのコピー代入演算子を削除 */
		void operator=(const InputManager&) = delete;

	private:
		std::unique_ptr<Impl> m_impl; //!< 実装

	};
}
