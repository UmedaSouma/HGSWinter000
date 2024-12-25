 /**********************************************************//**
  * @file components.h
  * @brief コンポーネント
  *
  * @author Taito Jin
  * @date since 2024/12/25
  **************************************************************/
#pragma once

/* インクルード */
#include "jing/jing_config.h"

/** @brief 2D ポリゴン */
struct Polygon2DComp
{
	ComPtr<IDirect3DVertexBuffer9> vtxBuf = nullptr; //!< 頂点バッファへのポインタ
	Vec3 pos = { 0.0f, 0.0f, 0.0f }; //!< 位置
	Vec3 rot = { 0.0f, 0.0f, 0.0f }; //!< 向き
	Vec3 size = { 0.0f, 0.0f, 0.0f }; //!< サイズ
	Color col = { 1.0f, 1.0f, 1.0f, 1.0f }; //!< 色
	std::string key; //!< 使用するテクスチャのキー
	UINT numU = 1; //!< X 方向の分割数
	UINT numV = 1; //!< Y 方向の分割数
	UINT nowNumU = 0; //!< 現在の X 方向の分割数
	UINT nowNumV = 0; //!< 現在の Y 方向の分割数
	UINT interval = 0; //!< アニメーション間隔
	UINT frame = 0; //!< 現在のフレーム
};

/** @brief テクスチャアニメーション */
struct TextureAnimComp
{
};


