#ifndef _GEISTGLOBALS_H_
#define _GEISTGLOBALS_H_

#include <memory>
#include <string>
#include "raylib.h"

class Engine;
class ResourceManager;
class StateMachine;
class ScriptingSystem;
class SoundSystem;
class InputSystem;

class Sprite;

//  Global pointers

extern std::unique_ptr<Engine>           g_Engine;
extern std::unique_ptr<ResourceManager>  g_ResourceManager;
extern std::unique_ptr<StateMachine>     g_StateMachine;
extern std::unique_ptr<ScriptingSystem>  g_ScriptingSystem;
extern std::unique_ptr<SoundSystem>      g_SoundSystem;
extern std::unique_ptr<InputSystem>      g_InputSystem;

//  Global functions
void DrawStringCentered(Font* font, float fontsize, std::string text, float centerx, float centery,  Color color = WHITE);
void DrawStringCentered(Font* font, float fontsize, std::string text, Vector2 center, Color color = WHITE);
void DrawStringCentered(Font* font, float fontsize, char* text, float centerx, float centery, Color color = WHITE);
void DrawStringCentered(Font* font, float fontsize, char* text, Vector2 center, Color color = WHITE);
void DrawStringRight(Font* font, float fontsize, std::string text, float rightx, float y, Color color = WHITE);
void DrawStringRight(Font* font, float fontsize, char* text, float rightx, float y, Color color = WHITE);
void DebugPrint(std::string msg);

// Design-space coordinates (default 640x480 logical resolution scaled to render resolution)
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

#endif