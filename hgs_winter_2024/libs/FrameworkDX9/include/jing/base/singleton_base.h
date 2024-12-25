/**********************************************************//**
 * @file singleton_base.h
 * @brief シングルトンの基底クラス
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

// USAGE:
// - 継承先は final を付けること
// - 派生クラスのコンストラクタを private にすること
// - インスタンス生成時に基底クラスが派生クラスの private メンバのコンストラクタにアクセスするので friend を付けること

// class MySingleton final : public SingletonBase<MySingleton>
// {
// private:
//     MySingleton() = default;
//     ~MySingleton() = default;
//     MySingleton(const MySingleton&) = delete;
//     void operator=(const MySingleton&) = delete;
// 
//     friend class SingletonBase<MySingleton>;
// };

/** @brief Jing 名前空間 */
namespace Jing
{
	/**
	 * @brief シングルトンの基底クラス
	 * @tparam T シングルトンの型
	 */
	template <typename T>
	class SingletonBase
	{
	public:
		/**
		 * @brief インスタンスの取得
		 * @return インスタンス
		 */
		static T& GetInstance()
		{
			static T instance;
			return instance;
		}

	protected:
		/** @brief コンストラクタ */
		SingletonBase() = default;

		/** @brief デストラクタ */
		virtual ~SingletonBase() = default;

	private:
		/** @brief デフォルトのコピーコンストラクタを削除 */
		SingletonBase(const SingletonBase&) = delete;

		/** @brief デフォルトのコピー代入演算子を削除 */
		void operator=(const SingletonBase&) = delete;

	};
}
