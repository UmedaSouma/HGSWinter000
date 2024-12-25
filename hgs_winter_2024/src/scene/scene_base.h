/**********************************************************//**
 * @file scene_base.h
 * @brief シーンの基底クラス
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

/** @brief シーンの基底クラス */
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	/** @brief コンストラクタ */
	SceneBase();

	/** @brief デストラクタ */
	virtual ~SceneBase() = default;

	/**
	 * @brief 初期設定
	 * @return 結果
	 */
	virtual HRESULT Init();

	/** @brief 終了処理 */
	virtual void Uninit();

	/** @brief 更新処理 */
	virtual void Update(float deltaTime);

	/** @brief 描画処理 */
	virtual void Draw() const;

	/**
	 * @brief シーンの切替
	 * @return 次のシーン
	 */
	virtual std::shared_ptr<SceneBase> ChangeScene();

	/**
	 * @brief ダウンキャスト
	 * @tparam T ダウンキャストする型
	 * @return ダウンキャストした型
	 */
	template <typename T> std::shared_ptr<T> Downcast()
	{
		static_assert(std::is_base_of<SceneBase, T>::value, "T は SceneBase の派生クラスであること！");

		return std::dynamic_pointer_cast<T>(shared_from_this());
	}

	/**
	 * @brief アップキャスト
	 * @tparam T アップキャストする型
	 * @return アップキャストした型
	 */
	template <typename T> std::shared_ptr<T> Upcast()
	{
		static_assert(std::is_base_of<SceneBase, T>::value, "T は SceneBase の基底クラスであること！");

		return std::static_pointer_cast<T>(shared_from_this());
	}

protected:
	/**
	 * @brief 2D ポリゴンの作成
	 * @param [in] pos 位置
	 * @param [in] rot 回転
	 * @param [in] size サイズ
	 * @param [in] col 色
	 * @param [in] numU X 方向の分割数
	 * @param [in] numV Y 方向の分割数
	 * @param [in] interval アニメーション間隔
	 * @param [in] playerIdx プレイヤーのインデックス
	 * @param [in] stage ステージ
	 * @param [in] panel パネル
	 * @param [in] isHouse おうちかどうか
	 * @return エンティティ
	 */
	entt::entity CreatePolygon2D(
		const std::string& key,
		const Vec3& pos,
		const Vec3& rot,
		const Vec3& size,
		const Color& col,
		UINT numU,
		UINT numV,
		UINT interval,
		int playerIdx = -1,
		int stage = -1,
		int panel = -1,
		bool isHouse = false,
		bool isPedestrian = false);

protected:
	// TODO: private に変更
	entt::registry m_registry; //!< エンティティのレジストリ
	mutable std::map<std::string, ComPtr<IDirect3DTexture9>> m_textures; //!< テクスチャ

private:
	/** @brief デフォルトのコピーコンストラクタを削除 */
	SceneBase(const SceneBase&) = delete;

	/** @brief デフォルトのコピー代入演算子を削除 */
	void operator=(const SceneBase&) = delete;

	/**
	 * @brief エンティティの更新
	 * @param [in] deltaTime 前フレームからの経過時間
	 */
	virtual void UpdateSystem(float deltaTime) = 0;

	/** @brief エンティティの描画 */
	virtual void DrawSystem() const = 0;

};
