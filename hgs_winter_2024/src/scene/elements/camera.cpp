//--------------------------------------------------------------------------------
// 
// カメラ [camera.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
// // elements
#include "scene/elements/camera.h"
// scene
#include "scene/scene_game.h"
// game_manager
#include "game_manager/game_manager.h"
// input
#include "jing/input/input_manager.h"

//===================================================
// 無名名前空間
//===================================================
namespace /* anonymous */
{
	//---------------------------------------------------
	// 定数
	//---------------------------------------------------

	constexpr float VIEWINGANGLE_CAMERA = 45.0f;			// 視野角 (degree)
	constexpr float CORRECTION_CAMERROT = 0.003f;			// 補正
	constexpr float MINOR_ADJUST_ROT = 0.0001f;				// 回転が上限を突破しないための調整
	constexpr float MIN_POSDISTANSE_CAMERA = 50.0f;			// 最短距離
	constexpr float MAX_POSDISTANSE_CAMERA = 1000.0f;		// 最長距離
	constexpr float NEAR_RANGE_CAMERA = 10.0f;				// Z 値の最小値
	constexpr float FAR_RANGE_CAMERA = 15000.0f;			// Z 値の最大値

} // namespace /* anonymous */

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
Camera::Camera()
	: m_eye(DEF_VEC3)
	, m_focus(DEF_VEC3)
	, m_up(DEF_VEC3)
	, m_move(DEF_VEC3)
	, m_rot(DEF_VEC2)
	, m_mtxProjection(DEF_MAT4)
	, m_mtxView(DEF_MAT4)
	, m_dist(0.0f)
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT Camera::Init()
{
	// ビューポートの設定
	{
		Viewport viewport;
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = SCREEN_WIDTH;
		viewport.Height = SCREEN_HEIGHT;
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		GM.GetDevice()->SetViewport(&viewport);
	}

	// 初期パラメータの設定
	{
		// 位置設定
		SetPos(Vec3(0.0f, 10.0f, -10.0f), Vec3(0.0f, 0.0f, 0.0f));

		// 上方向ベクトル
		m_up = Vec3(0.0f, 1.0f, 0.0f);
	}

	// カメラの設定
	Draw();

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void Camera::Uninit()
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void Camera::Update(float deltaTime)
{
	auto& inputManager = Jing::InputManager::GetInstance(); // 入力処理の管理者
	auto& keyboard = inputManager.GetKeyboard(); // キーボードの取得
	auto& mouse = inputManager.GetMouse(); // マウスの取得
	Vec2 moveMouse = mouse.GetMove(); // マウスの移動量

	// 各ボタンのプレス情報
	bool isPressLeft = mouse.GetPress(Jing::Mouse::ButtonType::Left);
	bool isPressRight = mouse.GetPress(Jing::Mouse::ButtonType::Right);
	bool isPressMiddle = mouse.GetPress(Jing::Mouse::ButtonType::Middle);

	if (isPressLeft && !isPressRight)
	{ // 左クリック
		// 回転の更新処理
		UpdateRot(deltaTime, moveMouse);
	}
	if (!isPressLeft && isPressRight)
	{ // 右クリック
		// 回転の更新処理
		UpdateRot(deltaTime, moveMouse);

		// 注視点の更新
		UpdateFocus();
	}
	else if (isPressLeft && isPressRight)
	{ // 左右クリック

		// 視点の更新
		{
			float sin = sinf(m_rot.y);
			float cos = cosf(m_rot.y);

			m_eye.x += (-(sin * moveMouse.x - cos * moveMouse.y) * deltaTime);
			m_eye.z += ((sin * moveMouse.y + cos * moveMouse.x) * deltaTime);
		}

		// 注視点の更新
		UpdateFocus();
	}
	else if (isPressMiddle)
	{ // ホイールクリック，かつ追尾中でない
		m_eye.y += (moveMouse.y * deltaTime);

		// 注視点の更新
		UpdateFocus();
	}

	// カメラの回転制限
	UpdateRotRestictions();

	// ホイールによるカメラの位置，向きの更新
	UpdatePosByWheel(deltaTime);

	// 視点の更新
	UpdateEye();
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void Camera::Draw() const
{
	// デバイスの取得
	auto device = GM.GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&m_mtxProjection,
		D3DXToRadian(VIEWINGANGLE_CAMERA), // 視野角
		static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), // 画面のアスペクト比
		NEAR_RANGE_CAMERA, // Z 値の最小値
		FAR_RANGE_CAMERA); // Z 値の最大値

	// プロジェクションマトリックスの設定
	device->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	m_mtxView = DEF_MAT4;

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_eye, &m_focus, &m_up);

	// ビューマトリックスの設定
	device->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//---------------------------------------------------
