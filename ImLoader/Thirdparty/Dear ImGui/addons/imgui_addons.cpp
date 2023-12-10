#include "imgui_addons.h"

#define _CRT_SECURE_NO_WARNINGS

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ImVec4 ImAdd::Hex2RGBA(int hexValue, float alpha)
{
    float r = (hexValue >> 16) & 0xFF;
    float g = (hexValue >> 8) & 0xFF;
    float b = hexValue & 0xFF;

    return ImVec4(r / 255, g / 255, b / 255, alpha);
}

void ImAdd::TextSide(ImVec4 left_text_color, ImVec4 right_text_color, const char* left_text, const char* right_text, int custom_width)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;
    
    const float width = custom_width > 0 ? custom_width : ImGui::CalcItemSize(ImVec2(-0.1f, 0.f), 0.f, 0.f).x + ImGui::GetStyle().WindowPadding.x;

    ImGui::TextColored(left_text_color, left_text);
    ImGui::SameLine(width - ImGui::CalcTextSize(right_text).x);
    ImGui::TextColored(right_text_color, right_text);
}

bool ImAdd::Button(const char* label, const ImVec2& size_arg, ImDrawFlags drawflag)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    // Animate Colors
    static std::map<ImGuiID, Button_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, Button_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.frame_col = ImLerp(it_anim->second.frame_col, (held && hovered) ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : hovered ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) : ImGui::GetStyleColorVec4(ImGuiCol_Button), g.IO.DeltaTime * fAnimationSpeed);

    // Render
    const ImU32 col = ImGui::GetColorU32(it_anim->second.frame_col);
    ImGui::RenderNavHighlight(bb, id);

    window->DrawList->AddRectFilled(bb.Min, bb.Max, col, style.FrameRounding, drawflag);
    if (style.FrameBorderSize > 0.0f)
    {
        window->DrawList->AddRect(bb.Min + ImVec2(1, 1), bb.Max + ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_BorderShadow), style.FrameRounding, drawflag, style.FrameBorderSize);
        window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(ImGuiCol_Border), style.FrameRounding, drawflag, style.FrameBorderSize);
    }

    if (g.LogEnabled)
        ImGui::LogSetNextTextDecoration("[", "]");
    ImGui::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::ButtonText(const char* label, const ImVec2& size_arg, ImDrawFlags drawflag)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x, label_size.y);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    // Animate Colors
    static std::map<ImGuiID, Button_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, Button_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.frame_col = ImLerp(it_anim->second.frame_col, (held && hovered) ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : hovered ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) : ImGui::GetStyleColorVec4(ImGuiCol_Button), g.IO.DeltaTime * fAnimationSpeed);

    // Render
    const ImU32 col = ImGui::GetColorU32(it_anim->second.frame_col);
    ImGui::RenderNavHighlight(bb, id);

    if (g.LogEnabled)
        ImGui::LogSetNextTextDecoration("[", "]");

    ImGui::PushStyleColor(ImGuiCol_Text, it_anim->second.frame_col);
    ImGui::RenderTextClipped(bb.Min, bb.Max, label, NULL, &label_size, style.ButtonTextAlign, &bb);
    ImGui::PopStyleColor();

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::RadioButtonImage(ImTextureID user_texture_id, const char* label, int current_id, int* id_parent, const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held, active, disabled;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    active = *id_parent == current_id;
    disabled = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;

    if (pressed)
    {
        *id_parent = current_id;
    }

    // Animate Colors
    static std::map<ImGuiID, RadioButtonImage_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, RadioButtonImage_State() });
        it_anim = anim.find(id);
    }

    ImVec4 label_rgba = ImGui::GetStyleColorVec4(disabled ? ImGuiCol_TextDisabled : ImGuiCol_Text);
    ImVec4 label_col = active ? label_rgba : ImVec4(label_rgba.x, label_rgba.y, label_rgba.z, 0.f);
    it_anim->second.label_col = ImLerp(it_anim->second.label_col, label_col, g.IO.DeltaTime * fAnimationSpeed);
    
    ImVec4 overlay_rgba = ImGui::GetStyleColorVec4(ImGuiCol_TitleBg);
    ImVec4 overlay_disabled = overlay_rgba;
    ImVec4 overlay_col = disabled ? overlay_disabled : active ? overlay_rgba : hovered ? ImGui::GetStyleColorVec4(ImGuiCol_TitleBgActive) : ImVec4(overlay_rgba.x, overlay_rgba.y, overlay_rgba.z, 0.f);
    it_anim->second.overlay_col = ImLerp(it_anim->second.overlay_col, overlay_col, g.IO.DeltaTime * fAnimationSpeed);
    
    ImVec4 border_rgba = ImGui::GetStyleColorVec4(disabled ? ImGuiCol_TextDisabled : ImGuiCol_Border);
    ImVec4 border_col = active ? border_rgba : ImVec4(border_rgba.x, border_rgba.y, border_rgba.z, 0.f);
    it_anim->second.border_col = ImLerp(it_anim->second.border_col, border_col, g.IO.DeltaTime * fAnimationSpeed);

    ImGui::RenderNavHighlight(bb, id);
    
    window->DrawList->AddImageRounded(user_texture_id, pos, pos + size, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255), style.FrameRounding);
    window->DrawList->AddRectFilled(pos, pos + size, ImGui::GetColorU32(it_anim->second.overlay_col), style.FrameRounding);
    window->DrawList->AddRect(pos, pos + size, ImGui::GetColorU32(it_anim->second.border_col), style.FrameRounding - style.FrameBorderSize, 0, style.FrameBorderSize);

    window->DrawList->AddText(pos + size / 2 - label_size / 2, ImGui::GetColorU32(it_anim->second.label_col), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::ButtonImage(ImTextureID user_texture_id, const char* str_id, const char* icon, ImVec4 icon_color, ImVec4 online_dot_color, bool online_dot, const ImVec2& size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(str_id);
    const ImVec2 icon_size = ImGui::CalcTextSize(icon, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, icon_size.x + style.FramePadding.x * 2.0f, icon_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    // Animate Colors
    static std::map<ImGuiID, ButtonImage_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, ButtonImage_State() });
        it_anim = anim.find(id);
    }

    ImVec4 overlay_rgba = ImGui::GetStyleColorVec4(ImGuiCol_TitleBgCollapsed);
    ImVec4 overlay_col = (hovered || held) ? overlay_rgba : ImVec4(overlay_rgba.x, overlay_rgba.y, overlay_rgba.z, 0.f);
    it_anim->second.overlay_col = ImLerp(it_anim->second.overlay_col, overlay_col, g.IO.DeltaTime * fAnimationSpeed);
    
    it_anim->second.dot_col = ImLerp(it_anim->second.dot_col, online_dot_color, g.IO.DeltaTime * fAnimationSpeed);

    ImVec4 icon_col = (hovered || held) ? icon_color : ImVec4(icon_color.x, icon_color.y, icon_color.z, 0.f);
    it_anim->second.icon_col = ImLerp(it_anim->second.icon_col, icon_col, g.IO.DeltaTime * fAnimationSpeed);

    ImGui::RenderNavHighlight(bb, id);

    float rounding = size.x > size.y ? size.x / 2 : size.y / 2;

    window->DrawList->AddShadowCircle(pos + size * 0.5, rounding, ImGui::GetColorU32(it_anim->second.icon_col), style.WindowShadowSize, ImVec2(0, 0));
    window->DrawList->AddImageRounded(user_texture_id, pos, pos + size, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255), rounding);
    window->DrawList->AddRectFilled(pos, pos + size, ImGui::GetColorU32(it_anim->second.overlay_col), rounding);
    window->DrawList->AddRect(pos, pos + size, ImGui::GetColorU32(it_anim->second.icon_col), rounding);
    window->DrawList->AddText(pos + size / 2 - icon_size / 2, ImGui::GetColorU32(it_anim->second.icon_col), icon);

    // Given data
    float X = pos.x + size.x / 2;   // Profile circle center X
    float Y = pos.y + size.y / 2;   // Profile circle center Y
    float R = rounding;             // Profile circle radius

    // Calculate the position of the green dot circle
    float angle = 0.25f * M_PI;             // 0.25 of a circle (pi/4 radians)
    float Xo = X + R * cos(angle);          // Green dot circle center X
    float Yo = Y - R * sin(angle);          // Green dot circle center Y
    float Ro = R / 4;                       // Green dot circle radius (adjust as needed)

    // Render the online dot circle
    window->DrawList->AddCircleFilled(ImVec2(Xo, Yo), Ro, ImGui::GetColorU32(it_anim->second.dot_col));

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

