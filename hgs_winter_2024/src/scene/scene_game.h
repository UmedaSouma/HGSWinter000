/**********************************************************//**
 * @file scene_game.h
 * @brief ゲーム
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

 /* インクルード */
#include "scene/scene_base.h"

/** @brief ゲームのクラス */
class SceneGame : public SceneBase
{
public:
	/** @brief コンストラクタ */
	SceneGame();

	/** @brief デストラクタ */
	~SceneGame() override = default;

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
	SceneGame(const SceneGame&) = delete;

	/** @brief デフォルトのコピー代入演算子を削除 */
	void operator=(const SceneGame&) = delete;

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

private:
	entt::entity m_topPlayer; //!< 一番上のプレイヤー
	entt::entity m_middlePlayer; //!< 中のプレイヤー
	entt::entity m_bottomPlayer; //!< 一番下のプレイヤー

	bool m_doDownMiddle; //!< 中のプレイヤーが下に移動するかどうか
	bool m_doUpMiddle; //!< 中のプレイヤーが上に移動するかどうか
	bool m_doLeftMiddle; //!< 中のプレイヤーが左に移動するかどうか
	bool m_doRightMiddle; //!< 中のプレイヤーが右に移動するかどうか

	bool m_doDownTop; //!< 一番上のプレイヤーが下に移動するかどうか
	bool m_doUpTop; //!< 一番上のプレイヤーが上に移動するかどうか
	bool m_doLeftTop; //!< 一番上のプレイヤーが左に移動するかどうか
	bool m_doRightTop; //!< 一番上のプレイヤーが右に移動するかどうか

	float m_time; //!< 時間

	bool m_topGameOver; //!< 一番上のプレイヤーがゲームオーバーかどうか
	bool m_middleGameOver; //!< 中のプレイヤーがゲームオーバーかどうか
	bool m_bottomGameOver; //!< 一番下のプレイヤーがゲームオーバーかどうか

};
