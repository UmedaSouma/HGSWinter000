/**********************************************************//**
 * @file light.h
 * @brief ライト
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

 /** @brief ライトのクラス */
class Light
{
public:
	/** @brief コンストラクタ */
	Light();

	/** @brief デストラクタ */
	~Light() = default;

	/**
	 * @brief 初期設定
	 * @return 結果
	 */
	HRESULT Init();

	/** @brief 終了処理 */
	void Uninit();

	/**
	 * @brief インデックスの設定
	 * @param [in] idx インデックス
	 */
	void SetIndx(UINT idx) { m_idx = idx; }

	/**
	 * @brief インデックスの取得
	 * @return インデックス
	 */
	const uint32_t& GetIdx() const { return m_idx; }

private:
	/** @brief デフォルトのコピーコンストラクタを削除 */
	Light(const Light&) = delete;

	/** @brief デフォルトのコピー代入演算子を削除 */
	void operator=(const Light&) = delete;

private:
	D3DLIGHT9 m_light; //!< ライト
	uint32_t m_idx; // インデックス
};
