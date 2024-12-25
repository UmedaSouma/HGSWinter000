//--------------------------------------------------------------------------------
// 
// ゲームのシーン [scene_game.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
#include "scene/scene_game.h"
#include "scene/scene_result.h"
// game_manager
#include "game_manager/game_manager.h"
// input
#include "jing/input/input_manager.h"
//ecs
#include "ecs/components.h"
// resource
#include "resource/font.h"

//===================================================
// 無名名前空間
//===================================================
namespace /* Anonymous */
{
	//---------------------------------------------------
	// パネルの衝突判定
	//---------------------------------------------------
	bool IsCollisionPanel(const Vec3& pos1, const Vec3& size1, const Vec3& pos2, const Vec3& size2)
	{
		float x = fabsf(pos1.x - pos2.x);
		float y = fabsf(pos1.y - pos2.y);

		if (x <= size1.x * HALF + size2.x * HALF &&
			y <= size1.y * HALF + size2.y * HALF)
		{
			return true;
		}

		return false;
	}

	constexpr float GAME_HALF_SIZE = 18.0f;
	constexpr float GAME_SIZE = GAME_HALF_SIZE * 2.0f;
	constexpr float GAME_OFFSET = 5.0f;

	/** @brief ランダムジェネレータ */
	class RandomGenerator
	{
	public:
		/**
		 * @brief 範囲を指定してランダムな値を取得
		 * @tparam T 算術型
		 * @param min 最小値
		 * @param max 最大値
		 * @return ランダムな値
		 */
		template<typename T>
		static T Get(
			T min = std::numeric_limits<T>::min(),
			T max = std::numeric_limits<T>::max())
		{
			static_assert(std::is_arithmetic<T>::value, "算術型である必要があります！！！");

			if (min > max)
			{
				throw std::invalid_argument("min が max よりも大きいです...");
			}

			if constexpr (std::is_integral<T>::value)
			{
				std::uniform_int_distribution<T> dis(min, max);
				return dis(_generator);
			}
			else
			{
				std::uniform_real_distribution<T> dis(min, max);
				return dis(_generator);
			}
		}

	private:

		static std::random_device _randomDevice; //!< 乱数生成エンジン
		static std::mt19937 _generator; //!< メルセンヌ・ツイスタを使用した生成器

		/** @brief コンストラクタを削除 */
		RandomGenerator() = delete;

		/** @brief デストラクタを削除 */
		~RandomGenerator() = delete;
	};

