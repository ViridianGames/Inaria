#include "InariaCompat.h"

#include <algorithm>
#include <cstdio>
#include <sstream>

#include "Geist/Engine.h"
#include "Geist/ResourceManager.h"

using namespace std;

InariaDisplay* g_Display = nullptr;
InariaInput* g_Input = nullptr;
InariaSound* g_Sound = nullptr;
InariaFont* g_Font = nullptr;
InariaFont* g_SmallFont = nullptr;
InariaFont* g_SmallFixedFont = nullptr;

static int ScaleX(int x) { return static_cast<int>(x * SX); }
static int ScaleY(int y) { return static_cast<int>(y * SY); }
static float ScaleXf(float x) { return x * SX; }
static float ScaleYf(float y) { return y * SY; }

bool KeyboardStateProxy::operator[](int sdlKey) const
{
    if (!g_InputSystem)
        return false;
    return g_InputSystem->IsKeyDown(SdlKeyToRaylib(sdlKey));
}

void InariaFont::Init(const std::string& fontPath, int fontSize)
{
    if (fontSize <= 0)
        fontSize = 12;
    m_FontSize = static_cast<float>(fontSize);
    m_Font = make_shared<::Font>(LoadFontEx(fontPath.c_str(), fontSize, nullptr, 0));
}

