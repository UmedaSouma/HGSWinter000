//--------------------------------------------------------------------------------
// 
// ライト [light.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
// // elements
#include "scene/elements/light.h"
// game_manager
#include "game_manager/game_manager.h"

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
Light::Light()
	: m_light()
	, m_idx(0)
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT Light::Init()
{
	auto device = GM.GetDevice();

	// 初期パラメータの設定
	{
		Vec3 dir = inParam[LIGHT_PARAM_DIR];
		Color diffuse = inParam[LIGHT_PARAM_DIFFUSE];
		uint32_t idx = inParam[LIGHT_PARAM_IDX];

		m_light.Type = D3DLIGHT_DIRECTIONAL;
		m_light.Direction = *D3DXVec3Normalize(&dir, &dir);
		m_light.Diffuse = diffuse;
		m_idx = idx;

		m_light.Range = 1000;
	}

	// デバイスに設定
	device->SetLight(m_idx, &m_light);
	device->LightEnable(m_idx, TRUE);

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void Light::Uninit()
{
	/* DO_NOTHING */
}
