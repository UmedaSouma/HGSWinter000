//--------------------------------------------------------------------------------
// 
// チュートリアルのシーン [scene_tutorial.cpp]
// Author: Taito Jin
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
// scene
#include "scene/scene_tutorial.h"
#include "scene/scene_game.h"
// game_manager
#include "game_manager/game_manager.h"
// input
#include "jing/input/input_manager.h"

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
SceneTutorial::SceneTutorial()
{
	/* DO_NOTHING */
}

//---------------------------------------------------
// 初期設定
//---------------------------------------------------
HRESULT SceneTutorial::Init()
{
	// 親クラスの処理
	HRESULT hr = SceneBase::Init();
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

//---------------------------------------------------
// 終了処理
//---------------------------------------------------
void SceneTutorial::Uninit()
{
	// 親クラスの処理
	SceneBase::Uninit();
}

//---------------------------------------------------
// 更新処理
//---------------------------------------------------
void SceneTutorial::Update(float deltaTime)
{
	// 親クラスの処理
	SceneBase::Update(deltaTime);

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
}

//---------------------------------------------------
// 描画処理
//---------------------------------------------------
void SceneTutorial::Draw() const
{
	// 親クラスの処理
	SceneBase::Draw();
}

//---------------------------------------------------
// エンティティの更新
//---------------------------------------------------
void SceneTutorial::UpdateSystem(float deltaTime)
{

}

//---------------------------------------------------
// エンティティの描画
//---------------------------------------------------
void SceneTutorial::DrawSystem() const
{

}

//---------------------------------------------------
// 次のシーンの設定
//---------------------------------------------------
std::shared_ptr<SceneBase> SceneTutorial::ChangeScene()
{
	// 終了処理
	Uninit();

	std::shared_ptr<SceneBase> nextScene = std::make_shared<SceneGame>();
	return nextScene;
}
