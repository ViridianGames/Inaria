#ifndef _INARIACOMPAT_H_
#define _INARIACOMPAT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "Geist/Globals.h"
#include "Geist/InputSystem.h"
#include "Geist/SoundSystem.h"
#include "Geist/Config.h"
#include "raylib.h"

// Resolution scale: legacy 640x480 -> 480x270
constexpr float SX = 480.0f / 640.0f;
constexpr float SY = 270.0f / 480.0f;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

using uint32_t = unsigned int;
using DWORD = uint32_t;
using UINT = uint32_t;

#define SDL_GetTicks() GetTicks()

// SDL keycode -> Raylib KEY_* shims
#define SDLK_UNKNOWN KEY_NULL
#define SDLK_RETURN KEY_ENTER
#define SDLK_ESCAPE KEY_ESCAPE
#define SDLK_SPACE KEY_SPACE
#define SDLK_a KEY_A
#define SDLK_b KEY_B
#define SDLK_c KEY_C
#define SDLK_d KEY_D
#define SDLK_e KEY_E
#define SDLK_f KEY_F
#define SDLK_g KEY_G
#define SDLK_h KEY_H
#define SDLK_i KEY_I
#define SDLK_j KEY_J
#define SDLK_k KEY_K
#define SDLK_l KEY_L
#define SDLK_m KEY_M
#define SDLK_n KEY_N
#define SDLK_o KEY_O
#define SDLK_p KEY_P
#define SDLK_q KEY_Q
#define SDLK_r KEY_R
#define SDLK_s KEY_S
#define SDLK_t KEY_T
#define SDLK_u KEY_U
#define SDLK_v KEY_V
#define SDLK_w KEY_W
#define SDLK_x KEY_X
#define SDLK_y KEY_Y
#define SDLK_z KEY_Z
#define SDLK_0 KEY_ZERO
#define SDLK_1 KEY_ONE
#define SDLK_2 KEY_TWO
#define SDLK_3 KEY_THREE
#define SDLK_4 KEY_FOUR
#define SDLK_5 KEY_FIVE
#define SDLK_6 KEY_SIX
#define SDLK_7 KEY_SEVEN
#define SDLK_8 KEY_EIGHT
#define SDLK_9 KEY_NINE
#define SDLK_UP KEY_UP
#define SDLK_DOWN KEY_DOWN
#define SDLK_LEFT KEY_LEFT
#define SDLK_RIGHT KEY_RIGHT
#define SDLK_LSHIFT KEY_LEFT_SHIFT
#define SDLK_RSHIFT KEY_RIGHT_SHIFT
#define SDLK_LCTRL KEY_LEFT_CONTROL
#define SDLK_RCTRL KEY_RIGHT_CONTROL
#define SDLK_LALT KEY_LEFT_ALT
#define SDLK_RALT KEY_RIGHT_ALT
#define SDLK_TAB KEY_TAB
#define SDLK_BACKSPACE KEY_BACKSPACE
#define SDLK_DELETE KEY_DELETE
#define SDLK_HOME KEY_HOME
#define SDLK_END KEY_END
#define SDLK_PAGEUP KEY_PAGE_UP
#define SDLK_PAGEDOWN KEY_PAGE_DOWN
#define SDLK_F1 KEY_F1
#define SDLK_F2 KEY_F2
#define SDLK_F3 KEY_F3
#define SDLK_F4 KEY_F4
#define SDLK_F5 KEY_F5
#define SDLK_F6 KEY_F6
#define SDLK_F7 KEY_F7
#define SDLK_F8 KEY_F8
#define SDLK_F9 KEY_F9
#define SDLK_F10 KEY_F10
#define SDLK_F11 KEY_F11
#define SDLK_F12 KEY_F12
#define SDLK_KP_0 KEY_KP_0
#define SDLK_KP_1 KEY_KP_1
#define SDLK_KP_2 KEY_KP_2
#define SDLK_KP_3 KEY_KP_3
#define SDLK_KP_4 KEY_KP_4
#define SDLK_KP_5 KEY_KP_5
#define SDLK_KP_6 KEY_KP_6
#define SDLK_KP_7 KEY_KP_7
#define SDLK_KP_8 KEY_KP_8
#define SDLK_KP_9 KEY_KP_9
#define SDLK_KP_ENTER KEY_KP_ENTER
#define SDLK_KP_PLUS KEY_KP_ADD
#define SDLK_KP_MINUS KEY_KP_SUBTRACT
#define SDLK_KP1 KEY_KP_1
#define SDLK_KP2 KEY_KP_2
#define SDLK_KP3 KEY_KP_3
#define SDLK_KP4 KEY_KP_4
#define SDLK_KP5 KEY_KP_5
#define SDLK_KP6 KEY_KP_6
#define SDLK_KP7 KEY_KP_7
#define SDLK_KP8 KEY_KP_8
#define SDLK_KP9 KEY_KP_9
#define SDLK_RETURN KEY_ENTER
#define SDLK_LCTRL KEY_LEFT_CONTROL

inline uint32_t GetTicks()
{
    return static_cast<uint32_t>(GetTime() * 1000.0);
}

inline int SdlKeyToRaylib(int sdlKey)
{
    return sdlKey;
}

struct LegacyMeshStub
{
    void* data = nullptr;
};

class LegacySprite
{
public:
    Texture* m_Texture = nullptr;
    int m_PosX = 0;
    int m_PosY = 0;
    int m_Width = 0;
    int m_Height = 0;
};

