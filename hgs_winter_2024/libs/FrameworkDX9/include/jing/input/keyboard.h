/**********************************************************//**
 * @file keyboard.h
 * @brief キーボードの入力処理
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief キーボードの入力処理のクラス */
	class Keyboard
	{
	public:
		/** @brief コンストラクタ */
		Keyboard();

		/** @brief デストラクタ */
		~Keyboard();

		/**
		 * @brief 初期設定
		 * @param [in] dInput DirectInput オブジェクトへのポインタ
		 * @param [in] hWnd ウィンドウハンドル
		 * @return 結果
		 */
		HRESULT Init(ComPtr<IDirectInput8> dInput, HWND hWnd);

		/** @brief 終了処理 */
		void Uninit();

		/** @brief 更新処理 */
		void Update();

		/**
		 * @brief プレス状態の取得
		 * @param [in] key キー
		 * @return プレス状態
		 */
		bool GetPress(int key) const;

		/**
		 * @brief トリガー状態の取得
		 * @param [in] key キー
		 * @return トリガー状態
		 */
		bool GetTrigger(int key) const;

		/**
		 * @brief リリース状態の取得
		 * @param [in] key キー
		 * @return リリース状態
		 */
		bool GetRelease(int key) const;

		/**
		 * @brief リピート状態の取得
		 * @param [in] key キー
		 * @return リピート状態
		 */
		bool GetRepeat(int key) const;

	private:
		/** @brief 実装の前方宣言 */
		class Impl;

	private:
		/** @brief デフォルトのコピーコンストラクタを削除 */
		Keyboard(const Keyboard&) = delete;

		/** @brief デフォルトのコピー代入演算子を削除 */
		void operator=(const Keyboard&) = delete;

	private:
		std::unique_ptr<Impl> m_impl; //!< 実装

	};
}
