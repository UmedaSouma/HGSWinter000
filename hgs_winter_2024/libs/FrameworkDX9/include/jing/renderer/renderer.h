/**********************************************************//**
 * @file renderer.h
 * @brief レンダラ
 *
 * @author Taito Jin
 * @date since 2024/12/24
 **************************************************************/
#pragma once

 /* インクルード */
#include "jing/jing_config.h"

 /** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief レンダラ */
	class Renderer
	{
	public:
		/** @brief コンストラクタ */
		Renderer();

		/** @brief デストラクタ */
		~Renderer();

		/**
		 * @brief 初期設定
		 * @param [in] hWnd ウィンドウハンドル
		 * @param [in] isWindowed ウィンドウモード
		 */
		HRESULT Init(HWND hWnd, BOOL isWindowed);

		/** @brief 終了処理 */
		void Uninit();

		/**
		 * @brief 更新処理
		 * @param in] deltaTime 前フレームからの経過時間
		 */
		void Update(float deltaTime);

		/**
		 * @brief 描画処理
		 * @param [in] func 描画処理
		 */
		void Draw(const std::function<void()>& func) const;

		/**
		 * @brief デバイスを取得
		 * @return デバイス
		 */
		ComPtr<IDirect3DDevice9> GetDevice() const;

	private:
		/** @brief 実装の前方宣言 */
		class Impl;

	private:
		/** @brief デフォルトのコピーコンストラクタを削除 */
		Renderer(const Renderer&) = delete;

		/** @brief デフォルトのコピー代入演算子を削除 */
		void operator=(const Renderer&) = delete;

	private:
		std::unique_ptr<Impl> m_impl; //!< 実装

	};
}
