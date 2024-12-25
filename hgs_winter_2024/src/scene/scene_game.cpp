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

//---------------------------------------------------
// コンストラクタ
//---------------------------------------------------
SceneGame::SceneGame()
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

#if 0
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

}

//---------------------------------------------------
// エンティティの描画
//---------------------------------------------------
void SceneGame::DrawSystem() const
{

}

//---------------------------------------------------
// 次のシーンの設定
//---------------------------------------------------
std::shared_ptr<SceneBase> SceneGame::ChangeScene()
{
	// 終了処理
	Uninit();

	std::shared_ptr<SceneBase> nextScene = std::make_shared<SceneResult>();
	return nextScene;
}
