#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3d11.h>
#include <string>

#include <dwmapi.h>

#include "render.hpp"

#include "../game/game.hpp"

#include "../../dependencies/font.hpp"
#include "../../dependencies/framework/backend/imgui_settings.h"
#include "../../dependencies/framework/backend/Fonts.h"

float color_edit[4];

static int iTabs = 0;
static int iSubTabs = 0;

HWND window_handle;

ID3D11Device* d3d_device;
ID3D11DeviceContext* d3d_device_ctx;
IDXGISwapChain* d3d_swap_chain;
ID3D11RenderTargetView* d3d_render_target;
D3DPRESENT_PARAMETERS d3d_present_params;

auto n_render::c_render::imgui() -> bool {

    DXGI_SWAP_CHAIN_DESC swap_chain_description;
    ZeroMemory(&swap_chain_description, sizeof(swap_chain_description));
    swap_chain_description.BufferCount = 2;
    swap_chain_description.BufferDesc.Width = 0;
    swap_chain_description.BufferDesc.Height = 0;
    swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
    swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
    swap_chain_description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_description.OutputWindow = window_handle;
    swap_chain_description.SampleDesc.Count = 1;
    swap_chain_description.SampleDesc.Quality = 0;
    swap_chain_description.Windowed = 1;
    swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL d3d_feature_lvl;

    const D3D_FEATURE_LEVEL d3d_feature_array[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, d3d_feature_array, 2, D3D11_SDK_VERSION, &swap_chain_description, &d3d_swap_chain, &d3d_device, &d3d_feature_lvl, &d3d_device_ctx);

    ID3D11Texture2D* pBackBuffer;

    d3d_swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

    d3d_device->CreateRenderTargetView(pBackBuffer, NULL, &d3d_render_target);

    pBackBuffer->Release();

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = NULL;

    (void)io;

    io.Fonts->AddFontFromMemoryTTF(&InterMedium, sizeof InterMedium, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());

    bold_font = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 17, NULL, io.Fonts->GetGlyphRangesCyrillic());

    tab_font = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 15, NULL, io.Fonts->GetGlyphRangesCyrillic());

    small_font = io.Fonts->AddFontFromMemoryTTF(&InterSemiBold, sizeof InterSemiBold, 12, NULL, io.Fonts->GetGlyphRangesCyrillic());

    icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 16, NULL, io.Fonts->GetGlyphRangesCyrillic());

    big_icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());

    logo_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 39, NULL, io.Fonts->GetGlyphRangesCyrillic());

    small_icon_font = io.Fonts->AddFontFromMemoryTTF(&icomoon, sizeof icomoon, 12, NULL, io.Fonts->GetGlyphRangesCyrillic());

    arrow_icons = io.Fonts->AddFontFromMemoryTTF(&arrowicon, sizeof arrowicon, 18, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ImGui_ImplWin32_Init(window_handle);

    ImGui_ImplDX11_Init(d3d_device, d3d_device_ctx);

    d3d_device->Release();

    return true;

}

auto n_render::c_render::hijack() -> bool {
    window_handle = FindWindowA(skCrypt("MedalOverlayClass"), skCrypt("MedalOverlay"));
    if (!window_handle) return false;
    MARGINS Margin = { -1 };
    DwmExtendFrameIntoClientArea(window_handle, &Margin);
    SetWindowPos(window_handle, 0, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
    ShowWindow(window_handle, SW_SHOW);
    UpdateWindow(window_handle);
    return true;

}

float pos = 40;
int item = 0;
bool bSelectingKey = 0;
int vis_color = 3;
int invis_color = 0;

auto key_picker(int *key) -> void {
    if (bSelectingKey) {

        if (GetAsyncKeyState(VK_RBUTTON) & 1) {
            *key = VK_RBUTTON;
            key_string = skCrypt("Right Click");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_LBUTTON) & 1) {
            *key = VK_LBUTTON;
            key_string = skCrypt("Left Click");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_MBUTTON) & 1) {
            *key = VK_MBUTTON;
            key_string = skCrypt("Middle Click");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_XBUTTON1) & 1) {
            *key = VK_XBUTTON1;
            key_string = skCrypt("Side Button 1");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_XBUTTON2) & 1) {
            *key = VK_XBUTTON2;
            key_string = skCrypt("Side Button 2");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_TAB) & 1) {
            *key = VK_TAB;
            key_string = skCrypt("Tab");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_CAPITAL) & 1) {
            *key = VK_CAPITAL;
            key_string = skCrypt("Capital");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_LSHIFT) & 1) {
            *key = VK_LSHIFT;
            key_string = skCrypt("Left Shift");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_LCONTROL) & 1) {
            *key = VK_LCONTROL;
            key_string = skCrypt("Left Control");
            bSelectingKey = 0;
        }
        else if (GetAsyncKeyState(VK_LMENU) & 1) {
            *key = VK_LMENU;
            key_string = skCrypt("Left Alt");
            bSelectingKey = 0;
        }
    }
}
inline static int keystatus = 0;

