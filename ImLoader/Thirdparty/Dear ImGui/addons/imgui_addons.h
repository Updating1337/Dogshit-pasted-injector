#pragma once

#include "../imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../imgui_internal.h"

#include "imgui_addons.h"

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

struct Button_State {
	ImVec4 frame_col;
};

struct ButtonImage_State {
	ImVec4 overlay_col;
	ImVec4 dot_col;
	ImVec4 icon_col;
};

struct Selectable_State {
	ImVec4 frame_col;
	ImVec4 label_col;
};

struct Combo_State {

	ImVec4 frame_col;
	ImVec4 bg_col;
	ImVec4 text_col;
};

struct RadioButtonImage_State {
	ImVec4 overlay_col;
	ImVec4 label_col;
	ImVec4 border_col;
};

namespace ImAdd
{
	using ComVec4 = const ImVec4&;
	static float fAnimationSpeed = 6.f;

	IMGUI_API ImVec4        Hex2RGBA(int hexValue, float alpha);
	IMGUI_API void          TextSide(ImVec4 left_text_color, ImVec4 right_text_color, const char* left_text, const char* right_text, int custom_width = NULL);
	IMGUI_API bool          Button(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImDrawFlags drawflag = ImDrawFlags_RoundCornersAll);
	IMGUI_API bool          ButtonText(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImDrawFlags drawflag = ImDrawFlags_RoundCornersAll);
	IMGUI_API bool          RadioButtonImage(ImTextureID user_texture_id, const char* label, int current_id, int* id_parent, const ImVec2& size_arg = ImVec2(0, 0));
	IMGUI_API bool			ButtonImage(ImTextureID user_texture_id, const char* str_id, const char* icon, ImVec4 icon_color, ImVec4 online_dot_color, bool online_dot = true, const ImVec2& size_arg = ImVec2(0, 0));
	IMGUI_API bool          Selectable(const char* label, bool selected, const ImVec2& size_arg = ImVec2(0, 0), ImDrawFlags draw_flags = ImDrawFlags_RoundCornersAll);
	IMGUI_API void          SeparatorText(const char* text);
	IMGUI_API bool          BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
	IMGUI_API bool          Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	IMGUI_API bool          Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int popup_max_height_in_items = -1);
	IMGUI_API bool          Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);
	IMGUI_API bool          BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags);
	IMGUI_API bool			InputText(const char* label, const char* icon, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
	IMGUI_API void			RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding, ImDrawFlags draw_flags = 0);
	IMGUI_API bool          CloseButton(ImGuiID id, const ImVec2& pos);
	IMGUI_API void          TabBox(const char* str_id, int* current_id, std::vector<const char*> list, ImVec2 size_arg);
	IMGUI_API void			Spinner(float radius, float thickness, int num_segments, ComVec4 color);
};

// Getter for the old Combo() API: const char*[]
static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
	ImGuiContext& g = *GImGui;
	if (items_count <= 0)
		return FLT_MAX;
	return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

// Getter for the old Combo() API: "item1\0item2\0item3\0"
static bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
	// FIXME-OPT: we could pre-compute the indices to fasten this. But only 1 active combo means the waste is limited.
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

const char* const szKeyNames[] =
{
	"Unknown",
	"LBUTTON",
	"RBUTTON",
	"CANCEL",
	"MBUTTON",
	"XBUTTON1",
	"XBUTTON2",
	"Unknown",
	"BACK",
	"TAB",
	"Unknown",
	"Unknown",
	"CLEAR",
	"RETURN",
	"Unknown",
	"Unknown",
	"SHIFT",
	"CONTROL",
	"MENU",
	"PAUSE",
	"CAPITAL",
	"KANA",
	"Unknown",
	"JUNJA",
	"FINAL",
	"KANJI",
	"Unknown",
	"ESCAPE",
	"CONVERT",
	"NONCONVERT",
	"ACCEPT",
	"MODECHANGE",
	"SPACE",
	"PRIOR",
	"NEXT",
	"END",
	"HOME",
	"LEFT",
	"UP",
	"RIGHT",
	"DOWN",
	"SELECT",
	"PRINT",
	"EXECUTE",
	"SNAPSHOT",
	"INSERT",
	"DELETE",
	"HELP",
	"NUMPAD 0",
	"NUMPAD 1",
	"NUMPAD 2",
	"NUMPAD 3",
	"NUMPAD 4",
	"NUMPAD 5",
	"NUMPAD 6",
	"NUMPAD 7",
	"NUMPAD 8",
	"NUMPAD 9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"KEY A",
	"KEY B",
	"KEY C",
	"KEY D",
	"KEY E",
	"KEY F",
	"KEY G",
	"KEY H",
	"KEY I",
	"KEY J",
	"KEY K",
	"KEY L",
	"KEY M",
	"KEY N",
	"KEY O",
	"KEY P",
	"KEY Q",
	"KEY R",
	"KEY S",
	"KEY T",
	"KEY U",
	"KEY V",
	"KEY W",
	"KEY X",
	"KEY Y",
	"KEY Z",
	"LWIN",
	"RWIN",
	"APPS",
	"Unknown",
	"SLEEP",
	"NUMPAD 0",
	"NUMPAD 1",
	"NUMPAD 2",
	"NUMPAD 3",
	"NUMPAD 4",
	"NUMPAD 5",
	"NUMPAD 6",
	"NUMPAD 7",
	"NUMPAD 8",
	"NUMPAD 9",
	"MULTIPLY",
	"ADD",
	"SEPARATOR",
	"SUBTRACT",
	"DECIMAL",
	"DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"F13",
	"F14",
	"F15",
	"F16",
	"F17",
	"F18",
	"F19",
	"F20",
	"F21",
	"F22",
	"F23",
	"F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"NUMLOCK",
	"SCROLL",
	"OEM_NEC_EQUAL",
	"OEM_FJ_MASSHOU",
	"OEM_FJ_TOUROKU",
	"OEM_FJ_LOYA",
	"OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LSHIFT",
	"RSHIFT",
	"LCONTROL",
	"RCONTROL",
	"LMENU",
	"RMENU"
};