bool ImAdd::Selectable(const char* label, bool selected, const ImVec2& size_arg, ImDrawFlags draw_flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, 0);

    // Animate Colors
    static std::map<ImGuiID, Selectable_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, Selectable_State() });
        it_anim = anim.find(id);
    }

    it_anim->second.frame_col = ImLerp(it_anim->second.frame_col, selected ? ImGui::GetStyleColorVec4(ImGuiCol_HeaderActive) : (held && hovered) ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : hovered ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) : ImGui::GetStyleColorVec4(ImGuiCol_Button), g.IO.DeltaTime * fAnimationSpeed);
    it_anim->second.label_col = ImLerp(it_anim->second.label_col, selected ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : (held && hovered) ? ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled) : hovered ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), g.IO.DeltaTime * fAnimationSpeed);

    // Render
    const ImU32 frame_col = ImGui::GetColorU32(it_anim->second.frame_col);
    const ImU32 label_col = ImGui::GetColorU32(it_anim->second.label_col);
    ImGui::RenderNavHighlight(bb, id);

    if (selected)
        window->DrawList->AddRectFilled(bb.Min, bb.Max, frame_col, style.FrameRounding, draw_flags);

    if (g.LogEnabled)
        ImGui::LogSetNextTextDecoration("[", "]");

    ImGui::RenderText(pos + (size / 2) - (label_size / 2), label);

    // Automatically close popups
    //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
    //    CloseCurrentPopup();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed;
}