inline static int realkey = 0;

inline static int realkey1 = 0;
inline static int keystatus1 = 0;
inline static int keystatus2 = 0;
inline static int keystatus3 = 0;
inline static int keystatus4 = 0;
char* name1;

inline int aimkey = 0;
inline bool GetKey(int key)
{
    realkey = key;
    return true;
}
inline void ChangeKey(void* blank)
{
    keystatus = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetAsyncKeyState(i) & 0x8000)
            {
                globals::aimbot_keyz = i;
                keystatus = 0;
                return;
            }
        }
    }
}

inline bool GetKey1(int key)
{
    realkey1 = key;
    return true;
}
inline void ChangeKey1(void* blank)
{
    keystatus1 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetAsyncKeyState(i) & 0x8000)
            {
                keystatus1 = 0;
                return;
            }
        }
    }
}

inline bool GetKey2(int key)
{
    realkey1 = key;
    return true;
}
inline void ChangeKey2(void* blank)
{
    keystatus2 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetAsyncKeyState(i) & 0x8000)
            {
                keystatus2 = 0;
                return;
            }
        }
    }
}

inline void ChangeKey3(void* blank)
{
    keystatus3 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetAsyncKeyState(i) & 0x8000)
            {
                keystatus3 = 0;
                return;
            }
        }
    }
}

inline void ChangeKey4(void* blank)
{
    keystatus4 = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetAsyncKeyState(i) & 0x8000)
            {
                keystatus4 = 0;
                return;
            }
        }
    }
}

