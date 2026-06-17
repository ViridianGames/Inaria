#include "InariaEffect.h"

#include "GameGlobals.h"

using namespace std;

InariaEffect::~InariaEffect()
{
}

void InariaEffect::Init(Sprite* sprite, int posxstart, int posystart, int posxend, int posyend,
    uint32_t time, bool isblocking, int animationType, string playonstart, string playonend,
    int red, int green, int blue, int alpha)
{
    m_IsDead = false;
    m_StartTime = static_cast<uint32_t>(g_Engine->GameTimeInMS());
    m_EndTime = m_StartTime + time;
    m_Duration = time;

    m_PosX = posxstart;
    m_PosY = posystart;
    m_PosXEnd = posxend;
    m_PosYEnd = posyend;

    m_Red = red;
    m_Green = green;
    m_Blue = blue;
    m_Alpha = alpha;

    m_AnimationType = animationType;
    m_Sprite = sprite;
    m_PlayOnStart = playonstart;
    m_PlayOnEnd = playonend;
    m_IsBlocking = isblocking;
    m_FirstUpdate = true;
}

void InariaEffect::Shutdown()
{
    m_IsDead = true;
}

void InariaEffect::Update()
{
    if (m_FirstUpdate)
    {
        m_FirstUpdate = false;
        m_StartTime = static_cast<uint32_t>(g_Engine->GameTimeInMS());
        m_EndTime = m_StartTime + m_Duration;
        if (g_SoundSystem && !m_PlayOnStart.empty())
            g_SoundSystem->PlaySound(m_PlayOnStart);
    }

    if (static_cast<uint32_t>(g_Engine->GameTimeInMS()) > m_EndTime)
    {
        if (g_SoundSystem && !m_PlayOnEnd.empty())
            g_SoundSystem->PlaySound(m_PlayOnEnd);
        m_IsDead = true;
    }

    if (!m_IsDead)
    {
        int deltatime = static_cast<uint32_t>(g_Engine->GameTimeInMS()) - m_StartTime;
        float percenttime = float(deltatime) / float(m_Duration);

        Position temp;
        temp.x = m_PosX + static_cast<int>(percenttime * (m_PosXEnd - m_PosX));
        temp.y = m_PosY + static_cast<int>(percenttime * (m_PosYEnd - m_PosY));

        m_Positions.push_front(temp);

        while (m_Positions.size() > 8)
            m_Positions.pop_back();

        for (size_t i = 1; i < m_Positions.size(); ++i)
        {
            if (g_RNG)
            {
                m_Positions[i].x += (g_RNG->Random(3) - 1);
                m_Positions[i].y += (g_RNG->Random(3) - 1);
            }
        }
    }
}

void InariaEffect::Draw()
{
    if (m_Positions.empty())
        return;

    switch (m_AnimationType)
    {
    case 0:
        if (m_Sprite && !m_FirstUpdate)
            DrawSpriteAt(m_Sprite, m_Positions.front().x, m_Positions.front().y, m_Red, m_Green, m_Blue, m_Alpha);
        break;

    case 1:
        if (m_Sprite && !m_FirstUpdate)
        {
            const int spriteWidth = static_cast<int>(m_Sprite->m_sourceRect.width);
            const int spriteHeight = static_cast<int>(m_Sprite->m_sourceRect.height);
            for (size_t i = 0; i < m_Positions.size(); i += 2)
            {
                DrawSpriteRectAt(m_Sprite,
                    static_cast<int>((i / 2) * (spriteWidth / 4)), 0,
                    7 * (spriteHeight / 8), 7 * (spriteHeight / 8),
                    m_Positions[i].x, m_Positions[i].y, MakeColor(m_Red, m_Green, m_Blue, m_Alpha));
            }
        }
        break;
    }
}