//--------------------------------------------------------------------------------
// 
// エントリーポイント [main.cpp]
// Author: Taito Jin
// Since: 2024/12/17
// 
//--------------------------------------------------------------------------------

//---------------------------------------------------
// インクルード
//---------------------------------------------------
#include "core/my_app.h"

//---------------------------------------------------
// エントリーポイント (Release)
//---------------------------------------------------
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /*hPrevInstance*/,
	_In_ LPSTR /*pCmdLine*/,
	_In_ int nCmdShow)
{
	// メモリリーク検出
#if defined(_DEBUG) || defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // defined(_DEBUG) || defined(DEBUG)

	// アプリケーションの実行
	MyApp app(SCREEN_WIDTH, SCREEN_HEIGHT);
	app.Run(hInstance, nCmdShow);

	return 0;
}

//---------------------------------------------------
// エントリーポイント (Debug)
//---------------------------------------------------
int main()
{
	// ロケール設定
	setlocale(LC_ALL, "ja_JP");

	return WinMain(GetModuleHandle(nullptr), nullptr, GetCommandLine(), SW_SHOWNORMAL);
}
