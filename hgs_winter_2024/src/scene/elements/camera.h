/**********************************************************//**
 * @file camera.h
 * @brief カメラ
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

/** @brief カメラのクラス */
class Camera
{
public:
	/** @brief コンストラクタ */
	Camera();

	/** @brief デストラクタ */
	~Camera() = default;

	/**
	 * @brief 初期設定
	 * @return 結果
	 */
	HRESULT Init();

	/** @brief 終了処理 */
	void Uninit();

	/**
	 * @brief 更新処理
	 * @param [in] deltaTime 前フレームからの経過時間
	 */
	void Update(float deltaTime);

	/** @brief 描画処理 */
	void Draw() const;

	/**
	 * @brief カメラの設定処理
	 * @param [in] eye 視点
	 * @param [in] focus 注視点
	 */
	void SetPos(const Vec3& eye, const Vec3& focus);

	/**
	 * @brief 視点の取得
	 * @return 視点
	 */
	const Vec3& GetEye() const { return m_eye; }

	/**
	 * @brief 注視点の取得
	 * @return 注視点
	 */
	const Vec3& GetFocus() const { return m_focus; }

	/**
	 * @brief 向きの取得
	 * @return 向き
	 */
	Vec2 GetRot() const { return Vec2(m_rot.x, m_rot.y - M_PI_2_F); }

private:
	/**
	 * @brief カメラの向きの更新
	 * @param [in] deltaTime 前フレームからの経過時間
	 * @param [in] move 移動量
	 */
	void UpdateRot(float deltaTime, const Vec2& move);	// カメラの向きの更新

	/**
	 * @brief ホイールによるカメラの位置，向きの更新
	 * @param [in] deltaTime 前フレームからの経過時間
	 */
	void UpdatePosByWheel(float deltaTime);

	/** @brief カメラの回転制限 */
	void UpdateRotRestictions();

	/** @brief カメラの視点の更新 */
	void UpdateEye();

	/** @brief カメラの注視点の更新 */
	void UpdateFocus();

	/**
	 * @brief カメラの向きを求める
	 * @param [in] inDistance 距離
	 * @return 向き
	 */
	Vec2 FindRot(const Vec3& inDistance);

private:
	Vec3 m_eye; //!< 視点
	Vec3 m_focus; //!< 注視点
	Vec3 m_up; //!< 上方向ベクトル
	Vec3 m_move; //!< 移動量
	Vec2 m_rot; //!< 向き
	float m_dist; //!< 注視点と視点の距離
	mutable Mat4 m_mtxProjection; //!< プロジェクションマトリックス
	mutable Mat4 m_mtxView; //!< ビューマトリックス

};