	std::random_device RandomGenerator::_randomDevice;
	std::mt19937 RandomGenerator::_generator(RandomGenerator::_randomDevice());

} // namespace /* Anonymous */

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
SceneGame::SceneGame()
	: m_topPlayer(entt::null)
	, m_middlePlayer(entt::null)
	, m_bottomPlayer(entt::null)
	, m_doDownMiddle(false)
	, m_doUpMiddle(false)
	, m_doLeftMiddle(false)
	, m_doRightMiddle(false)
	, m_doDownTop(false)
	, m_doUpTop(false)
	, m_doLeftTop(false)
	, m_doRightTop(false)
	, m_time(90.0f)
	, m_topGameOver(false)
	, m_middleGameOver(false)
	, m_bottomGameOver(false)
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT SceneGame::Init()
{
	// 親クラスの処理
	HRESULT hr = SceneBase::Init();
	if (FAILED(hr))
	{
		return hr;
	}

	// エンティティの生成
	{
		// プレイヤー
		{
			// 一番下のプレイヤー
			m_bottomPlayer = CreatePolygon2D(
				"assets\\images\\santa_anim_00.png",
				Vec3(200.0f, 600.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
				Color(0xFFFFFFFF),
				3U, 4U, 1U, 2, 2);

			// 中のプレイヤー
			m_middlePlayer = CreatePolygon2D(
				"assets\\images\\santa_anim_00.png",
				Vec3(320.0f, 560.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
				Color(0xFFFFFFFF),
				3U, 4U, 1U, 1, 1);

			// 一番上のプレイヤー
			m_topPlayer = CreatePolygon2D(
				"assets\\images\\santa_anim_00.png",
				Vec3(260.0f, 90.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
				Color(0xFFFFFFFF),
				3U, 4U, 1U, 0, 0);
		}

		// 通行者の配置
		{
			// 一番上の通行者
			CreatePolygon2D(
				"assets\\images\\passerby.png",
				Vec3(540.0f, 180.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
				Color(0xFFFFFFFF),
				1U, 1U, 1U, -1, 0, -1, false, true);

			// 中の通行者
			CreatePolygon2D(
				"assets\\images\\passerby.png",
				Vec3(40.0f, 40.0f + 240.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
				Color(0xFFFFFFFF),
				1U, 1U, 1U, -1, 1, -1, false, true);

			// 一番下の通行者
			CreatePolygon2D(
				"assets\\images\\passerby.png",
				Vec3(60.0f, 200.0f + 240.0f * 2.0f, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
				Color(0xFFFFFFFF),
				1U, 1U, 1U, -1, 2, -1, false, true);
		}

		// パネル
		{
			// 真ん中のパネルたち
			{
				// 真ん中の上方向パネル
				CreatePolygon2D(
					"assets\\images\\button_up000.png",
					Vec3(320.0f, 360.0f - GAME_HALF_SIZE - GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 0);

				// 真ん中の下方向パネル
				CreatePolygon2D(
					"assets\\images\\button_down000.png",
					Vec3(320.0f, 360.0f + GAME_HALF_SIZE + GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 1);

				// 真ん中の左方向パネル
				CreatePolygon2D(
					"assets\\images\\button_left000.png",
					Vec3(320.0f - GAME_SIZE - GAME_OFFSET, 360.0f + GAME_HALF_SIZE + GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 3);

				// 真ん中の右方向パネル
				CreatePolygon2D(
					"assets\\images\\button_right000.png",
					Vec3(320.0f + GAME_SIZE + GAME_OFFSET, 360.0f + GAME_HALF_SIZE + GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 2);
			}

			// 一番下パネルたち
			{
				// 一番下の上方向パネル
				CreatePolygon2D(
					"assets\\images\\button_up000.png",
					Vec3(320.0f, 600.0f - GAME_HALF_SIZE - GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 0);

				// 一番下の下方向パネル
				CreatePolygon2D(
					"assets\\images\\button_down000.png",
					Vec3(320.0f, 600.0f + GAME_HALF_SIZE + GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 1);

				// 一番下の左方向パネル
				CreatePolygon2D(
					"assets\\images\\button_left000.png",
					Vec3(320.0f - GAME_SIZE - GAME_OFFSET, 600.0f + GAME_HALF_SIZE + GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 3);

				// 一番下の右方向パネル
				CreatePolygon2D(
					"assets\\images\\button_right000.png",
					Vec3(320.0f + GAME_SIZE + GAME_OFFSET, 600.0f + GAME_HALF_SIZE + GAME_OFFSET, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, 2);
			}
		}

		// 家
		{
			std::vector<std::string> keys =
			{
				"assets\\images\\ie_front_01_snow_animation.png",
				"assets\\images\\ie_front_02_snow_animation.png",
				"assets\\images\\ie_front_03_snow_animation.png",
				"assets\\images\\ie_front_04_snow_animation.png",
				"assets\\images\\ie_front_05_snow_animation.png",
				"assets\\images\\ie_front_06_snow_animation.png",
				"assets\\images\\ie_front_07_snow_animation.png",
			};

			// 一番上のおうちたち
			{
				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(100.0f, 100.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(640.0f - 100.0f, 80.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(150.0f, 200.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(400.0f, 80.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(340.0f, 20.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(580.0f, 180.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);
			}

			// 真ん中のおうちたち
			{
				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(100.0f, 100.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(340.0f - 100.0f, 90.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(420.0f, 130.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(100.0f, 180.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(340.0f - 10.0f, 10.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(580.0f - 100.0f, 180.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(580.0f, 170.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(530.0f, 50.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(70.0f, 50.0f + 240.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);
			}

			// 一番下
			{
				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(50.0f, 40.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(90.0f, 180.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(110.0f, 100.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(150.0f, 40.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(160.0f, 190.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(400.0f, 50.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(440.0f, 100.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(350.0, 200.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(500.0, 165.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(520.0, 65.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(580.0f, 180.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);

				CreatePolygon2D(
					keys[RandomGenerator::Get(0, static_cast<int>(keys.size() - 1))],
					Vec3(590.0f, 50.0f + 240.0f * 2.0f, 0.0f),
					Vec3(0.0f, 0.0f, 0.0f),
					Vec3(GAME_SIZE, GAME_SIZE, 0.0f),
					Color(0xFFFFFFFF),
					2U, 1U, 1U, -1, -1, -1, true);
			}
		}

		// 背景
		CreatePolygon2D(
			"assets\\images\\game_bg.png",
			Vec3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
			Color(0xFFFFFFFF),
			1U, 1U, 1U);
	}

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void SceneGame::Uninit()
{
	// 親クラスの処理
	SceneBase::Uninit();
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void SceneGame::Update(float deltaTime)
{
	// 親クラスの処理
	SceneBase::Update(deltaTime);

	{
		auto& fm = FontManager::getInstance();
		DWORD fmt = static_cast<DWORD>(Font::FMT_FLAG::CENTER);

		std::string score = "Score: " + std::to_string(GM.GetScore());
		auto& currentTime = fm.getFont(FontManager::FONT_TYPE::SCORE);
		currentTime.setText(
			Vec2(100.0f, 30.0f),
			40,
			Color(56.0f / 255.0f, 195.0f / 255.0f, 1.0f, 1.0f),
			fmt,
			score);
	}

	{
		auto& fm = FontManager::getInstance();
		DWORD fmt = static_cast<DWORD>(Font::FMT_FLAG::CENTER);

		std::string score = "Timer: " + std::to_string(m_time);
		auto& currentTime = fm.getFont(FontManager::FONT_TYPE::TIME);
		currentTime.setText(
			Vec2(540.0f, 30.0f),
			40,
			Color(56.0f / 255.0f, 125.0f / 255.0f, 1.0f, 1.0f),
			fmt,
			score);
	}

	m_time -= deltaTime;
	if (m_time < 0.0f || m_bottomGameOver)
	{ // 1分30秒経過
		m_time = 0.0f;
		GM.ChangeScene();
	}

#ifdef _DEBUG
	Jing::InputManager& im = Jing::InputManager::GetInstance(); // 入力処理のマネージャー
	const Jing::Keyboard& keyboard = im.GetKeyboard(); (void)keyboard; // キーボード
	const Jing::Mouse& mouse = im.GetMouse(); (void)mouse; // マウス
	const Jing::Gamepad& gamepad = im.GetGamepad(); (void)gamepad; // ゲームパッド

	// 画面遷移
	if (keyboard.GetTrigger(DIK_RETURN) ||
		gamepad.GetButtonTrigger(Jing::Gamepad::ButtonType::A) ||
		gamepad.GetButtonTrigger(Jing::Gamepad::ButtonType::Start))
	{
		GM.ChangeScene();
	}
#endif
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void SceneGame::Draw() const
{
	// 親クラスの処理
	SceneBase::Draw();
}

//---------------------------------------------------
// エンティティの更新
//---------------------------------------------------
void SceneGame::UpdateSystem(float deltaTime)
{
	Jing::InputManager& im = Jing::InputManager::GetInstance(); // 入力処理のマネージャー
	const Jing::Keyboard& keyboard = im.GetKeyboard(); (void)keyboard; // キーボード
	const Jing::Mouse& mouse = im.GetMouse(); (void)mouse; // マウス
	const Jing::Gamepad& gamepad = im.GetGamepad(); (void)gamepad; // ゲームパッド

	// 一番下のプレイヤーの更新
	if (m_registry.valid(m_bottomPlayer))
	{
		auto& move = m_registry.get<MoveComp>(m_bottomPlayer);
		auto& polygon2d = m_registry.get<Polygon2DComp>(m_bottomPlayer);

		move.move = Vec3(0.0f, 0.0f, 0.0f);

		// Horizontal
		float h = 0.0f;
		if (keyboard.GetPress(DIK_W) || keyboard.GetPress(DIK_UP) || gamepad.GetButtonPress(Jing::Gamepad::ButtonType::Up)) { h -= 1.0f; }
		if (keyboard.GetPress(DIK_S) || keyboard.GetPress(DIK_DOWN) || gamepad.GetButtonPress(Jing::Gamepad::ButtonType::Down)) { h += 1.0f; }

		// Vertical
		float v = 0.0f;
		if (keyboard.GetPress(DIK_A) || keyboard.GetPress(DIK_UP) || gamepad.GetButtonPress(Jing::Gamepad::ButtonType::Left)) { v -= 1.0f; }
		if (keyboard.GetPress(DIK_D) || keyboard.GetPress(DIK_DOWN) || gamepad.GetButtonPress(Jing::Gamepad::ButtonType::Right)) { v += 1.0f; }

		move.move.x = v;
		move.move.y = h;
		if (h != 0.0f)
		{
			move.move.x = 0.0f;
		}

		move.move *= 100.0f;

#ifdef _DEBUG
#if 0
		ImGui::Begin("Game Debug");
		ImGui::Text("Player 1");
		ImGui::Text("Pos: (%.2f, %.2f)", polygon2d.pos.x, polygon2d.pos.y);
		ImGui::Text("Move: (%.2f, %.2f)", move.move.x, move.move.y);
		ImGui::End();
#endif
#endif // _DEBUG
	}

	// 中のプレイヤーの更新
	if (m_registry.valid(m_middlePlayer))
	{
		auto& move = m_registry.get<MoveComp>(m_middlePlayer);

		move.move = Vec3(0.0f, 0.0f, 0.0f);

		static const float SPEED = 0.5f * 75.1110f;

		if (m_doDownMiddle)
		{
			move.move.y = SPEED;
		}
		else if (m_doUpMiddle)
		{
			move.move.y = -SPEED;
		}
		else if (m_doLeftMiddle)
		{
			move.move.x = SPEED;
		}
		else if (m_doRightMiddle)
		{
			move.move.x = -SPEED;
		}
	}

	// 一番上のプレイヤーの更新
	if (m_registry.valid(m_topPlayer))
	{
		auto& move = m_registry.get<MoveComp>(m_topPlayer);

		move.move = Vec3(0.0f, 0.0f, 0.0f);

		static const float SPEED = 0.25f * 75.1110f;

		if (m_doDownTop)
		{
			move.move.y = SPEED;
		}
		else if (m_doUpTop)
		{
			move.move.y = -SPEED;
		}
		else if (m_doLeftTop)
		{
			move.move.x = SPEED;
		}
		else if (m_doRightTop)
		{
			move.move.x = -SPEED;
		}
	}

	// パネルごとに当たり判定
	{
		// 上向きパネル
		{
			// リセット
			m_doUpMiddle = false;
			m_doUpTop = false;

			auto view = m_registry.view<Polygon2DComp, UpPanelComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);
				polygon2d.nowNumU = 0U;

				// 一番下のプレイヤー
				if (m_registry.valid(m_bottomPlayer))
				{
					auto& bottomPolygon2d = m_registry.get<Polygon2DComp>(m_bottomPlayer);
					if (IsCollisionPanel(bottomPolygon2d.pos, bottomPolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doUpMiddle = true;
						polygon2d.nowNumU = 1U;
					}
				}
				// 中のプレイヤー
				if (m_registry.valid(m_middlePlayer))
				{
					auto& middlePolygon2d = m_registry.get<Polygon2DComp>(m_middlePlayer);
					if (IsCollisionPanel(middlePolygon2d.pos, middlePolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doUpTop = true;
						polygon2d.nowNumU = 1U;
					}
				}
			}
		}

		// 下向きパネル
		{
			m_doDownMiddle = false;
			m_doDownTop = false;

			auto view = m_registry.view<Polygon2DComp, DownPanelComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);
				polygon2d.nowNumU = 0U;
				// 一番下のプレイヤー
				if (m_registry.valid(m_bottomPlayer))
				{
					auto& bottomPolygon2d = m_registry.get<Polygon2DComp>(m_bottomPlayer);
					if (IsCollisionPanel(bottomPolygon2d.pos, bottomPolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doDownMiddle = true;
						polygon2d.nowNumU = 1U;
					}
				}
				// 中のプレイヤー
				if (m_registry.valid(m_middlePlayer))
				{
					auto& middlePolygon2d = m_registry.get<Polygon2DComp>(m_middlePlayer);
					if (IsCollisionPanel(middlePolygon2d.pos, middlePolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doDownTop = true;
						polygon2d.nowNumU = 1U;
					}
				}
			}
		}

		// 左向きパネル
		{
			m_doLeftMiddle = false;
			m_doLeftTop = false;

			auto view = m_registry.view<Polygon2DComp, LeftPanelComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);
				polygon2d.nowNumU = 0U;

				// 一番下のプレイヤー
				if (m_registry.valid(m_bottomPlayer))
				{
					auto& bottomPolygon2d = m_registry.get<Polygon2DComp>(m_bottomPlayer);
					if (IsCollisionPanel(bottomPolygon2d.pos, bottomPolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doLeftMiddle = true;
						polygon2d.nowNumU = 1U;
					}
				}

				// 中のプレイヤー
				if (m_registry.valid(m_middlePlayer))
				{
					auto& middlePolygon2d = m_registry.get<Polygon2DComp>(m_middlePlayer);
					if (IsCollisionPanel(middlePolygon2d.pos, middlePolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doLeftTop = true;
						polygon2d.nowNumU = 1U;
					}
				}
			}
		}

		// 右向きパネル
		{
			m_doRightMiddle = false;
			m_doRightTop = false;

			auto view = m_registry.view<Polygon2DComp, RightPanelComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);
				polygon2d.nowNumU = 0U;

				// 一番下のプレイヤー
				if (m_registry.valid(m_bottomPlayer))
				{
					auto& bottomPolygon2d = m_registry.get<Polygon2DComp>(m_bottomPlayer);
					if (IsCollisionPanel(bottomPolygon2d.pos, bottomPolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doRightMiddle = true;
						polygon2d.nowNumU = 1U;
					}
				}

				// 中のプレイヤー
				if (m_registry.valid(m_middlePlayer))
				{
					auto& middlePolygon2d = m_registry.get<Polygon2DComp>(m_middlePlayer);
					if (IsCollisionPanel(middlePolygon2d.pos, middlePolygon2d.size, polygon2d.pos, polygon2d.size))
					{
						m_doRightTop = true;
						polygon2d.nowNumU = 1U;
					}
				}
			}
		}
	}

	// 通行者の更新
	{
		auto view = m_registry.view<Polygon2DComp, MoveComp, PedestrianComp>();
		for (auto entity : view)
		{
			auto& polygon2d = view.get<Polygon2DComp>(entity);
			auto& move = view.get<MoveComp>(entity);
			auto& people = view.get<PedestrianComp>(entity);

			static const float SPEED = 0.25f * 75.1110f;
			move.move = Vec3(0.0f, 0.0f, 0.0f);

			if (people.isMove)
			{
				people.moveTime -= deltaTime;
				if (people.moveTime <= 0.0f)
				{
					people.coolTime = RandomGenerator::Get(1.0f, 3.0f);
					people.isMove = false;
					if (RandomGenerator::Get(1, 2) == 1)
					{
						if (RandomGenerator::Get(1, 2) == 1)
						{
							people.move = Vec3(SPEED, 0.0f, 0.0f);
						}
						else
						{
							people.move = Vec3(-SPEED, 0.0f, 0.0f);
						}
					}
					else
					{
						if (RandomGenerator::Get(1, 2) == 1)
						{
							people.move = Vec3(0.0f, SPEED, 0.0f);
						}
						else
						{
							people.move = Vec3(0.0f, -SPEED, 0.0f);
						}
					}
				}

				move.move = people.move;
			}
			else
			{
				people.coolTime -= deltaTime;
				if (people.coolTime <= 0.0f)
				{
					people.isMove = true;
					people.moveTime = RandomGenerator::Get(1.0f, 3.0f);

					if (RandomGenerator::Get(1, 2) == 1)
					{
						if (RandomGenerator::Get(1, 2) == 1)
						{
							people.move = Vec3(SPEED, 0.0f, 0.0f);
						}
						else
						{
							people.move = Vec3(-SPEED, 0.0f, 0.0f);
						}
					}
					else
					{
						if (RandomGenerator::Get(1, 2) == 1)
						{
							people.move = Vec3(0.0f, SPEED, 0.0f);
						}
						else
						{
							people.move = Vec3(0.0f, -SPEED, 0.0f);
						}
					}
				}
			}

			if (m_registry.valid(m_topPlayer) && !m_topGameOver)
			{
				// 当たり判定
				auto& topPolygon2d = m_registry.get<Polygon2DComp>(m_topPlayer);
				if (IsCollisionPanel(polygon2d.pos, polygon2d.size, topPolygon2d.pos, topPolygon2d.size))
				{
					m_topGameOver = true;

					// GameOver
					CreatePolygon2D(
						"assets\\images\\gameover.png",
						Vec3(320.0f, 120.0f, 0.0f),
						Vec3(0.0f, 0.0f, 0.0f),
						Vec3(640.0f, 240.0f, 0.0f),
						Color(0xFFFFFFFF),
						1U, 1U, 1U, -1, -1, -1, false, false, true);
				}
			}

			if (m_registry.valid(m_middlePlayer) && !m_middleGameOver)
			{
				// 当たり判定
				auto& middlePolygon2d = m_registry.get<Polygon2DComp>(m_middlePlayer);
				if (IsCollisionPanel(polygon2d.pos, polygon2d.size, middlePolygon2d.pos, middlePolygon2d.size))
				{
					m_middleGameOver = true;

					// GameOver
					CreatePolygon2D(
						"assets\\images\\gameover.png",
						Vec3(320.0f, 360.0f, 0.0f),
						Vec3(0.0f, 0.0f, 0.0f),
						Vec3(640.0f, 240.0f, 0.0f),
						Color(0xFFFFFFFF),
						1U, 1U, 1U, -1, -1, -1, false, false, true);
				}
			}

			if (m_registry.valid(m_bottomPlayer) && !m_bottomGameOver)
			{
				// 当たり判定
				auto& bottomPolygon2d = m_registry.get<Polygon2DComp>(m_bottomPlayer);
				if (IsCollisionPanel(polygon2d.pos, polygon2d.size, bottomPolygon2d.pos, bottomPolygon2d.size))
				{
					m_bottomGameOver = true;

					// GameOver
					CreatePolygon2D(
						"assets\\images\\gameover.png",
						Vec3(320.0f, 600.0f, 0.0f),
						Vec3(0.0f, 0.0f, 0.0f),
						Vec3(640.0f, 240.0f, 0.0f),
						Color(0xFFFFFFFF),
						1U, 1U, 1U, -1, -1, -1, false, false, true);
				}
			}
		}
	}

	// ステージごとの移動の制限
	{
		// 一番上
		{
			auto view = m_registry.view<Polygon2DComp, TopStageComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);

				if (polygon2d.pos.x < 10.0f + GAME_HALF_SIZE)
				{
					polygon2d.pos.x = 10.0f + GAME_HALF_SIZE;
				}
				else if (polygon2d.pos.x > SCREEN_WIDTH * HALF - 10.0f - GAME_HALF_SIZE)
				{
					polygon2d.pos.x = SCREEN_WIDTH * HALF - 10.0f - GAME_HALF_SIZE;
				}

				if (polygon2d.pos.y < 10.0f + GAME_HALF_SIZE)
				{
					polygon2d.pos.y = 10.0f + GAME_HALF_SIZE;
				}
				else if (polygon2d.pos.y > 240.0f - 10.0f - GAME_HALF_SIZE)
				{
					polygon2d.pos.y = 240.0f - 10.0f - GAME_HALF_SIZE;
				}
			}
		}

		// 中
		{
			auto view = m_registry.view<Polygon2DComp, MiddleStageComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);
				if (polygon2d.pos.x < 10.0f + GAME_HALF_SIZE)
				{
					polygon2d.pos.x = 10.0f + GAME_HALF_SIZE;
				}
				else if (polygon2d.pos.x > SCREEN_WIDTH * HALF - 10.0f - GAME_HALF_SIZE)
				{
					polygon2d.pos.x = SCREEN_WIDTH * HALF - 10.0f - GAME_HALF_SIZE;
				}

				if (polygon2d.pos.y < 240.0f + 10.0f + GAME_HALF_SIZE)
				{
					polygon2d.pos.y = 240.0f + 10.0f + GAME_HALF_SIZE;
				}
				else if (polygon2d.pos.y > 240.0f + 240.0f - 10.0f - GAME_HALF_SIZE)
				{
					polygon2d.pos.y = 240.0f + 240.0f - 10.0f - GAME_HALF_SIZE;
				}
			}
		}

		// 一番下
		{
			auto view = m_registry.view<Polygon2DComp, BottomStageComp>();
			for (auto entity : view)
			{
				auto& polygon2d = view.get<Polygon2DComp>(entity);
				if (polygon2d.pos.x < 10.0f + GAME_HALF_SIZE)
				{
					polygon2d.pos.x = 10.0f + GAME_HALF_SIZE;
				}
				else if (polygon2d.pos.x > SCREEN_WIDTH * HALF - 10.0f - GAME_HALF_SIZE)
				{
					polygon2d.pos.x = SCREEN_WIDTH * HALF - 10.0f - GAME_HALF_SIZE;
				}

				if (polygon2d.pos.y < 240.0f + 240.0f + 10.0f + GAME_HALF_SIZE)
				{
					polygon2d.pos.y = 240.0f + 240.0f + 10.0f + GAME_HALF_SIZE;
				}
				else if (polygon2d.pos.y > 240.0f + 240.0f + 240.0f - 10.0f - GAME_HALF_SIZE)
				{
					polygon2d.pos.y = 240.0f + 240.0f + 240.0f - 10.0f - GAME_HALF_SIZE;
				}
			}
		}
	}

	// おうちの更新処理
	{
		auto view = m_registry.view<House, Polygon2DComp>();
		for (auto entity : view)
		{
			auto& house = view.get<House>(entity);
			auto& polygon2d = view.get<Polygon2DComp>(entity);
			if (house.isShines)
			{
				house.shineTime -= deltaTime;
				if (house.shineTime <= 0.0f)
				{
					house.isShines = false;
					house.coolTime = RandomGenerator::Get(3.0f, 5.0f);
				}

				polygon2d.nowNumU = 1;


			}
			else
			{
				house.coolTime -= deltaTime;
				if (house.coolTime <= 0.0f)
				{
					house.isShines = true;
					house.shineTime = RandomGenerator::Get(3.0f, 5.0f);
				}

				polygon2d.nowNumU = 0;

				// 一番上のプレイヤー
				if (m_registry.valid(m_topPlayer))
				{
					if (IsCollisionPanel(polygon2d.pos, polygon2d.size, m_registry.get<Polygon2DComp>(m_topPlayer).pos, m_registry.get<Polygon2DComp>(m_topPlayer).size))
					{
						house.isShines = true;
						house.shineTime = 500.0f;
						GM.AddScore(30);
					}
				}

				// 中のプレイヤー
				if (m_registry.valid(m_middlePlayer))
				{
					if (IsCollisionPanel(polygon2d.pos, polygon2d.size, m_registry.get<Polygon2DComp>(m_middlePlayer).pos, m_registry.get<Polygon2DComp>(m_middlePlayer).size))
					{
						house.isShines = true;
						house.shineTime = 500.0f;
						GM.AddScore(5);
					}
				}

				// 一番下のプレイヤー
				if (m_registry.valid(m_bottomPlayer))
				{
					if (IsCollisionPanel(polygon2d.pos, polygon2d.size, m_registry.get<Polygon2DComp>(m_bottomPlayer).pos, m_registry.get<Polygon2DComp>(m_bottomPlayer).size))
					{
						house.isShines = true;
						house.shineTime = 500.0f;
						GM.AddScore(1);
					}
				}
			}
		}
	}

	// 移動の更新
	{
		auto view = m_registry.view<MoveComp, Polygon2DComp>();
		for (auto entity : view)
		{
			auto& move = view.get<MoveComp>(entity);
			auto& polygon2d = view.get<Polygon2DComp>(entity);

			move.move *= 2.0f /* 速度 */ * deltaTime;
			polygon2d.pos += move.move;
		}
	}

	// 2D ポリゴンの更新
	{
		auto view = m_registry.view<Polygon2DComp>();
		for (auto entity : view)
		{
			auto& polygon2d = view.get<Polygon2DComp>(entity);

			// 頂点バッファをロック
			Vertex2D* pVtx;
			HRESULT hr = polygon2d.vtxBuf->Lock(0U, 0U, reinterpret_cast<void**>(&pVtx), 0U);
			assert(SUCCEEDED(hr));

			// 対角線の長さを計算
			const float length = sqrtf(polygon2d.size.x * polygon2d.size.x + polygon2d.size.y * polygon2d.size.y) * HALF;

			// 対角線の角度を計算
			const float angle = atan2f(polygon2d.size.x, polygon2d.size.y);

			// 座標の計算
			float u = 1.0f / static_cast<float>(polygon2d.numU);
			float u1 = static_cast<float>(polygon2d.nowNumU) * u;
			float u2 = static_cast<float>(polygon2d.nowNumU + 1) * u;
			float v = 1.0f / static_cast<float>(polygon2d.numV);
			float v1 = static_cast<float>(polygon2d.nowNumV) * v;
			float v2 = static_cast<float>(polygon2d.nowNumV + 1) * v;

			// 頂点座標の設定
			pVtx[0].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z - (D3DX_PI - angle)) * length, polygon2d.pos.y + cosf(polygon2d.rot.z - (D3DX_PI - angle)) * length, 0.0f);
			pVtx[1].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z + (D3DX_PI - angle)) * length, polygon2d.pos.y + cosf(polygon2d.rot.z + (D3DX_PI - angle)) * length, 0.0f);
			pVtx[2].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z - angle) * length, polygon2d.pos.y + cosf(polygon2d.rot.z - angle) * length, 0.0f);
			pVtx[3].pos = Vec3(polygon2d.pos.x + sinf(polygon2d.rot.z + angle) * length, polygon2d.pos.y + cosf(polygon2d.rot.z + angle) * length, 0.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = Vec2(u1, v1);
			pVtx[1].tex = Vec2(u2, v1);
			pVtx[2].tex = Vec2(u1, v2);
			pVtx[3].tex = Vec2(u2, v2);

			// 頂点カラーの設定
			for (int i = 0; i < 4; i++)
			{
				pVtx[i].col = polygon2d.col;
			}

			// 頂点バッファをアンロック
			hr = polygon2d.vtxBuf->Unlock();
			assert(SUCCEEDED(hr));
		}
	}
}

//---------------------------------------------------
// エンティティの描画
//---------------------------------------------------
void SceneGame::DrawSystem() const
{
	// 2D ポリゴンの描画
	{
		auto view = m_registry.view<Polygon2DComp>();
		for (auto entity : view)
		{
			const Polygon2DComp& polygon2D = view.get<Polygon2DComp>(entity);

			// デバイスの取得
			auto device = GM.GetDevice();

			Texture texture = m_textures[polygon2D.key].Get();

			// 頂点バッファをデータストリームに設定
			device->SetStreamSource(0U, polygon2D.vtxBuf.Get(), 0U, sizeof(Vertex2D));

			// 頂点フォーマットの設定
			device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			device->SetTexture(0U, texture);

			// ポリゴンの描画
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0U, 2U);
		}
	}

	// GameOver 表示
	{
		auto view = m_registry.view<Polygon2DComp, GameOverComp>();
		for (auto entity : view)
		{
			const Polygon2DComp& polygon2D = view.get<Polygon2DComp>(entity);

			// デバイスの取得
			auto device = GM.GetDevice();

			Texture texture = m_textures[polygon2D.key].Get();

			// 頂点バッファをデータストリームに設定
			device->SetStreamSource(0U, polygon2D.vtxBuf.Get(), 0U, sizeof(Vertex2D));

			// 頂点フォーマットの設定
			device->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			device->SetTexture(0U, texture);

			// ポリゴンの描画
			device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0U, 2U);
		}
	}
}

//---------------------------------------------------
// 次のシーンの設定
//---------------------------------------------------
std::shared_ptr<SceneBase> SceneGame::ChangeScene()
{
	{
		auto& fm = FontManager::getInstance();
		DWORD fmt = static_cast<DWORD>(Font::FMT_FLAG::CENTER);

		std::string score = "";
		auto& currentTime = fm.getFont(FontManager::FONT_TYPE::SCORE);
		currentTime.setText(
			Vec2(320.0f, 50.0f),
			30,
			Color(0.0f, 0.0f, 0.0f, 1.0f),
			fmt,
			score);
	}

	{
		auto& fm = FontManager::getInstance();
		DWORD fmt = static_cast<DWORD>(Font::FMT_FLAG::CENTER);

		std::string score = "";
		auto& currentTime = fm.getFont(FontManager::FONT_TYPE::TIME);
		currentTime.setText(
			Vec2(320.0f, 50.0f),
			30,
			Color(0.0f, 0.0f, 0.0f, 1.0f),
			fmt,
			score);
	}

	// 終了処理
	Uninit();

	std::shared_ptr<SceneBase> nextScene = std::make_shared<SceneResult>();
	return nextScene;
}
