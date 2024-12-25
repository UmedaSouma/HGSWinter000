/**********************************************************//**
 * @file gamepad.h
 * @brief ゲームパッドの入力処理
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief ゲームパッドの入力処理のクラス */
	class Gamepad
	{
	public:
		static constexpr float IS_DIR = 0.1f; //!< 傾けている状態

	public:
		/** @brief ボタンの種類 */
		enum class ButtonType : WORD
		{
			Up = 0U, //!< 十字 (上)
			Down, //!< 十字 (下)
			Left, //!< 十字 (左)
			Right, //!< 十字 (右)
			Start, //!< スタート
			Back, //!< バック
			LeftThumb, //!< 左スティック押込み
			RightThumb, //!< 右スティック押込み
			LeftShoulder, //!< LB
			RightShoulder, //!< RB
			LeftTrigger, //!< LT トリガー
			RightTrigger, //!< RT トリガー
			A, //!< A
			B, //!< B
			X, //!< X
			Y, //!< Y
			Max
		};

		/** @brief トリガーボタンの種類 */
		enum class TriggerType : BYTE
		{
			Left = 0U, //!< 左トリガー
			Right, //!< 右トリガー
			Max
		};

		/** @brief スティックの種類 */
		enum class StickType : BYTE
		{
			Left = 0u, //!< 左スティック
			Right, //!< 右スティック
			Max
		};

		/** @brief スティックの方向 */
		enum class StickAngle : BYTE
		{
			Up = 0U, //!< 上
			Down, //!< 下
			Left, //!< 左
			Right, //!< 右
			Max
		};

	public:
		/** @brief コンストラクタ */
		Gamepad();

		/** @brief デストラクタ */
		~Gamepad();

		/**
		 * @brief 初期設定
		 * @return 結果
		 */
		HRESULT Init();

		/** @brief 終了処理 */
		void Uninit();

		/** @brief 更新処理 */
		void Update();

		/**
		 * @brief 振動の設定処理
		 * @param [in] time 振動持続時間
		 * @param [in] strength 振動の強さ (0 ~ 65535)
		 */
		void SetVibration(int time, WORD strength);

		/**
		 * @brief ボタンのプレス情報の取得
		 * @param [in] type ボタンの種類
		 * @return ボタンの入力情報
		 */
		bool GetButtonPress(ButtonType type) const;

		/**
		 * @brief ボタンのトリガー情報の取得
		 * @param [in] type ボタンの種類
		 * @return ボタンの入力情報
		 */
		bool GetButtonTrigger(ButtonType type) const;

		/**
		 * @brief ボタンのリピート情報の取得
		 * @param [in] type ボタンの種類
		 * @return ボタンの入力情報
		 */
		bool GetButtonRepeat(ButtonType type) const;

		/**
		 * @brief ボタンのリリース情報の取得
		 * @param [in] type ボタンの種類
		 * @return ボタンの入力情報
		 */
		bool GetButtonRelease(ButtonType type) const;

		/**
		 * @brief スティックの倒し具合の取得
		 * @param [in] type スティックの種類
		 * @return スティックの倒し具合
		 */
		float GetStickDir(StickType type) const;

		/**
		 * @brief スティックの角度の取得
		 * @param [in] type スティックの種類
		 * @return スティックの角度
		 */
		float GetStickAngle(StickType type) const;

		/**
		 * @brief スティックのプレス情報の取得
		 * @param [in] type スティックの種類
		 * @param [in] angle スティックの方向
		 * @return スティックの入力情報
		 */
		bool GetStickPress(StickType type, StickAngle angle) const;

		/**
		 * @brief スティックのトリガー情報の取得
		 * @param [in] type スティックの種類
		 * @param [in] angle スティックの方向
		 * @return スティックの入力情報
		 */
		bool GetStickTrigger(StickType type, StickAngle angle) const;

		/**
		 * @brief スティックのリリース情報の取得
		 * @param [in] type スティックの種類
		 * @param [in] angle スティックの方向
		 * @return スティックの入力情報
		 */
		bool GetStickRelease(StickType type, StickAngle angle) const;

		/**
		 * @brief スティックのリピート情報の取得
		 * @param [in] type スティックの種類
		 * @param [in] angle スティックの方向
		 * @return スティックの入力情報
		 */
		bool GetStickRepeat(StickType type, StickAngle angle) const;

		/**
		 * @brief トリガーボタンの押し具合の取得
		 * @param [in] type トリガーボタンの種類
		 * @return トリガーボタンの押し具合
		 */
		float GetTriggerPressure(TriggerType type) const;

		/**
		 * @brief トリガーボタンのプレス情報の取得
		 * @param [in] type トリガーボタンの種類
		 * @return トリガーボタンの入力情報
		 */
		bool GetStickPress(TriggerType type) const;

		/**
		 * @brief トリガーボタンのトリガー情報の取得
		 * @param [in] type トリガーボタンの種類
		 * @return トリガーボタンの入力情報
		 */
		bool GetStickTrigger(TriggerType type) const;

		/**
		 * @brief トリガーボタンのリリース情報の取得
		 * @param [in] type トリガーボタンの種類
		 * @return トリガーボタンの入力情報
		 */
		bool GetStickRelease(TriggerType type) const;

		/**
		 * @brief トリガーボタンのリピート情報の取得
		 * @param [in] type トリガーボタンの種類
		 * @return トリガーボタンの入力情報
		 */
		bool GetStickRepeat(TriggerType type) const;

		/**
		 * @brief スティックが傾いているかの判定
		 * @param [in] type スティックの種類
		 * @return スティックが傾いているか
		 */
		bool IsDir(StickType type) const;

	private:
		/** @brief 実装の前方宣言 */
		class Impl;

	private:
		/** @brief デフォルトのコピーコンストラクタを削除 */
		Gamepad(const Gamepad&) = delete;

		/** @brief デフォルトのコピー代入演算子を削除 */
		void operator=(const Gamepad&) = delete;

	private:
		std::unique_ptr<Impl> m_impl; //!< 実装

	};
}
