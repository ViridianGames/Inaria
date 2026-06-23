#ifndef _INARIAGEISTCOMPAT_H_
#define _INARIAGEISTCOMPAT_H_

#include <string>
#include <vector>
#include "raylib.h"

struct ColoredString;

class Font;
class InputSystem;
class SoundSystem;
class Sprite;
class Texture;

constexpr float DESIGN_WIDTH = 640.0f;
constexpr float DESIGN_HEIGHT = 480.0f;

float DesignScaleX();
float DesignScaleY();
float DesignToRenderX(float x);
float DesignToRenderY(float y);
float DesignToRenderW(float w);
float DesignToRenderH(float h);
int GetDesignMouseX();
int GetDesignMouseY();

int GetStringMetrics(Font* font, float fontSize, const std::string& text);
void DrawTextAt(Font* font, float fontSize, const std::string& text, int x, int y, Color color = WHITE);
void DrawTextAt(Font* font, float fontSize, const std::string& text, int x, int y, int r, int g, int b, int a = 255);
void DrawTextCenteredAt(Font* font, float fontSize, const std::string& text, int centerX, int centerY, Color color = WHITE);
void DrawTextCenteredAt(Font* font, float fontSize, const std::string& text, int centerX, int centerY, int r, int g, int b, int a = 255);
int DrawParagraphAt(Font* font, float fontSize, const std::string& text, int x, int y, int maxWidth, int lineHeight, Color color = WHITE);

void DrawImageAt(Texture* texture, int x, int y, Color tint = WHITE);
void DrawImageRectAt(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, Color tint = WHITE);
void DrawImageRectScaledAt(Texture* texture, int srcx, int srcy, int srcw, int srch, int destx, int desty, int destw, int desth, Color tint = WHITE);
void DrawImageResizedAt(Texture* texture, int destx, int desty, int destw, int desth, Color tint = WHITE);
void DrawSpriteAt(Sprite* sprite, int x, int y, Color tint = WHITE);
void DrawSpriteAt(Sprite* sprite, int x, int y, int r, int g, int b, int a = 255);
void DrawSpriteRectAt(Sprite* sprite, int srcx, int srcy, int srcw, int srch, int destx, int desty, Color tint = WHITE);
void DrawSpriteResizedAt(Sprite* sprite, int destx, int desty, int destw, int desth, Color tint = WHITE);
void DrawBoxAt(int x, int y, int width, int height, Color color = WHITE, bool filled = false);
void DrawBoxAt(int x, int y, int width, int height, int r, int g, int b, int a = 255, bool filled = false);

bool IsMouseInDesignRegion(int x1, int y1, int x2, int y2);
bool IsLButtonDownInDesignRegion(int x1, int y1, int x2, int y2);
bool WasLButtonClickedInDesignRegion(int x1, int y1, int x2, int y2);
bool WasRButtonClickedInDesignRegion(int x1, int y1, int x2, int y2);
bool WasLButtonJustClickedInDesignRegion(int x1, int y1, int x2, int y2);

void PlaySoundGroup(const std::string& prefix, int count, const std::string& extension);
void DrawToolTipRender(Font* font, float size, std::vector<ColoredString> strings, int x, int y, float lineWidth = 1, int anchorcorner = 0);
void DrawToolTipRender(Font* font, float size, std::string strings, int x, int y, float lineWidth = 1, int anchorcorner = 0, Color color = WHITE);

#endif