void ImAdd::SeparatorText(const char* text)
{
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    auto pos = ImGui::GetCursorScreenPos();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.62f, 0.63f, 1.00f));
    ImGui::Text(text);
    ImGui::PopStyleColor();

    float yOffset = static_cast<int>(ImGui::CalcTextSize(text).y / 2);
    bool bAjustThinkness = (ImGui::CalcTextSize(text).y / 2) - yOffset >= 0.5f ? true : false;

    ImGui::GetWindowDrawList()->AddLine(
        ImVec2(pos.x + ImGui::CalcTextSize(text).x + style.WindowPadding.x,
            pos.y + static_cast<int>(ImGui::CalcTextSize(text).y / 2)),
        ImVec2(pos.x + ImGui::GetWindowSize().x - style.WindowPadding.x * 2,
            pos.y + static_cast<int>(ImGui::CalcTextSize(text).y / 2)),
        ImGui::GetColorU32(ImGuiCol_Border),
        bAjustThinkness ? 2.f : 1.f);

}

bool ImAdd::BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    using namespace ImGui;

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

    ImVec2 pos = window->DC.CursorPos;
    const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : GetFrameHeight();
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const float width = ImGui::CalcItemSize(ImVec2(-0.1f, 0.f), 0.f, 0.f).x;
    ImVec2 r_pos = ImVec2(0, label_size.y + style.ItemInnerSpacing.y);

    const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(width, label_size.y + style.FramePadding.y * 2.0f));

    const ImRect total_bb(bb.Min, bb.Max + ImVec2(width, label_size.y + style.ItemInnerSpacing.y));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id, &bb))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(ImRect(total_bb.Min + r_pos, total_bb.Max), id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    // Animate Colors
    static std::map<ImGuiID, Combo_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, Combo_State() });
        it_anim = anim.find(id);
    }

    ImVec4 frame_col = GetStyleColorVec4(hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    it_anim->second.frame_col = ImLerp(it_anim->second.frame_col, frame_col, g.IO.DeltaTime * fAnimationSpeed);

    // Render shape
    const float value_x2 = ImMax(bb.Min.x, bb.Max.x - arrow_size);
    RenderNavHighlight(total_bb, id);

    if (!(flags & ImGuiComboFlags_NoPreview))
        window->DrawList->AddRectFilled(r_pos + bb.Min, r_pos + ImVec2(value_x2, bb.Max.y), GetColorU32(it_anim->second.frame_col), style.FrameRounding, (flags & ImGuiComboFlags_NoArrowButton) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersLeft);

    if (!(flags & ImGuiComboFlags_NoArrowButton))
    {
        ImVec4 bg_col = GetStyleColorVec4((popup_open || hovered) ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        it_anim->second.bg_col = ImLerp(it_anim->second.bg_col, bg_col, g.IO.DeltaTime * fAnimationSpeed);

        ImVec4 text_col = GetStyleColorVec4(ImGuiCol_Text);
        it_anim->second.text_col = ImLerp(it_anim->second.text_col, text_col, g.IO.DeltaTime * fAnimationSpeed);

        window->DrawList->AddRectFilled(r_pos + ImVec2(value_x2, bb.Min.y), r_pos + bb.Max, GetColorU32(it_anim->second.bg_col), style.FrameRounding, (width <= arrow_size) ? ImDrawFlags_RoundCornersAll : ImDrawFlags_RoundCornersRight);

        if (value_x2 + arrow_size - style.FramePadding.x <= bb.Max.x)
            RenderArrow(window->DrawList, r_pos + ImVec2(value_x2 + style.FramePadding.y, bb.Min.y + style.FramePadding.y), GetColorU32(it_anim->second.text_col), ImGuiDir_Down, 1.0f);
    }

    RenderFrameBorder(r_pos + bb.Min, r_pos + bb.Max, style.FrameRounding);

    // Custom preview
    if (flags & ImGuiComboFlags_CustomPreview)
    {
        g.ComboPreviewData.PreviewRect = ImRect(bb.Min.x, bb.Min.y, value_x2, bb.Max.y);
        IM_ASSERT(preview_value == NULL || preview_value[0] == 0);
        preview_value = NULL;
    }

    // Render preview and label
    if (preview_value != NULL && !(flags & ImGuiComboFlags_NoPreview))
    {
        if (g.LogEnabled)
            LogSetNextTextDecoration("{", "}");
        RenderTextClipped(r_pos + bb.Min + style.FramePadding, r_pos + ImVec2(value_x2, bb.Max.y), preview_value, NULL, NULL);
    }
    if (label_size.x > 0)
        RenderText(ImVec2(pos.x, bb.Min.y), label);

    if (!popup_open)
        return false;

    g.NextWindowData.Flags = backup_next_window_data_flags;
    return ImAdd::BeginComboPopup(popup_id, ImRect(bb.Min + r_pos, bb.Max + r_pos), flags);
}

bool ImAdd::BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags)
{
    using namespace ImGui;

    ImGuiContext& g = *GImGui;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    // Set popup size
    float w = bb.GetWidth();
    if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
        int popup_max_height_in_items = -1;
        if (flags & ImGuiComboFlags_HeightRegular)     popup_max_height_in_items = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)  popup_max_height_in_items = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)  popup_max_height_in_items = 20;
        ImVec2 constraint_min(0.0f, 0.0f), constraint_max(FLT_MAX, FLT_MAX);
        if ((g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.x <= 0.0f) // Don't apply constraints if user specified a size
            constraint_min.x = w;
        if ((g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.y <= 0.0f)
            constraint_max.y = CalcMaxPopupHeightFromItemCount(popup_max_height_in_items);
        SetNextWindowSizeConstraints(constraint_min, constraint_max);
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if (ImGuiWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, ImGuiPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar(ImGuiStyleVar_WindowPadding, g.Style.FramePadding); // Horizontally align ourselves with the framed text
    bool ret = Begin(name, NULL, window_flags);
    PopStyleVar();
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0);   // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    return true;
}

bool ImAdd::Combo(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int popup_max_height_in_items)
{
    using namespace ImGui;

    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        items_getter(data, *current_item, &preview_value);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!ImAdd::BeginCombo(label, preview_value, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        PushID(i);
        const bool item_selected = (i == *current_item);
        const char* item_text;
        if (!items_getter(data, i, &item_text))
            item_text = "*Unknown item*";

        PushStyleVar(ImGuiStyleVar_ItemSpacing, g.Style.FramePadding);
        if (ImAdd::Selectable(item_text, item_selected, ImVec2(-0.1, 0)) && *current_item != i)
        {
            value_changed = true;
            *current_item = i;
        }
        PopStyleVar();

        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

// Combo box helper allowing to pass an array of strings.
bool ImAdd::Combo(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    using namespace ImGui;

    const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}

// Combo box helper allowing to pass all items in a single string literal holding multiple zero-terminated items "item1\0item2\0"
bool ImAdd::Combo(const char* label, int* current_item, const char* items_separated_by_zeros, int height_in_items)
{
    using namespace ImGui;

    int items_count = 0;
    const char* p = items_separated_by_zeros;       // FIXME-OPT: Avoid computing this, or at least only when combo is open
    while (*p)
    {
        p += strlen(p) + 1;
        items_count++;
    }
    bool value_changed = Combo(label, current_item, Items_SingleStringGetter, (void*)items_separated_by_zeros, items_count, height_in_items);
    return value_changed;
}

bool ImAdd::InputText(const char* label, const char* icon, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImVec2 pos = window->DC.CursorPos;
    const float width = ImGui::CalcItemSize(ImVec2(-0.1f, 0.f), 0.f, 0.f).x;

    window->DrawList->AddRectFilled(pos + ImVec2(width - ImGui::GetFrameHeight(), ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y), pos + ImVec2(width, ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y + ImGui::GetFrameHeight()), ImGui::GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, ImDrawFlags_RoundCornersRight);
    window->DrawList->AddRectFilled(pos + ImVec2(0, ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y), pos + ImVec2(width - ImGui::GetFrameHeight(), ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y + ImGui::GetFrameHeight()), ImGui::GetColorU32(ImGuiCol_FrameBg), g.Style.FrameRounding, ImDrawFlags_RoundCornersLeft);

    // Renders
    if (strlen(icon))
    {
        ImVec2 icon_size = ImGui::CalcTextSize(icon);
        window->DrawList->AddText(pos + ImVec2(width - ImGui::GetFrameHeight() + (ImGui::GetFrameHeight() / 2 - icon_size.x / 2) + 1, ImGui::GetFontSize() + g.Style.ItemInnerSpacing.y + (ImGui::GetFrameHeight() / 2 - icon_size.y / 2) + 1), ImGui::GetColorU32(ImGuiCol_WindowBg), icon);
    }

    // Input Widget
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(g.Style.ItemSpacing.x, g.Style.ItemInnerSpacing.y));
    ImGui::TextDisabled(label);
    IM_ASSERT(!(flags & ImGuiInputTextFlags_Multiline)); // call InputTextMultiline()
    ImGui::PushItemWidth(width - ImGui::GetFrameHeight());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));
    auto result = ImGui::InputTextEx(std::string("##Input_" + std::string(label)).c_str(), NULL, buf, (int)buf_size, ImVec2(0, 0), flags, callback, user_data);
    ImGui::PopStyleColor();
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();

    return result;
}

