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
	ComPtr<IDirect3DVertexBuffer9> vtxBuf; //!< 頂点バッファへのポインタ
	Vec3 pos; //!< 位置
	Vec3 rot; //!< 向き
	Vec3 size; //!< サイズ
	Color col; //!< 色
	std::string key; //!< 使用するテクスチャのキー
};