inline void DrawBox(float X, float Y, float W, float H, const ImU32& color, int thickness)
{
    ImGui::GetForegroundDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

static const char* keyNames[] =
{
    "Keybind",
    "Left Mouse",
    "Right Mouse",
    "Cancel",
    "Middle Mouse",
    "Mouse 5",
    "Mouse 4",
    "",
    "Backspace",
    "Tab",
    "",
    "",
    "Clear",
    "Enter",
    "",
    "",
    "Shift",
    "Control",
    "Alt",
    "Pause",
    "Caps",
    "",
    "",
    "",
    "",
    "",
    "",
    "Escape",
    "",
    "",
    "",
    "",
    "Space",
    "Page Up",
    "Page Down",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "",
    "",
    "",
    "Print",
    "Insert",
    "Delete",
    "",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "",
    "",
    "",
    "",
    "",
    "Numpad 0",
    "Numpad 1",
    "Numpad 2",
    "Numpad 3",
    "Numpad 4",
    "Numpad 5",
    "Numpad 6",
    "Numpad 7",
    "Numpad 8",
    "Numpad 9",
    "Multiply",
    "Add",
    "",
    "Subtract",
    "Decimal",
    "Divide",
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
};
static const char* KeyNames1[] = {
	"Aimbot Bot Key",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};

std::string congratsmen = "Hus X Veru";

static const char* KeyNames[] = {
	"Trigger Bot Key",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"VK_TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"VK_SHIFT",
	"VK_CONTROL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"VK_INSERT",
	"VK_DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"VK_NUMPAD0",
	"VK_NUMPAD1",
	"VK_NUMPAD2",
	"VK_NUMPAD3",
	"VK_NUMPAD4",
	"VK_NUMPAD5",
	"VK_NUMPAD6",
	"VK_NUMPAD7",
	"VK_NUMPAD8",
	"VK_NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"VK_F1",
	"VK_F2",
	"VK_F3",
	"VK_F4",
	"VK_F5",
	"VK_F6",
	"VK_F7",
	"VK_F8",
	"VK_F9",
	"VK_F10",
	"VK_F11",
	"VK_F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_LSHIFT",
	"VK_RSHIFT",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU"
};
static const int KeyCodes[] = {
	0x0,  //Undefined
	0x01,
	0x02,
	0x03,
	0x04,
	0x05,
	0x06,
	0x07, //Undefined
	0x08,
	0x09,
	0x0A, //Reserved
	0x0B, //Reserved
	0x0C,
	0x0D,
	0x0E, //Undefined
	0x0F, //Undefined
	0x10,
	0x11,
	0x12,
	0x13,
	0x14,
	0x15,
	0x16, //IME On
	0x17,
	0x18,
	0x19,
	0x1A, //IME Off
	0x1B,
	0x1C,
	0x1D,
	0x1E,
	0x1F,
	0x20,
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x26,
	0x27,
	0x28,
	0x29,
	0x2A,
	0x2B,
	0x2C,
	0x2D,
	0x2E,
	0x2F,
	0x30,
	0x31,
	0x32,
	0x33,
	0x34,
	0x35,
	0x36,
	0x37,
	0x38,
	0x39,
	0x3A, //Undefined
	0x3B, //Undefined
	0x3C, //Undefined
	0x3D, //Undefined
	0x3E, //Undefined
	0x3F, //Undefined
	0x40, //Undefined
	0x41,
	0x42,
	0x43,
	0x44,
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4A,
	0x4B,
	0x4C,
	0x4B,
	0x4E,
	0x4F,
	0x50,
	0x51,
	0x52,
	0x53,
	0x54,
	0x55,
	0x56,
	0x57,
	0x58,
	0x59,
	0x5A,
	0x5B,
	0x5C,
	0x5D,
	0x5E, //Rservered
	0x5F,
	0x60, //Numpad1
	0x61, //Numpad2
	0x62, //Numpad3
	0x63, //Numpad4
	0x64, //Numpad5
	0x65, //Numpad6
	0x66, //Numpad7
	0x67, //Numpad8
	0x68, //Numpad8
	0x69, //Numpad9
	0x6A,
	0x6B,
	0x6C,
	0x6D,
	0x6E,
	0x6F,
	0x70, //F1
	0x71, //F2
	0x72, //F3
	0x73, //F4
	0x74, //F5
	0x75, //F6
	0x76, //F7
	0x77, //F8
	0x78, //F9
	0x79, //F10
	0x7A, //F11
	0x7B, //F12
	0x7C, //F13
	0x7D, //F14
	0x7E, //F15
	0x7F, //F16
	0x80, //F17
	0x81, //F18
	0x82, //F19
	0x83, //F20
	0x84, //F21
	0x85, //F22
	0x86, //F23
	0x87, //F24
	0x88, //Unkown
	0x89, //Unkown
	0x8A, //Unkown
	0x8B, //Unkown
	0x8C, //Unkown
	0x8D, //Unkown
	0x8E, //Unkown
	0x8F, //Unkown
	0x90,
	0x91,
	0x92, //OEM Specific
	0x93, //OEM Specific
	0x94, //OEM Specific
	0x95, //OEM Specific
	0x96, //OEM Specific
	0x97, //Unkown
	0x98, //Unkown
	0x99, //Unkown
	0x9A, //Unkown
	0x9B, //Unkown
	0x9C, //Unkown
	0x9D, //Unkown
	0x9E, //Unkown
	0x9F, //Unkown
	0xA0,
	0xA1,
	0xA2,
	0xA3,
	0xA4,
	0xA5
};

float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}

float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}
void HotKey(int* k, const ImVec2& size_arg = ImVec2(0, 0))
{
	static bool waitingforkey = false;
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames1[*(int*)k], size_arg))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("...", size_arg);
		Sleep(50);
		for (auto& Key : KeyCodes)
		{
			if (GetAsyncKeyState(Key)) {
				*(int*)k = Key;
				waitingforkey = false;
			}
		}
	}
}

void HotKey2(int* k, const ImVec2& size_arg = ImVec2(0, 0))
{
	static bool waitingforkey = false;
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames[*(int*)k], size_arg))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("...", size_arg);
		Sleep(50);
		for (auto& Key : KeyCodes)
		{
			if (GetAsyncKeyState(Key)) {
				*(int*)k = Key;
				waitingforkey = false;
			}
		}
	}
}

