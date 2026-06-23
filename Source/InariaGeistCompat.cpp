#include "InariaGeistCompat.h"

#include "../Geist/Source/Engine.h"
#include "../Geist/Source/Globals.h"
#include "../Geist/Source/InputSystem.h"
#include "../Geist/Source/Primitives.h"
#include "../Geist/Source/Primitives.h"
#include "../Geist/Source/RNG.h"
#include "../Geist/Source/SoundSystem.h"
#include "../Geist/Source/TooltipSystem.h"

#include <memory>
#include <sstream>

float DesignScaleX()
{
    if (!g_Engine)
        return DESIGN_WIDTH / 640.0f;
    return g_Engine->m_RenderWidth / DESIGN_WIDTH;
}

float DesignScaleY()
{
    if (!g_Engine)
        return DESIGN_HEIGHT / 480.0f;
    return g_Engine->m_RenderHeight / DESIGN_HEIGHT;
}

float DesignToRenderX(float x) { return x * DesignScaleX(); }
float DesignToRenderY(float y) { return y * DesignScaleY(); }
float DesignToRenderW(float w) { return w * DesignScaleX(); }
float DesignToRenderH(float h) { return h * DesignScaleY(); }

int GetDesignMouseX()
{
    if (!g_InputSystem)
        return 0;
    return static_cast<int>(g_InputSystem->m_MouseX / DesignScaleX());
}

int GetDesignMouseY()
{
    if (!g_InputSystem)
        return 0;
    return static_cast<int>(g_InputSystem->m_MouseY / DesignScaleY());
}

int GetStringMetrics(Font* font, float fontSize, const std::string& text)
{
    if (!font)
        return 0;
    return static_cast<int>(MeasureTextEx(*font, text.c_str(), fontSize, 1).x / DesignScaleX());
}

void DrawTextAt(Font* font, float fontSize, const std::string& text, int x, int y, Color color)
{
    if (!font)
        return;
    DrawTextEx(*font, text.c_str(),
        Vector2{ DesignToRenderX(static_cast<float>(x)), DesignToRenderY(static_cast<float>(y)) },
        fontSize, 1, color);
}

