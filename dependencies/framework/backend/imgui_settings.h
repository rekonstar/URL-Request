#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include "C:\Users\rootste\source\repos\demon_services\dependencies\framework\imgui_internal.h"

#include <string>

inline ImColor main_color(163, 149, 216, 255);

inline ImColor text_color[2] = {ImColor(221, 221, 221, 255), ImColor(86, 82, 97, 255) };

inline ImColor background_color(50, 44, 67, 153);

inline ImColor second_color(250, 253, 258, 255);

inline ImColor stroke_color(86, 88, 97, 50);

inline ImColor child_color(19, 19, 19, 255);

inline ImColor scroll_bg_col(50, 44, 67, 255);

inline ImColor winbg_color(13, 10, 17, 255);

inline ImFont* bold_font;

inline ImFont* icon_font;

inline ImFont* tab_font;

inline ImFont* big_icon_font;

inline ImFont* logo_font;

inline ImFont* small_font;

inline ImFont* small_icon_font;

inline ImFont* arrow_icons;

inline ImVec2 frame_size(298, 20);

inline float anim_speed = 8.f;

inline bool disable_scroll = false;

inline float ImDegToRad(float degrees)
{
    static const float deg_to_rad = 0.01745329251994329576923690768489f;
    return degrees * deg_to_rad;
}

inline ImColor GetColorWithAlpha(ImColor color, float alpha)
{
return ImColor(color.Value.x, color.Value.y, color.Value.z, alpha);
}

inline ImVec2 center_text(ImVec2 min, ImVec2 max, const char* text)
{
    return min + (max - min) / 2 - ImGui::CalcTextSize(text) / 2;
}

inline int rotation_start_index;
inline void ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}

inline ImVec2 ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}

inline ImVec4 ImColorToImVec4(const ImColor& color)
{
    return ImVec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w);
}

static float asd = 0;

inline void Pickerbox(std::string label, bool* v, float col[4])
{
    std::string picker_name = "##picker" + label;

    asd += ImGui::GetIO().MouseWheel;

    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-20, -10));
    ImGui::ColorEdit4(picker_name.c_str(), (float*)col, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, 30));
    ImGui::Checkbox(label.c_str(), v);
}

inline void Keybindbox(std::string label, bool* v, int* key)
{
    std::string picker_name = "##keybind" + label;

    frame_size.x = 200;
    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, 0));
    ImGui::Checkbox(label.c_str(), v);
    frame_size.x = 298;
    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, 30));
    ImGui::Keybind(picker_name.c_str(), key);
}


inline void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter())
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}

inline void rect_glow(ImDrawList* draw, ImVec2 start, ImVec2 end, ImColor col, float rounding, float intensity) {
    while (true) {
        if (col.Value.w < 0.0019f)
            break;

        draw->AddRectFilled(start, end, col, rounding);

        col.Value.w -= col.Value.w / intensity;
        start = ImVec2(start.x - 1, start.y - 1);
        end = ImVec2(end.x + 1, end.y + 1);
    }
}