void HotKeytrig(int* k, const ImVec2& size_arg = ImVec2(0, 0))
{
	static bool waitingforkey = false;
	if (waitingforkey == false) {
		if (ImGui::Button(KeyNames[*(int*)k], size_arg))
			waitingforkey = true;
	}
	else if (waitingforkey == true) {
		ImGui::Button("...", size_arg);
		Sleep(50);
		for (auto& Key : KeyCodes)
		{
			if (GetAsyncKeyState(Key)) {
				*(int*)k = Key;
				waitingforkey = false;
			}
		}
	}
}
inline static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}
inline void HotkeyButton(int aimkey, void* changekey, int status)
{
    const char* preview_value = NULL;
    if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
        Items_ArrayGetter(keyNames, aimkey, &preview_value);
    std::string aimkeys;
    if (preview_value == NULL)
        aimkeys = (("Select Key"));
    else
        aimkeys = preview_value;

    if (status == 1)
    {
        aimkeys = (("Press the Key"));
    }
    if (ImGui::Button(aimkeys.c_str(), ImVec2(200, 30)))
    {
        if (status == 0)
        {
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
        }
    }
}

auto menu() -> void {
    if (globals::render_menu)
    {

        ImGuiStyle& s = ImGui::GetStyle();

        s.FramePadding = ImVec2(10, 10);
        s.ItemSpacing = ImVec2(10, 10);
        s.FrameRounding = 4.f;
        s.WindowRounding = 10.f;
        s.WindowBorderSize = 0.f;
        s.PopupBorderSize = 0.f;
        s.WindowPadding = ImVec2(0, 0);
        s.ChildBorderSize = 0.0001f;
        s.Colors[ImGuiCol_WindowBg] = winbg_color;
        s.Colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 0.f);
        s.WindowShadowSize = 0;
        s.PopupRounding = 5.f;
        s.PopupBorderSize = 1.3f;
        s.ScrollbarSize = 5.f;
        s.ScrollbarRounding = 10.f;

        ImGui::SetNextWindowSize(ImVec2(666, 450));
        ImGui::Begin("General", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
        {
            anim_speed = ImGui::GetIO().DeltaTime * 8.f;

            auto draw = ImGui::GetWindowDrawList();
            const auto& p = ImGui::GetWindowPos();
            draw->AddRectFilled(p, p + ImVec2(666, 450), winbg_color, 10.f);

            ImGui::SetCursorPos(ImVec2(10, 13));
            ImGui::BeginChild("Tabs", ImVec2(640, 30), true); {
                ImGui::Tab("Aimbot", "6", &iTabs, 0); ImGui::SameLine();
                ImGui::Tab("Visuals", "1", &iTabs, 3); ImGui::SameLine();
                ImGui::Tab("Settings", "3", &iTabs, 5); ImGui::SameLine();
            }
            ImGui::EndChild(false);

            ImGui::SetCursorPos(ImVec2(10, 54));

            // Render content based on selected tab
            if (iTabs == 0) { // Aimbot tab
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild("Softaim", ImVec2(318, 250), false, 0, "5"); 
                    {
                        ImGui::Checkbox("Enable", &globals::aimbot);
                        ImGui::Checkbox("Prediction", &globals::prediction);
                        ImGui::Checkbox("Visibility Check", &globals::visible);
                        ImGui::Checkbox("Draw Crosshair", &globals::render_crosshair);
                        ImGui::SliderInt("Aimbot Smooth", &globals::aimbot_smooth, 5, 20);
                        ImGui::SliderInt("Aimbot FOV", &globals::aimbot_fov, 60, 200);
                    }
                    ImGui::EndChild(true);
                }
                ImGui::EndGroup();
                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    ImGui::BeginChild("Target Area", ImVec2(318, 150), false, 0, "4"); 
                    {
                        const char* hitboxNames[] = { "Head", "Neck", "Chest", "Pelvis" };
                        ImGui::Combo("Hitbox", &globals::hitbox, hitboxNames, IM_ARRAYSIZE(hitboxNames));
                        if (globals::hitbox >= 0 && globals::hitbox < IM_ARRAYSIZE(hitboxNames)) {
                            ImVec2 hbsize = ImGui::CalcTextSize(hitboxNames[globals::hitbox]);
                            ImGui::SetCursorPos({ 195 - hbsize.x, 140 });
                            ImGui::Text(hitboxNames[globals::hitbox]);
                        }
                        HotkeyButton(globals::aimbot_keyz, ChangeKey, keystatus);
                    }
                    ImGui::EndChild(true);
                }
                ImGui::EndGroup();
            }
            else if (iTabs == 3) { // Visuals tab
                ImGui::BeginGroup();
                {
                    ImGui::BeginChild("Players", ImVec2(318, 250), false, 3, "5"); {
                        ImGui::Checkbox("Static Box", &globals::static_box);
                        ImGui::Checkbox("Player Rank", &globals::rank);
                        ImGui::Checkbox("Skeleton", &globals::skeleton);
                        ImGui::Checkbox("Distance", &globals::distance);
                        ImGui::Checkbox("Snaplines", &globals::snapline);
                        ImGui::Checkbox("Platform", &globals::weapon);
                    }
                    ImGui::EndChild(true);
                }
                ImGui::EndGroup();
                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    ImGui::BeginChild("Radar", ImVec2(318, 150), false, 3, "4");
                    {
                        ImGui::Checkbox("Enable Radar", &globals::radar);
                    }
                    ImGui::EndChild(true);
                }
                ImGui::EndGroup();
            }
            else if (iTabs == 5) {
                ImGui::BeginGroup();
                {
                    ImGui::Text("Settings");
                }
                ImGui::EndGroup();
            }

            if (!ImGui::IsAnyItemHovered())
                disable_scroll = false;
        }
        ImGui::End();
    }
}