void DrawTextAt(Font* font, float fontSize, const std::string& text, int x, int y, int r, int g, int b, int a)
{
    DrawTextAt(font, fontSize, text, x, y,
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
            static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void DrawTextCenteredAt(Font* font, float fontSize, const std::string& text, int centerX, int centerY, Color color)
{
    int width = GetStringMetrics(font, fontSize, text);
    DrawTextAt(font, fontSize, text, centerX - width / 2, centerY, color);
}

void DrawTextCenteredAt(Font* font, float fontSize, const std::string& text, int centerX, int centerY, int r, int g, int b, int a)
{
    DrawTextCenteredAt(font, fontSize, text, centerX, centerY,
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
            static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

int DrawParagraphAt(Font* font, float fontSize, const std::string& text, int x, int y, int maxWidth, int lineHeight, Color color)
{
    if (!font)
        return 0;

    std::istringstream iss(text);
    std::string word;
    std::string line;
    int lines = 0;
    int posy = y;

    while (iss >> word)
    {
        std::string testLine = line.empty() ? word : line + " " + word;
        if (GetStringMetrics(font, fontSize, testLine) > maxWidth && !line.empty())
        {
            DrawTextAt(font, fontSize, line, x, posy, color);
            posy += lineHeight;
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
        DrawTextAt(font, fontSize, line, x, posy, color);
        ++lines;
    }

    return lines;
}

void DrawImageAt(Texture* texture, int x, int y, Color tint)
{
    if (!texture)
        return;
    DrawTexture(*texture,
        static_cast<int>(DesignToRenderX(static_cast<float>(x))),
        static_cast<int>(DesignToRenderY(static_cast<float>(y))), tint);
}

void DrawImageRectAt(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, Color tint)
{
    if (!texture)
        return;
    Rectangle source{ static_cast<float>(srcx), static_cast<float>(srcy), static_cast<float>(srcw), static_cast<float>(srch) };
    Rectangle dest{ DesignToRenderX(static_cast<float>(destx)), DesignToRenderY(static_cast<float>(desty)),
        DesignToRenderW(static_cast<float>(srcw)), DesignToRenderH(static_cast<float>(srch)) };
    DrawTexturePro(*texture, source, dest, Vector2{ 0, 0 }, 0.0f, tint);
}

void DrawImageRectScaledAt(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, int destw, int desth, Color tint)
{
    if (!texture)
        return;
    Rectangle source{ static_cast<float>(srcx), static_cast<float>(srcy), static_cast<float>(srcw), static_cast<float>(srch) };
    Rectangle dest{ DesignToRenderX(static_cast<float>(destx)), DesignToRenderY(static_cast<float>(desty)),
        DesignToRenderW(static_cast<float>(destw)), DesignToRenderH(static_cast<float>(desth)) };
    DrawTexturePro(*texture, source, dest, Vector2{ 0, 0 }, 0.0f, tint);
}

void DrawImageResizedAt(Texture* texture, int destx, int desty, int destw, int desth, Color tint)
{
    if (!texture)
        return;
    DrawImageRectScaledAt(texture, 0, 0, texture->width, texture->height, destx, desty, destw, desth, tint);
}

void DrawSpriteAt(Sprite* sprite, int x, int y, Color tint)
{
    if (!sprite || !sprite->m_texture)
        return;
    Rectangle dest{ DesignToRenderX(static_cast<float>(x)), DesignToRenderY(static_cast<float>(y)),
        DesignToRenderW(sprite->m_sourceRect.width), DesignToRenderH(sprite->m_sourceRect.height) };
    sprite->DrawScaled(dest, Vector2{ 0, 0 }, 0.0f, tint);
}

void DrawSpriteAt(Sprite* sprite, int x, int y, int r, int g, int b, int a)
{
    DrawSpriteAt(sprite, x, y,
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
            static_cast<unsigned char>(b), static_cast<unsigned char>(a) });
}

void DrawSpriteRectAt(Sprite* sprite, int srcx, int srcy, int srcw, int srch, int destx, int desty, Color tint)
{
    if (!sprite || !sprite->m_texture)
        return;
    Rectangle source{ static_cast<float>(srcx), static_cast<float>(srcy), static_cast<float>(srcw), static_cast<float>(srch) };
    Rectangle dest{ DesignToRenderX(static_cast<float>(destx)), DesignToRenderY(static_cast<float>(desty)),
        DesignToRenderW(static_cast<float>(srcw)), DesignToRenderH(static_cast<float>(srch)) };
    DrawTexturePro(*sprite->m_texture, source, dest, Vector2{ 0, 0 }, 0.0f, tint);
}

void DrawSpriteResizedAt(Sprite* sprite, int destx, int desty, int destw, int desth, Color tint)
{
    if (!sprite || !sprite->m_texture)
        return;
    Rectangle dest{ DesignToRenderX(static_cast<float>(destx)), DesignToRenderY(static_cast<float>(desty)),
        DesignToRenderW(static_cast<float>(destw)), DesignToRenderH(static_cast<float>(desth)) };
    sprite->DrawScaled(dest, Vector2{ 0, 0 }, 0.0f, tint);
}

void DrawBoxAt(int x, int y, int width, int height, Color color, bool filled)
{
    int rx = static_cast<int>(DesignToRenderX(static_cast<float>(x)));
    int ry = static_cast<int>(DesignToRenderY(static_cast<float>(y)));
    int rw = static_cast<int>(DesignToRenderW(static_cast<float>(width)));
    int rh = static_cast<int>(DesignToRenderH(static_cast<float>(height)));
    if (filled)
        DrawRectangle(rx, ry, rw, rh, color);
    else
        DrawRectangleLines(rx, ry, rw, rh, color);
}

void DrawBoxAt(int x, int y, int width, int height, int r, int g, int b, int a, bool filled)
{
    DrawBoxAt(x, y, width, height,
        Color{ static_cast<unsigned char>(r), static_cast<unsigned char>(g),
            static_cast<unsigned char>(b), static_cast<unsigned char>(a) }, filled);
}

bool IsMouseInDesignRegion(int x1, int y1, int x2, int y2)
{
    int mouseX = GetDesignMouseX();
    int mouseY = GetDesignMouseY();
    return mouseX >= x1 && mouseX <= x2 && mouseY >= y1 && mouseY <= y2;
}

bool IsLButtonDownInDesignRegion(int x1, int y1, int x2, int y2)
{
    return g_InputSystem && g_InputSystem->m_IsLeftButtonDown && IsMouseInDesignRegion(x1, y1, x2, y2);
}

bool WasLButtonClickedInDesignRegion(int x1, int y1, int x2, int y2)
{
    return g_InputSystem && g_InputSystem->m_WasLeftButtonClicked && IsMouseInDesignRegion(x1, y1, x2, y2);
}

bool WasRButtonClickedInDesignRegion(int x1, int y1, int x2, int y2)
{
    return g_InputSystem && g_InputSystem->m_WasRightButtonClicked && IsMouseInDesignRegion(x1, y1, x2, y2);
}

bool WasLButtonJustClickedInDesignRegion(int x1, int y1, int x2, int y2)
{
    return g_InputSystem && g_InputSystem->m_IsLeftButtonDown && !g_InputSystem->m_WasLeftButtonDown
        && IsMouseInDesignRegion(x1, y1, x2, y2);
}

void PlaySoundGroup(const std::string& prefix, int count, const std::string& extension)
{
    if (!g_SoundSystem || count <= 0)
        return;

    static std::unique_ptr<RNG> soundGroupRNG = std::make_unique<RNG>();
    int index = soundGroupRNG->Random(count) + 1;
    g_SoundSystem->PlaySound(prefix + std::to_string(index) + extension);
}

void DrawToolTipRender(Font* font, float size, std::vector<ColoredString> strings, int x, int y, float lineWidth, int anchorcorner)
{
    DrawToolTip(font, size, strings, x, y, lineWidth, anchorcorner);
}

void DrawToolTipRender(Font* font, float size, std::string strings, int x, int y, float lineWidth, int anchorcorner, Color color)
{
    DrawToolTip(font, size, strings, x, y, lineWidth, anchorcorner, color);
}