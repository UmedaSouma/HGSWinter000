/**********************************************************//**
 * @file imgui_addons.h
 * @brief 拡張機能
 *
 * @author Taito Jin
 * @date since 2024/11/23
 **************************************************************/
#pragma once

/** @brief 拡張機能の名前空間 */
namespace ImGuiAddons
{
	/**
	 * @brief BufferingBar
	 * @param [in] label ラベル
	 * @param [in] value
	 * @param [in] size_arg
	 * @param [in] bg_col
	 * @param [in] fg_col
	 */
	void BufferingBar(const char* label, float value, const ImVec2& size_arg, const ImU32& bg_col, const ImU32& fg_col);

	/**
	 * @brief Spinner
	 * @param [in] label ラベル
	 * @param [in] radius
	 * @param [in] thickness
	 * @param [in] color 色
	 */
	bool Spinner(const char* label, float radius, int thickness, const ImU32& color);

	/**
	 * @brief ToggleButton
	 * @param [in] str_id ラベル
	 * @param [in] v
	 */
	void ToggleButton(const char* str_id, bool& v);

#if 0
	// others
	bool Transform(const std::string& inTitle, D3DXVECTOR3* outResult);		// Transform (MEMO: 返り値 => 入力を決定したかどうか)
	bool TransformVec2(const std::string& inTitle, D3DXVECTOR2* outResult);	// Transform Vector2 (MEMO: 返り値 => 入力を決定したかどうか)
	bool Float(const std::string& inTitle, float* outResult);				// float (MEMO: 返り値 => 入力を決定したかどうか)
	bool Int(const std::string& inTitle, int* outResult);					// int (MEMO: 返り値 => 入力を決定したかどうか)
	void Material(const std::string& inTitle, D3DCOLORVALUE& inOutResult);	// Material
	void Color(const std::string& inTitle, D3DXCOLOR& inOutResult);			// 色
#endif
}
