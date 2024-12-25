/**********************************************************//**
 * @file mouse.h
 * @brief マウスの入力処理
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief マウスの入力処理のクラス */
	class Mouse
	{
	public:
		/** @brief マウスのボタンの種類 */
		enum class ButtonType : UINT
		{
			Left = 0U, //!< 左クリック
			Right, //!< 右クリック
			Middle, //!< 中央 (ホイール)
			Max
		};

	public:
		/** @brief コンストラクタ */
		Mouse();

		/** @brief デストラクタ */
		~Mouse();

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
		 * @brief カーソルの位置の取得
		 * @return カーソルの位置
		 */
		const Vec2& GetPos() const;

		/**
		 * @brief 移動量の取得
		 * @return 移動量
		 */
		const Vec2& GetMove() const;

		/**
		 * @brief ホイールの回転量の取得
		 * @return ホイールの回転量
		 */
		float GetWheelRotVolume() const;

		/**
		 * @brief ボタンの押下状態の取得
		 * @param [in] type ボタンの種類
		 * @return 押下状態
		 */
		bool GetPress(ButtonType type) const;

		/**
		 * @brief ボタンのトリガー状態の取得
		 * @param [in] type ボタンの種類
		 * @return トリガー状態
		 */
		bool GetTrigger(ButtonType type) const;

		/**
		 * @brief ボタンのリリース状態の取得
		 * @param [in] type ボタンの種類
		 * @return リリース状態
		 */
		bool GetRelease(ButtonType type) const;

	private:
		/** @brief 実装の前方宣言 */
		class Impl;

	private:
		/** @brief デフォルトのコピーコンストラクタを削除 */
		Mouse(const Mouse&) = delete;

		/** @brief デフォルトのコピー代入演算子を削除 */
		void operator=(const Mouse&) = delete;

	private:
		std::unique_ptr<Impl> m_impl; //!< 実装

	};
}