void InariaFont::DrawText(const std::string& text, int x, int y, int r, int g, int b, int a)
{
    if (!m_Font)
        return;
    DrawTextEx(*m_Font, text.c_str(), Vector2{ ScaleXf(static_cast<float>(x)), ScaleYf(static_cast<float>(y)) },
        m_FontSize, 1, Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
        static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void InariaFont::DrawTextA(const char* text, int x, int y, int r, int g, int b, int a)
{
    if (!text)
        return;
    DrawText(string(text), x, y, r, g, b, a);
}

void InariaFont::DrawTextA(const std::string& text, int x, int y, int r, int g, int b, int a)
{
    DrawText(text, x, y, r, g, b, a);
}

void InariaFont::DrawTextCentered(const std::string& text, int x, int y, int r, int g, int b, int a)
{
    int width = GetStringMetrics(text);
    DrawText(text, x - width / 2, y, r, g, b, a);
}

int InariaFont::DrawParagraph(const std::string& text, int x, int y, int width, int height, int r, int g, int b, int a)
{
    if (!m_Font)
        return 0;

    istringstream iss(text);
    string word;
    string line;
    int lines = 0;
    int posy = y;

    while (iss >> word)
    {
        string testLine = line.empty() ? word : line + " " + word;
        if (GetStringMetrics(testLine) > width && !line.empty())
        {
            DrawText(line, x, posy, r, g, b, a);
            posy += static_cast<int>(m_FontSize * 1.2f);
            ++lines;
            line = word + " ";
        }
        else
        {
            line = testLine + " ";
        }
    }

    if (!line.empty())
    {
        DrawText(line, x, posy, r, g, b, a);
        ++lines;
    }

    return lines;
}

int InariaFont::GetStringMetrics(const std::string& text)
{
    if (!m_Font)
        return 0;
    return static_cast<int>(MeasureTextEx(*m_Font, text.c_str(), m_FontSize, 1).x / SX);
}

void InariaDisplay::Enter2DMode()
{
}

void InariaDisplay::ChangeToFullscreen()
{
    if (!IsWindowFullscreen())
        ToggleFullscreen();
    m_FullScreen = true;
}

void InariaDisplay::ChangeToWindowed()
{
    if (IsWindowFullscreen())
        ToggleFullscreen();
    m_FullScreen = false;
}

void InariaDisplay::BlitImage(Texture* texture, int x, int y, int r, int g, int b, int a)
{
    if (!texture)
        return;
    DrawTexture(*texture, ScaleX(x), ScaleY(y),
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
        static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void InariaDisplay::BlitImageRect(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, int r, int g, int b, int a)
{
    if (!texture)
        return;
    Rectangle source{ static_cast<float>(srcx), static_cast<float>(srcy), static_cast<float>(srcw), static_cast<float>(srch) };
    Rectangle dest{ ScaleXf(static_cast<float>(destx)), ScaleYf(static_cast<float>(desty)),
        ScaleXf(static_cast<float>(srcw)), ScaleYf(static_cast<float>(srch)) };
    DrawTexturePro(*texture, source, dest, Vector2{ 0, 0 }, 0.0f,
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
        static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void InariaDisplay::BlitImageResized(Texture* texture, int destx, int desty, int destw, int desth, int r, int g, int b, int a)
{
    if (!texture)
        return;
    BlitImageResized(texture, 0, 0, texture->width, texture->height, destx, desty, destw, desth, r, g, b, a);
}

void InariaDisplay::BlitImageResized(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, int destw, int desth, int r, int g, int b, int a)
{
    if (!texture)
        return;
    Rectangle source{ static_cast<float>(srcx), static_cast<float>(srcy), static_cast<float>(srcw), static_cast<float>(srch) };
    Rectangle dest{ ScaleXf(static_cast<float>(destx)), ScaleYf(static_cast<float>(desty)),
        ScaleXf(static_cast<float>(destw)), ScaleYf(static_cast<float>(desth)) };
    DrawTexturePro(*texture, source, dest, Vector2{ 0, 0 }, 0.0f,
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
        static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void InariaDisplay::DrawSprite(LegacySprite* sprite, int x, int y, int r, int g, int b, int a)
{
    if (!sprite || !sprite->m_Texture)
        return;
    BlitImageRect(sprite->m_Texture, sprite->m_PosX, sprite->m_PosY, sprite->m_Width, sprite->m_Height, x, y, r, g, b, a);
}

void InariaDisplay::DrawSpriteRect(LegacySprite* sprite, int srcx, int srcy, int srcw, int srch, int destx, int desty, int r, int g, int b, int a)
{
    if (!sprite || !sprite->m_Texture)
        return;
    BlitImageRect(sprite->m_Texture, srcx, srcy, srcw, srch, destx, desty, r, g, b, a);
}

void InariaDisplay::DrawSpriteResized(LegacySprite* sprite, int destx, int desty, int destw, int desth, int r, int g, int b, int a)
{
    if (!sprite || !sprite->m_Texture)
        return;
    BlitImageResized(sprite->m_Texture, sprite->m_PosX, sprite->m_PosY, sprite->m_Width, sprite->m_Height,
        destx, desty, destw, desth, r, g, b, a);
}

void InariaDisplay::DrawBoxCorners(int x, int y, int endx, int endy, int r, int g, int b, int a, bool filled)
{
    int left = min(x, endx);
    int top = min(y, endy);
    int width = abs(endx - x);
    int height = abs(endy - y);
    DrawBox(left, top, width, height, r, g, b, a, filled);
}

void InariaDisplay::DrawBox(int x, int y, int width, int height, int r, int g, int b, int a, bool filled)
{
    Color color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
        static_cast<unsigned char>(b), static_cast<unsigned char>(a) };
    if (filled)
        DrawRectangle(ScaleX(x), ScaleY(y), ScaleX(width), ScaleY(height), color);
    else
        DrawRectangleLines(ScaleX(x), ScaleY(y), ScaleX(width), ScaleY(height), color);
}

void InariaDisplay::DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
    DrawLineEx(Vector2{ ScaleXf(static_cast<float>(x1)), ScaleYf(static_cast<float>(y1)) },
        Vector2{ ScaleXf(static_cast<float>(x2)), ScaleYf(static_cast<float>(y2)) },
        1.0f, Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
        static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void InariaInput::Update()
{
    if (!g_InputSystem || !g_Engine)
        return;

    float scale = g_Engine->m_RenderWidth / 640.0f;
    m_MouseX = static_cast<int>(g_InputSystem->m_MouseX / scale);
    m_MouseY = static_cast<int>(g_InputSystem->m_MouseY / scale);
    m_WasLeftButtonClicked = g_InputSystem->WasLButtonClicked();
    m_WasRightButtonClicked = g_InputSystem->WasRButtonClicked();
    m_WasMiddleButtonClicked = g_InputSystem->WasMButtonClicked();
    m_IsLeftButtonDown = g_InputSystem->IsLButtonDown();
    m_IsRightButtonDown = g_InputSystem->IsRButtonDown();
    m_IsMiddleButtonDown = g_InputSystem->IsMButtonDown();
}

bool InariaInput::IsMouseInRegion(int x, int y, int endx, int endy) const
{
    return m_MouseX >= x && m_MouseX <= endx && m_MouseY >= y && m_MouseY <= endy;
}

bool InariaInput::IsLButtonDownInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->IsLButtonDown() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::IsRButtonDownInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->IsRButtonDown() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::IsMButtonDownInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->IsMButtonDown() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::WasLButtonClickedInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->WasLButtonClicked() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::WasRButtonClickedInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->WasRButtonClicked() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::WasMButtonClickedInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->WasMButtonClicked() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::WasLButtonJustClickedInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->WasLButtonJustClicked() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::WasRButtonJustClickedInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->WasRButtonJustClicked() && IsMouseInRegion(x, y, endx, endy);
}

bool InariaInput::WasMButtonJustClickedInRegion(int x, int y, int endx, int endy)
{
    return g_InputSystem->WasMButtonJustClicked() && IsMouseInRegion(x, y, endx, endy);
}

void InariaSound::Play(const std::string& name, bool isMusic, bool loop)
{
    if (!g_SoundSystem)
        return;

    string path = name;
    if (path.front() == '/')
        path.erase(0, 1);

    if (isMusic)
        g_SoundSystem->PlayMusic(path);
    else
        g_SoundSystem->PlaySound(path);
}

void InariaSound::PlaySoundGroup(const std::string& prefix, int count, const std::string& extension)
{
    if (!g_SoundSystem || count <= 0)
        return;

    int index = GetRandomValue(1, count);
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "%s%d%s", prefix.c_str(), index, extension.c_str());
    g_SoundSystem->PlaySound(buffer);
}

void InariaSound::StopMusic()
{
    if (g_SoundSystem)
        g_SoundSystem->StopMusic("");
}

int InariaSound::GetSoundVolume()
{
    if (!g_SoundSystem)
        return 0;
    return static_cast<int>(g_SoundSystem->GetGlobalSoundVolume() * 255.0f);
}

void InariaSound::SetSoundVolume(int volume)
{
    if (!g_SoundSystem)
        return;
    g_SoundSystem->SetGlobalSoundVolume(volume / 255.0f);
}

int InariaSound::GetMusicVolume()
{
    if (!g_SoundSystem)
        return 0;
    return static_cast<int>(g_SoundSystem->GetGlobalMusicVolume() * 255.0f);
}

void InariaSound::SetMusicVolume(int volume)
{
    if (!g_SoundSystem)
        return;
    g_SoundSystem->SetGlobalMusicVolume(volume / 255.0f);
}