void ImAdd::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding, ImDrawFlags draw_flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding, draw_flags);
    const float border_size = g.Style.FrameBorderSize;
    if (border && border_size > 0.0f)
    {
        window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), ImGui::GetColorU32(ImGuiCol_BorderShadow), rounding, draw_flags, border_size);
        window->DrawList->AddRect(p_min, p_max, ImGui::GetColorU32(ImGuiCol_Border), rounding, draw_flags, border_size);
    }
}

bool ImAdd::CloseButton(ImGuiID id, const ImVec2& pos)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    // Tweak 1: Shrink hit-testing area if button covers an abnormally large proportion of the visible region. That's in order to facilitate moving the window away. (#3825)
    // This may better be applied as a general hit-rect reduction mechanism for all widgets to ensure the area to move window is always accessible?
    const ImRect bb(pos, pos + ImVec2(g.FontSize, g.FontSize) + g.Style.FramePadding * 2.0f);
    ImRect bb_interact = bb;
    const float area_to_visible_ratio = window->OuterRectClipped.GetArea() / bb.GetArea();
    if (area_to_visible_ratio < 1.5f)
        bb_interact.Expand(ImFloor(bb_interact.GetSize() * -0.25f));

    // Tweak 2: We intentionally allow interaction when clipped so that a mechanical Alt,Right,Activate sequence can always close a window.
    // (this isn't the regular behavior of buttons, but it doesn't affect the user much because navigation tends to keep items visible).
    bool is_clipped = !ImGui::ItemAdd(bb_interact, id);

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb_interact, id, &hovered, &held);
    if (is_clipped)
        return pressed;
    
    // Animate Colors
    static std::map<ImGuiID, Button_State> anim;
    auto it_anim = anim.find(id);

    if (it_anim == anim.end())
    {
        anim.insert({ id, Button_State() });
        it_anim = anim.find(id);
    }

    ImVec4 frame_col = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImVec4 frame_rgb = ImVec4(frame_col.x, frame_col.y, frame_col.z, 0.0f);
    it_anim->second.frame_col = ImLerp(it_anim->second.frame_col, (held && hovered) ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive) : hovered ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) : frame_rgb, g.IO.DeltaTime * fAnimationSpeed);

    // Render
    const ImU32 col = ImGui::GetColorU32(it_anim->second.frame_col);

    ImVec2 center = bb.GetCenter();
    window->DrawList->AddCircleFilled(center, ImMax(2.0f, g.FontSize * 0.5f + 1.0f), col, g.FontSize);

    float cross_extent = g.FontSize * 0.5f * 0.7071f - 1.0f;
    ImU32 cross_col = ImGui::GetColorU32(ImGuiCol_Text);
    center -= ImVec2(0.5f, 0.5f);
    window->DrawList->AddLine(center + ImVec2(+cross_extent, +cross_extent), center + ImVec2(-cross_extent, -cross_extent), cross_col, 1.0f);
    window->DrawList->AddLine(center + ImVec2(+cross_extent, -cross_extent), center + ImVec2(-cross_extent, +cross_extent), cross_col, 1.0f);

    return pressed;
}

