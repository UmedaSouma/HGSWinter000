﻿/**********************************************************//**
 * @file scene_result.h
 * @brief リザルト
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

 /* インクルード */
#include "scene/scene_base.h"

/** @brief リザルトのクラス */
class SceneResult : public SceneBase
{
public:
	/** @brief コンストラクタ */
	SceneResult();

	/** @brief デストラクタ */
	~SceneResult() override = default;

	/**
	 * @brief 初期設定
	 * @return 結果
	 */
	HRESULT Init() override;

	/** @brief 終了処理 */
	void Uninit() override;

	/** @brief 更新処理 */
	void Update(float deltaTime) override;

	/** @brief 描画処理 */
	void Draw() const override;

private:
	/** @brief デフォルトのコピーコンストラクタを削除 */
	SceneResult(const SceneResult&) = delete;

	/** @brief デフォルトのコピー代入演算子を削除 */
	void operator=(const SceneResult&) = delete;

	/**
	 * @brief エンティティの更新
	 * @param [in] deltaTime 前フレームからの経過時間
	 */
	void UpdateSystem(float deltaTime) override;

	/** @brief エンティティの描画 */
	void DrawSystem() const override;

	/**
	 * @brief 次のシーンの設定
	 * @return 次のシーン
	 */
	std::shared_ptr<SceneBase> ChangeScene() override;

};
