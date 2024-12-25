/**********************************************************//**
 * @file time_manager.h
 * @brief 時間のマネージャー
 *
 * @author Taito Jin
 * @date since 2024/12/18
 **************************************************************/
#pragma once

 /** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief 時間のマネージャーのクラス */
	class TimeManager
	{
	public:
		/** @brief コンストラクタ */
		TimeManager();

		/** @brief デストラクタ */
		~TimeManager() = default;

		/** @brief リセット */
		void Reset();

		/** @brief 更新 */
		void Update();

		/**
		 * @brief デルタタイムの取得
		 * @return デルタタイム
		 */
		float GetDeltaTime() const { return m_DeltaTime; }

		/**
		 * @brief 時間の取得
		 * @return 時間
		 */
		float GetTime() const { return m_Time; }

		/**
		 * @brief FPS の取得
		 * @return FPS
		 */
		float GetFPS() const { return m_FPS; }

		/**
		 * @brief フレームカウントの取得
		 * @return 60 FPS のうち 1 フレーム分が経過したか
		 */
		bool GetIsElapsed() const { return m_IsElapsed; }

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime; //!< 計測を開始した時間
		std::chrono::time_point<std::chrono::high_resolution_clock> m_LastFrameTime; //!< 最後に測定をした時間
		std::chrono::time_point<std::chrono::high_resolution_clock> m_FPSMeasurementLastTime; //!< FPS の計測を最後にした時間
		std::chrono::time_point<std::chrono::high_resolution_clock> m_FPSLimitLastTime; //!< FPS の制限の計測を最後にした時間
		float m_FPS; //!< FPS
		float m_DeltaTime; //!< デルタタイム
		float m_Time; //!< 時間
		int m_FrameCount; //!< フレームカウント
		bool m_IsElapsed; //!< 経過したか
	};
}