class InariaFont
{
public:
    InariaFont() = default;

    void Init(const std::string& fontPath, int fontSize = 0);
    void SetFixedWidth() { m_FixedWidth = true; }

    void DrawText(const std::string& text, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    void DrawTextA(const char* text, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    void DrawTextA(const std::string& text, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    void DrawTextCentered(const std::string& text, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    int DrawParagraph(const std::string& text, int x, int y, int width, int height, int r = 255, int g = 255, int b = 255, int a = 255);
    int GetStringMetrics(const std::string& text);

    std::shared_ptr<Font> m_Font;
    float m_FontSize = 12.0f;
    bool m_FixedWidth = false;
};

class InariaDisplay
{
public:
    bool m_FullScreen = false;

    void Enter2DMode();
    void ChangeToFullscreen();
    void ChangeToWindowed();

    void BlitImage(Texture* texture, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    void BlitImageRect(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, int r = 255, int g = 255, int b = 255, int a = 255);
    void BlitImageResized(Texture* texture, int destx, int desty, int destw, int desth, int r = 255, int g = 255, int b = 255, int a = 255);
    void BlitImageResized(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, int destw, int desth, int r = 255, int g = 255, int b = 255, int a = 255);

    void DrawSprite(LegacySprite* sprite, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
    void DrawSpriteRect(LegacySprite* sprite, int srcx, int srcy, int srcw, int srch, int destx, int desty, int r = 255, int g = 255, int b = 255, int a = 255);
    void DrawSpriteResized(LegacySprite* sprite, int destx, int desty, int destw, int desth, int r = 255, int g = 255, int b = 255, int a = 255);

    void DrawBoxCorners(int x, int y, int endx, int endy, int r = 255, int g = 255, int b = 255, int a = 255, bool filled = false);
    void DrawBox(int x, int y, int width, int height, int r = 255, int g = 255, int b = 255, int a = 255, bool filled = false);
    void DrawLine(int x1, int y1, int x2, int y2, int r = 255, int g = 255, int b = 255, int a = 255);
};

class KeyboardStateProxy
{
public:
    bool operator[](int sdlKey) const;
};

class InariaInput
{
public:
    void Update();

    bool IsMouseInRegion(int x, int y, int endx, int endy) const;
    bool IsLButtonDownInRegion(int x, int y, int endx, int endy);
    bool IsRButtonDownInRegion(int x, int y, int endx, int endy);
    bool IsMButtonDownInRegion(int x, int y, int endx, int endy);
    bool WasLButtonClickedInRegion(int x, int y, int endx, int endy);
    bool WasRButtonClickedInRegion(int x, int y, int endx, int endy);
    bool WasMButtonClickedInRegion(int x, int y, int endx, int endy);
    bool WasLButtonJustClickedInRegion(int x, int y, int endx, int endy);
    bool WasRButtonJustClickedInRegion(int x, int y, int endx, int endy);
    bool WasMButtonJustClickedInRegion(int x, int y, int endx, int endy);

    bool WasLButtonClicked() { return g_InputSystem->WasLButtonClicked(); }
    bool WasRButtonClicked() { return g_InputSystem->WasRButtonClicked(); }
    bool WasMButtonClicked() { return g_InputSystem->WasMButtonClicked(); }
    bool IsLButtonDown() { return g_InputSystem->IsLButtonDown(); }
    bool IsRButtonDown() { return g_InputSystem->IsRButtonDown(); }
    bool IsMButtonDown() { return g_InputSystem->IsMButtonDown(); }
    bool WasLButtonJustClicked() { return g_InputSystem->WasLButtonJustClicked(); }
    bool WasRButtonJustClicked() { return g_InputSystem->WasRButtonJustClicked(); }
    bool WasMButtonJustClicked() { return g_InputSystem->WasMButtonJustClicked(); }

    bool WasKeyPressed(unsigned int key) { return g_InputSystem->WasKeyPressed(SdlKeyToRaylib(key)); }
    bool IsKeyDown(unsigned int key) { return g_InputSystem->IsKeyDown(SdlKeyToRaylib(key)); }
    bool JustPressed(unsigned int key) { return g_InputSystem->JustPressed(SdlKeyToRaylib(key)); }
    void DumpInput() { if (g_InputSystem) g_InputSystem->DumpInput(); }

    int m_MouseX = 0;
    int m_MouseY = 0;
    bool m_WasLeftButtonClicked = false;
    bool m_WasRightButtonClicked = false;
    bool m_WasMiddleButtonClicked = false;
    bool m_IsLeftButtonDown = false;
    bool m_IsRightButtonDown = false;
    bool m_IsMiddleButtonDown = false;
    KeyboardStateProxy m_KeyboardState;
};

class InariaSound
{
public:
    void Play(const std::string& name, bool isMusic = false, bool loop = false);
    void PlaySoundGroup(const std::string& prefix, int count, const std::string& extension);
    void StopMusic();

    int GetSoundVolume();
    void SetSoundVolume(int volume);
    int GetMusicVolume();
    void SetMusicVolume(int volume);
    void* GetCurrentMusic() { return nullptr; }
};

extern InariaDisplay* g_Display;
extern InariaInput* g_Input;
extern InariaSound* g_Sound;
extern InariaFont* g_Font;
extern InariaFont* g_SmallFont;
extern InariaFont* g_SmallFixedFont;

#endif