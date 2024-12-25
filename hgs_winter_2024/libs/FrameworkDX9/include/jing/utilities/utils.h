/**********************************************************//**
 * @file utils.h
 * @brief ユーティリティ
 *
 * @author Taito Jin
 * @date since 2024/12/25
 **************************************************************/
#pragma once

/* インクルード */
#include "jing/jing_config.h"

/** @brief Jing 名前空間 */
namespace Jing
{
	/** @brief ユーティリティの名前空間 */
	namespace Utils
	{
		/**
		 * @brief クォータニオンからオイラー角を計算する (回転順 YXZ)
		 * @param [in] rot クォータニオン
		 * @return オイラー角
		 */
		Vec3 QuatToEulerYXZ(const Quat& rot);

		/**
		 * @brief 向きの計算
		 * @param [in] axis 軸
		 * @return クォータニオン
		 */
		Quat CalcRot(const Vec3& axis, float radian);

		/**
		 * @brief オイラー角からクォータニオンを計算する
		 * @param [in] inRadian オイラー角
		 * @return クォータニオン
		 */
		Quat CalcuQuatByEularYawPitchRoll(const Vec3& radian);

		/**
		 * @brief 向きを掛ける
		 * @param [in] axis 軸
		 * @param [in] radian 角度
		 * @return クォータニオン
		 */
		Quat MulRot(const Vec3& axis, float radian);
	}
}
