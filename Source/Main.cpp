///////////////////////////////////////////////////////////////////////////
//
// Name:     MAIN.CPP
// Author:   Anthony Salter
// Date:     2/03/05
// Purpose:  Contains the entry point for the program.
//
///////////////////////////////////////////////////////////////////////////

#include "Geist/Globals.h"
#include "Geist/Engine.h"
#include "Geist/StateMachine.h"
#include "Geist/ResourceManager.h"
#include "Geist/RNG.h"
#include "raylib.h"

#include <memory>
#include <sstream>
#include <string>

#include "GameGlobals.h"
#include "InariaCompat.h"
#include "InariaEffect.h"

#include "EditorState.h"
#include "EnchantingState.h"
#include "InfiniteWarningState.h"
#include "LevelUpState.h"
#include "MainState.h"
#include "OptionsState.h"
#include "StoreState.h"
#include "TitleState.h"
#include "TutorialState.h"

using namespace std;

int main(int argv, char** argc)
{
    g_Engine = make_unique<Engine>();
    g_Engine->Init("engine.cfg");
    g_Engine->m_useVirtualResolution = true;

    g_Display = new InariaDisplay();
    g_Input = new InariaInput();
    g_Sound = new InariaSound();

    g_Display->Enter2DMode();

    g_Font = new InariaFont();
    g_Font->Init("Fonts/softsquare.ttf", 12);

    g_SmallFont = new InariaFont();
    g_SmallFont->Init("Fonts/littleleague.ttf", 8);

    g_SmallFixedFont = new InariaFont();
    g_SmallFixedFont->Init("Fonts/littleleague.ttf", 8);
    g_SmallFixedFont->SetFixedWidth();

    g_Mask = g_ResourceManager->GetTexture("Images/mask.png");
    g_Star = new LegacySprite();
    g_Star->m_Texture = g_ResourceManager->GetTexture("Images/star.png");
    g_Star->m_Width = static_cast<int>(g_Star->m_Texture->width);
    g_Star->m_Height = static_cast<int>(g_Star->m_Texture->height);

    g_Bubble = new LegacySprite();
    g_Bubble->m_Texture = g_ResourceManager->GetTexture("Images/bubble.png");
    g_Bubble->m_Width = static_cast<int>(g_Bubble->m_Texture->width);
    g_Bubble->m_Height = static_cast<int>(g_Bubble->m_Texture->height);

    g_TextBackground = g_ResourceManager->GetTexture("Images/textbackground.png");
    g_TBUL = g_ResourceManager->GetTexture("Images/tbul.png");
    g_TBUR = g_ResourceManager->GetTexture("Images/tbur.png");
    g_TBLR = g_ResourceManager->GetTexture("Images/tblr.png");
    g_TBLL = g_ResourceManager->GetTexture("Images/tbll.png");
    g_TalkSpurLeft = g_ResourceManager->GetTexture("Images/talkspurleft.png");
    g_TalkSpurRight = g_ResourceManager->GetTexture("Images/talkspurright.png");
    g_TalkSpurRightUp = g_ResourceManager->GetTexture("Images/talkspurrightup.png");

    g_FloorShadows[0] = g_ResourceManager->GetTexture("Images/floorshadow_corner1.png");
    g_FloorShadows[1] = g_ResourceManager->GetTexture("Images/floorshadow_corner2.png");
    g_FloorShadows[2] = g_ResourceManager->GetTexture("Images/floorshadow_corner3.png");
    g_FloorShadows[3] = g_ResourceManager->GetTexture("Images/floorshadow_corner4.png");
    g_FloorShadows[4] = g_ResourceManager->GetTexture("Images/floorshadow_e.png");
    g_FloorShadows[5] = g_ResourceManager->GetTexture("Images/floorshadow_s.png");

    g_WallShadows[0] = g_ResourceManager->GetTexture("Images/wallshadow_e.png");
    g_WallShadows[1] = g_ResourceManager->GetTexture("Images/wallshadow_n.png");
    g_WallShadows[2] = g_ResourceManager->GetTexture("Images/wallshadow_ne.png");
    g_WallShadows[3] = g_ResourceManager->GetTexture("Images/wallshadow_nw.png");
    g_WallShadows[4] = g_ResourceManager->GetTexture("Images/wallshadow_s.png");
    g_WallShadows[5] = g_ResourceManager->GetTexture("Images/wallshadow_se.png");
    g_WallShadows[6] = g_ResourceManager->GetTexture("Images/wallshadow_sw.png");
    g_WallShadows[7] = g_ResourceManager->GetTexture("Images/wallshadow_w.png");

    g_InfiniteYes = 0;

    g_RNG = new RNG();
    g_RNG->SeedRNG(32767);

    g_MusicTracks[MUSIC_CASTLE] = "Music/inaria_castle-v001m1.ogg";
    g_MusicTracks[MUSIC_DANCEPARTY] = "Music/inaria_dance_party-v001m1.ogg";
    g_MusicTracks[MUSIC_DUNGEON] = "Music/inaria_dungeon-v002m1.ogg";
    g_MusicTracks[MUSIC_EMERALD] = "Music/inaria_emerald-v001m1.ogg";
    g_MusicTracks[MUSIC_LASTFRONT] = "Music/inaria_last_front-v002m1.ogg";
    g_MusicTracks[MUSIC_OVERWORLD] = "Music/inaria_overworld-v001m1.ogg";
    g_MusicTracks[MUSIC_RUBY] = "Music/inaria_ruby-v002m1.ogg";
    g_MusicTracks[MUSIC_SAPPHIRE] = "Music/inaria_sapphire-v001m1.ogg";
    g_MusicTracks[MUSIC_SHOPPING] = "Music/inaria_shopping-v001m1.ogg";
    g_MusicTracks[MUSIC_THEME] = "Music/inaria_theme-v003m1.ogg";
    g_MusicTracks[MUSIC_VOLCANOHEART] = "Music/inaria_volcano_heart-v001m1.ogg";
    g_MusicTracks[MUSIC_WIN] = "Music/inaria_win-v002fm1.ogg";

    g_TileSize = 18;
    g_Offset = static_cast<int>(g_Engine->m_EngineConfig.GetNumber("offset"));
    g_ViewRange = static_cast<int>(g_Engine->m_EngineConfig.GetNumber("viewrange"));

    MainState* mainState = new MainState();
    mainState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_MAINSTATE, mainState, "MainState");

    LevelUpState* levelUpState = new LevelUpState();
    levelUpState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_LEVELUPSTATE, levelUpState, "LevelUpState");

    StoreState* storeState = new StoreState();
    storeState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_STORESTATE, storeState, "StoreState");

    OptionsState* optionsState = new OptionsState();
    optionsState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_OPTIONSSTATE, optionsState, "OptionsState");

    TitleState* titleState = new TitleState();
    titleState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_TITLESTATE, titleState, "TitleState");

    EditorState* editorState = new EditorState();
    editorState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_EDITORSTATE, editorState, "EditorState");

    TutorialState* tutorialState = new TutorialState();
    tutorialState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_TUTORIALSTATE, tutorialState, "TutorialState");

    EnchantingState* enchanterState = new EnchantingState();
    enchanterState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_ENCHANTERSTATE, enchanterState, "EnchantingState");

    InfiniteWarningState* infiniteWarningState = new InfiniteWarningState();
    infiniteWarningState->Init("engine.cfg");
    g_StateMachine->RegisterState(STATE_INFINITEWARNINGSTATE, infiniteWarningState, "InfiniteWarningState");

    Texture* spritesTex = g_ResourceManager->GetTexture("Images/sprites.png");
    for (int i = 0; i < NUMBER_OF_TILES; ++i)
    {
        LegacySprite* sprite = new LegacySprite();
        sprite->m_Texture = spritesTex;
        sprite->m_Width = g_TileSize;
        sprite->m_Height = g_TileSize;
        sprite->m_PosX = (i % 16) * g_TileSize;
        sprite->m_PosY = (i / 16) * g_TileSize;
        g_Tiles[i] = sprite;
    }

    g_StateMachine->MakeStateTransition(STATE_TITLESTATE);

    while (!g_Engine->m_Done && !WindowShouldClose())
    {
        g_Input->Update();
        g_Engine->Update();
        g_Engine->Draw();
        PlayMusic();
    }

    g_Engine->Shutdown();

    delete g_Font;
    delete g_SmallFont;
    delete g_SmallFixedFont;
    delete g_Input;
    delete g_Sound;
    delete g_Display;

    return 0;
}