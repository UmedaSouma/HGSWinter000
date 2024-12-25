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

/** @brief キャラクター */
struct CharaComp {};

/** @brief プレイヤー */
struct PlayerComp {};

/** @brief 移動 */
struct MoveComp
{
	Vec3 move = { 0.0f, 0.0f, 0.0f }; //!< 移動量
};

/** @brief 一番上のステージ */
struct TopStageComp {};

/** @brief 中のステージ */
struct MiddleStageComp {};

/** @brief 一番下のステージ */
struct BottomStageComp {};

/** @brief 上方向パネル */
struct UpPanelComp {};

/** @brief 下方向パネル */
struct DownPanelComp {};

/** @brief 左方向パネル */
struct LeftPanelComp {};

/** @brief 右方向パネル */
struct RightPanelComp {};

/** @brief おうち */
struct House
{
	bool isShines = false; //!< 輝いているかどうか
	float shineTime = 0.0f; //!< 輝いている時間
	float coolTime = 0.0f; //!< クールタイム
};

/** @brief 通行人 */
struct PedestrianComp
{
	float moveTime = 0.0f; //!< 移動時間
	float coolTime = 0.0f; //!< クールタイム
	bool isMove = false; //!< 移動中かどうか
	Vec3 move = { 0.0f, 0.0f, 0.0f }; //!< 移動量
};

/** @brief GameOver */
struct GameOverComp
{

};

/** @brief 雪 */
struct Snow
{

};





