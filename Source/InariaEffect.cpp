#include "InariaEffect.h"

#include "GameGlobals.h"

using namespace std;

InariaEffect::~InariaEffect()
{
}

void InariaEffect::Init(LegacySprite* sprite, int posxstart, int posystart, int posxend, int posyend,
    uint32_t time, bool isblocking, int animationType, string playonstart, string playonend,
    int red, int green, int blue, int alpha)
{
    m_IsDead = false;
    m_StartTime = SDL_GetTicks();
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
    m_Texture = sprite;
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
        m_StartTime = SDL_GetTicks();
        m_EndTime = m_StartTime + m_Duration;
        if (g_Sound && !m_PlayOnStart.empty())
            g_Sound->Play(m_PlayOnStart);
    }

    if (SDL_GetTicks() > m_EndTime)
    {
        if (g_Sound && !m_PlayOnEnd.empty())
            g_Sound->Play(m_PlayOnEnd);
        m_IsDead = true;
    }

    if (!m_IsDead)
    {
        int deltatime = SDL_GetTicks() - m_StartTime;
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
        if (m_Texture && !m_FirstUpdate)
            g_Display->DrawSprite(m_Texture, m_Positions.front().x, m_Positions.front().y, m_Red, m_Green, m_Blue, m_Alpha);
        break;

    case 1:
        if (m_Texture && !m_FirstUpdate)
        {
            for (size_t i = 0; i < m_Positions.size(); i += 2)
            {
                g_Display->DrawSpriteRect(m_Texture,
                    static_cast<int>((i / 2) * (m_Texture->m_Width / 4)), 0,
                    7 * (m_Texture->m_Height / 8), 7 * (m_Texture->m_Height / 8),
                    m_Positions[i].x, m_Positions[i].y, m_Red, m_Green, m_Blue, m_Alpha);
            }
        }
        break;
    }
}