// カメラの位置の設定
//---------------------------------------------------
void Camera::SetPos(const Vec3& eye, const Vec3& focus)
{
	m_eye = eye;
	m_focus = focus;

	Vec3 distance = m_focus - m_eye;
	m_dist = D3DXVec3Length(&distance);

	m_rot = FindRot(distance);
}

//---------------------------------------------------
// カメラの向きの更新
//---------------------------------------------------
void Camera::UpdateRot(float deltaTime, const Vec2& move)
{
	m_rot.x -= (move.y * CORRECTION_CAMERROT * deltaTime);
	m_rot.y -= (move.x * CORRECTION_CAMERROT * deltaTime);
}

//---------------------------------------------------
// ホイールによるカメラの位置，向きの更新
//---------------------------------------------------
void Camera::UpdatePosByWheel(float deltaTime)
{
	float wheelRotVol = Jing::InputManager::GetInstance().GetMouse().GetWheelRotVolume(); // ホイールの回転量

	m_dist += (wheelRotVol * deltaTime); // 拡大・縮小の更新

	// 注視点と視点の距離の制限
	m_dist = std::clamp(m_dist, MIN_POSDISTANSE_CAMERA, MAX_POSDISTANSE_CAMERA);
}

//---------------------------------------------------
// カメラの回転制限
//---------------------------------------------------
void Camera::UpdateRotRestictions()
{
	// カメラの向きの補正
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= (D3DX_PI * 2.0f);
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += (D3DX_PI * 2.0f);
	}

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= (D3DX_PI * 2.0f);
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += (D3DX_PI * 2.0f);
	}

	// 回転の制限
	float halfAngle = M_PI_2_F; // 90°
	if (m_rot.x >= halfAngle - MINOR_ADJUST_ROT)
	{
		m_rot.x = halfAngle - MINOR_ADJUST_ROT;
	}
	else if (m_rot.x <= -halfAngle + MINOR_ADJUST_ROT)
	{
		m_rot.x = -halfAngle + MINOR_ADJUST_ROT;
	}
}

//---------------------------------------------------
// カメラの視点の更新
//---------------------------------------------------
void Camera::UpdateEye()
{
	m_eye.x = cosf(m_rot.x) * cosf(m_rot.y);
	m_eye.y = sinf(m_rot.x);
	m_eye.z = cosf(m_rot.x) * sinf(m_rot.y);

	m_eye *= -m_dist;
	m_eye += m_focus;
}

//---------------------------------------------------
// カメラの注視点の更新
//---------------------------------------------------
void Camera::UpdateFocus()
{
	m_focus.x = cosf(m_rot.x) * cosf(m_rot.y);
	m_focus.y = sinf(m_rot.x);
	m_focus.z = cosf(m_rot.x) * sinf(m_rot.y);

	m_focus *= m_dist;
	m_focus += m_eye;
}

//---------------------------------------------------
// カメラの向きを求める
//---------------------------------------------------
Vec2 Camera::FindRot(const Vec3& dist)
{
	// 符号を返す
	auto sign = [](float value) -> float { return (value >= 0) ? 1.0f : -1.0f; };

	// 向きを求める
	Vec2 rot = DEF_VEC2;
	{
		// 方位角
		rot.y = M_PI_2_F - atan2f(dist.x, dist.z);

		// 仰角
		float hx = -(sinf(rot.y) * M_PI_2_F + atan2f(dist.z, dist.y)) * sign(-dist.z);
		float hz = -(cosf(rot.y) * M_PI_2_F + atan2f(dist.x, dist.y)) * sign(-dist.x);
		rot.x = -(hx + hz);
	}

	return rot;
}
