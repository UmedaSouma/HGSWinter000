/**********************************************************//**
 * @file app.h
 * @brief アプリケーション
 *
 * @author Taito Jin
 * @date since 2024/12/17
 **************************************************************/
#pragma once

/** @brief アプリケーションのクラス */
class MyApp : public Jing::App
{
public:
	/**
	 * @brief コンストラクタ
	 * @param [in] width ウィンドウの幅
	 * @param [in] height ウィンドウの高さ
	 */
	MyApp(UINT width, UINT height);

	/** @brief デストラクタ */
	~MyApp() = default;

private:
	/**
	 * @brief 初期設定
	 * @return 結果
	 */
	HRESULT OnInit() override;

	/** @brief 終了処理 */
	void OnUninit() override;

	/**
	 * @brief 更新処理
	 * @param [in] deltaTime 前フレームからの経過時間
	 */
	void OnUpdate(float deltaTime) override;

	/** @brief 描画処理 */
	void OnDraw() override;

	/**
	 * @brief メッセージ処理
	 * @param [in] hWnd ウィンドウハンドル
	 * @param [in] msg メッセージ
	 * @param [in] wParam パラメータ
	 * @param [in] lParam パラメータ
	 */
	void OnMsgProc(HWND hWnd, UINT msg, WPARAM wP, LPARAM lP) override;
};
