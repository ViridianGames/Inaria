#ifndef _INARIAEFFECT_H_
#define _INARIAEFFECT_H_

#include <cstdint>
#include <deque>
#include <list>
#include <string>

#include "../Geist/Source/Primitives.h"

struct Position
{
    int x = 0;
    int y = 0;
};

class InariaEffect
{
public:
    InariaEffect() = default;
    virtual ~InariaEffect();

    void Init(Sprite* sprite,
        int posxstart, int posystart,
        int posxend, int posyend,
        uint32_t time, bool isblocking = true,
        int animationType = 0,
        std::string playonstart = std::string(""),
        std::string playonend = std::string(""),
        int red = 255,
        int green = 255,
        int blue = 255,
        int alpha = 255);

    virtual void Shutdown();
    virtual void Update();
    virtual void Draw();

    virtual bool IsDead() { return m_IsDead; }

    bool m_IsDead = false;

    uint32_t m_StartTime = 0;
    uint32_t m_EndTime = 0;
    uint32_t m_Duration = 0;

    int m_PosX = 0;
    int m_PosY = 0;
    int m_PosXEnd = 0;
    int m_PosYEnd = 0;

    int m_AnimationType = 0;
    bool m_IsBlocking = true;
    bool m_FirstUpdate = true;

    int m_Red = 255;
    int m_Green = 255;
    int m_Blue = 255;
    int m_Alpha = 255;

    Sprite* m_Sprite = nullptr;

    std::string m_PlayOnStart;
    std::string m_PlayOnEnd;

    std::deque<Position> m_Positions;
};

#endif