auto draw() -> void {

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.f);

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_END)) exit(0);

    game->actorloop();

    std::string null = "";
    std::string str = null + (std::string)skCrypt("Rendering ") + std::to_string(p_list.size()) + (std::string)skCrypt(" Enemies");
    ImVec2 text_size = ImGui::CalcTextSize(str.c_str());
    ImGui::GetBackgroundDrawList()->AddText(ImVec2((globals::screen_width / 2) - (text_size.x / 2), 50), ImColor(255, 255, 255), str.c_str());
    ImGui::GetBackgroundDrawList()->AddText(ImVec2((globals::screen_width / 2) - (text_size.x / 2), 50), ImColor(255, 255, 255), str.c_str());


    if (globals::aimbot_fov) {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(globals::screen_width / 2, globals::screen_height / 2), globals::aimbot_fov * 10, ImColor(0, 0, 0), 64, 3);
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(globals::screen_width / 2, globals::screen_height / 2), globals::aimbot_fov * 10, ImColor(255, 255, 255), 64, 1);
    }

    if (globals::render_crosshair) {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2((globals::screen_width / 2) - 6, globals::screen_height / 2), ImVec2((globals::screen_width / 2) +  7, globals::screen_height / 2), ImColor(255, 255, 255), 2);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(globals::screen_width / 2, (globals::screen_height / 2) - 6), ImVec2(globals::screen_width / 2, (globals::screen_height / 2) + 7), ImColor(255, 255, 255), 2);
    }

    if (globals::render_menu) menu();

    if (GetAsyncKeyState(VK_RSHIFT) & 1)
        globals::render_menu = !globals::render_menu;

    ImGui::Render();
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    d3d_device_ctx->OMSetRenderTargets(1, &d3d_render_target, nullptr);
    d3d_device_ctx->ClearRenderTargetView(d3d_render_target, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    d3d_swap_chain->Present(1, 0);

}

auto n_render::c_render::render() -> bool {

    static RECT rect_og;
    MSG msg = { NULL };
    ZeroMemory(&msg, sizeof(MSG));

    while (msg.message != WM_QUIT)
    {

        UpdateWindow(window_handle);
        ShowWindow(window_handle, SW_SHOW);

        if (PeekMessageA(&msg, window_handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ImGuiIO& io = ImGui::GetIO();
        io.ImeWindowHandle = window_handle;
        io.DeltaTime = 1.0f / 60.0f;


        POINT p_cursor;
        GetCursorPos(&p_cursor);
        io.MousePos.x = p_cursor.x;
        io.MousePos.y = p_cursor.y;

        if (GetAsyncKeyState(VK_LBUTTON)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].x = io.MousePos.y;
        }
        else
            io.MouseDown[0] = false;

        draw();

    }
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    DestroyWindow(window_handle);

    return true;

}