void ImAdd::TabBox(const char* str_id, int* current_id, std::vector<const char*> list, ImVec2 size_arg)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems || list.size() <= 0)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGui::GetStyleColorVec4(ImGuiCol_Border));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, style.FrameBorderSize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, style.FrameRounding);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, style.FrameRounding);

    ImGui::BeginChild(str_id, size_arg, false);
    {
        float width = ImGui::GetWindowSize().x / list.size();

        //ImGui::GetWindowDrawList()->AddRect(ImGui::GetWindowPos(), ImGui::GetWindowPos() + ImGui::GetWindowSize(), ImGui::GetColorU32(ImGuiCol_Border), style.ChildRounding, 0, style.FrameBorderSize);

        for (int i = 0; i < list.size(); ++i)
        {
            ImDrawFlags draw_flags = i == 0 ? ImDrawFlags_RoundCornersLeft : i == list.size() - 1 ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone;
            bool selected = *current_id == i;

            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(selected ? ImGuiCol_TextDisabled : ImGuiCol_Text));
            if (ImAdd::Selectable(list[i], selected, ImVec2(width, size_arg.y), draw_flags))
            {
                *current_id = i;
            }
            ImGui::PopStyleColor();
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(5);
    ImGui::PopStyleColor();
    ImGui::Spacing();
}

void ImAdd::Spinner(float radius, float thickness, int num_segments, ComVec4 color)
{
    using namespace ImGui;

    auto window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    auto& g = *GImGui;
    const auto& style = g.Style;
    auto&& pos = ImGui::GetCursorPos();
    ImVec2 size{ radius * 2, radius * 2 };
    const ImRect bb{ pos, pos + size };
    ItemSize(bb);
    if (!ItemAdd(bb, 0))
        return;

    window->DrawList->PathClear();
    int start = static_cast<int>(abs(ImSin(static_cast<float>(g.Time * 1.8f)) * (num_segments - 5)));
    const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
    const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;
    const auto&& centre = pos + ImVec2(radius, radius);
    for (auto i = 0; i < num_segments; i++) {
        const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
        auto time = static_cast<float>(g.Time);
        window->DrawList->PathLineTo({ centre.x + ImCos(a + time * 8) * radius,
                                      centre.y + ImSin(a + time * 8) * radius });
    }
    window->DrawList->PathStroke(GetColorU32(color), false, thickness);
}