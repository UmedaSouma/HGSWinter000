/**********************************************************//**
 * @file jing_config.h
 * @brief 基本情報まとめ
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/* using */
#pragma region Using

// JSON
#if 1
using json = nlohmann::json;
#else
using json = nlohmann::ordered_json;
#endif
using json_validator = nlohmann::json_schema::json_validator;

// DX9
using Vec2 = D3DXVECTOR2;
using Vec3 = D3DXVECTOR3;
using Vec4 = D3DXVECTOR4;
using Mat4 = D3DXMATRIX;
using Quat = D3DXQUATERNION;
using Color = D3DXCOLOR;
using ColVal = D3DCOLORVALUE;
using Device = LPDIRECT3DDEVICE9;
using Texture = LPDIRECT3DTEXTURE9;
using VertexBuffer = LPDIRECT3DVERTEXBUFFER9;
using IndexBuffer = LPDIRECT3DINDEXBUFFER9;
using Mesh = LPD3DXMESH;
using BufferPointer = LPD3DXBUFFER;
using MeshMaterial = D3DXMATERIAL;
using GraphicsMaterial = D3DMATERIAL9;
//using Light = D3DLIGHT9;
using Viewport = D3DVIEWPORT9;
using Surface = LPDIRECT3DSURFACE9;
using Plane = D3DXPLANE;
using Font = ID3DXFont;

// Microsoft::WRL
using Microsoft::WRL::ComPtr;

// USAGE: tchar[] text = TEXT("text");
#ifdef UNICODE
using tchar = wchar_t;
using tstring = std::wstring;
using tstringstream = std::wstringstream;
using tostringstream = std::wostringstream;
using tistringstream = std::wistringstream;
using tifstream = std::wifstream;
using tofstream = std::wofstream;
using tregex = std::wregex;
using tsmatch = std::wsmatch;
using tstring_view = std::wstring_view;
using tregex_iterator = std::wsregex_iterator;
using tregex_token_iterator = std::wsregex_token_iterator;
#else
using tchar = char;
using tstring = std::string;
using tstringstream = std::stringstream;
using tostringstream = std::ostringstream;
using tistringstream = std::istringstream;
using tifstream = std::ifstream;
using tofstream = std::ofstream;
using tregex = std::regex;
using tsmatch = std::smatch;
using tstring_view = std::string_view;
using tregex_iterator = std::sregex_iterator;
using tregex_token_iterator = std::sregex_token_iterator;
#endif // UNICODE

#pragma endregion

/* マクロ定義 */
#pragma region Defines

// メモリリーク検出
#ifdef _DEBUG
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif // _DEBUG

// corect_math_defines.h => float ver
#define M_E_F (2.7182818f) //!< e
#define M_LOG2E_F (1.4426950f) //!< log2(e)
#define M_LOG10E_F (0.4342945f) //!< log10(e)
#define M_LN2_F (0.6931472f) //!< ln(2)
#define M_LN10_F (2.3025851f) //!< ln(10)
#define M_PI_F (3.1415927f) //!< pi
#define M_PI_2_F (1.5707963f) //!< pi/2
#define M_PI_4_F (0.7853982f) //!< pi/4
#define M_1_PI_F (0.3183099f) //!< 1/pi
#define M_2_PI_F (0.6366198f) //!< 2/pi
#define M_2_SQRTPI_F (1.1283792f) //!< 2/sqrt(pi)
#define M_SQRT2_F (1.4142135f) //!< sqrt(2)
#define M_SQRT1_2_F (0.7071068f) //!< 1/sqrt(2)

#pragma endregion

/* 定数 */
#pragma region Constants

// デフォルト値
const D3DXVECTOR2 DEF_VEC2 = D3DXVECTOR2(0.0f, 0.0f); //!< デフォルトの 2D ベクトル
const D3DXVECTOR3 DEF_VEC3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //!< デフォルトの 3D ベクトル
const D3DXQUATERNION DEF_ROT = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f); //!< デフォルトのクォータニオン
const D3DXCOLOR DEF_COL = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); //!< デフォルトのカラー
const D3DXMATRIX DEF_MAT4 = D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); //!< デフォルトの行列
const D3DXVECTOR3 DEF_SCALE = D3DXVECTOR3(1.0f, 1.0f, 1.0f); //!< デフォルトのスケール
const D3DMATERIAL9 DEF_MATERIAL = { { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, 5.0f }; //!< デフォルトのマテリアル
const btTransform DEF_TRANSFORM = btTransform(btQuaternion(DEF_ROT.x, DEF_ROT.y, DEF_ROT.z, DEF_ROT.w), btVector3(DEF_VEC3.x, DEF_VEC3.y, DEF_VEC3.z)); //!< デフォルトの変換行列

constexpr float HALF = 0.5f; //!< 半分
constexpr float QUARTER = 0.25f; //!< 1/4
constexpr float EPSILON = 1.175494015e-37f; //!< エプシロン

constexpr UINT SCREEN_WIDTH = 1280; //!< ウィンドウの幅
constexpr UINT SCREEN_HEIGHT = 720; //!< ウィンドウの高さ

constexpr const char* NONE = "NONE"; //!< なし
constexpr const tchar* WINDOW_CLASS_NAME = TEXT("HGS Winter 2024"); //!< ウィンドウクラス名
constexpr const tchar* WINDOW_NAME = TEXT("HGS Winter 2024"); //!< ウィンドウ名

constexpr DWORD FVF_VERTEX_2D = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1; //!< 頂点フォーマット [2D]
constexpr DWORD FVF_VERTEX_3D = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1; //!< 頂点フォーマット [3D]

#pragma endregion

/* 構造体 */
#pragma region Structs

/** @brief 頂点情報 [2D] の構造体 */
struct Vertex2D
{
	D3DXVECTOR3 pos; //!< 頂点座標
	float rhw; //!< 座標変換用係数 (1.0f で固定)
	D3DCOLOR col; //!< 頂点カラー
	D3DXVECTOR2 tex; //!< テクスチャ座標
};

/** @brief 頂点情報 [3D] の構造体 */
struct Vertex3D
{
	D3DXVECTOR3 pos; //!< 頂点座標
	D3DXVECTOR3 nor; //!< 法線ベクトル
	D3DCOLOR col; //!< 頂点カラー
	D3DXVECTOR2 tex; //!< テクスチャ座標
};

#pragma endregion
