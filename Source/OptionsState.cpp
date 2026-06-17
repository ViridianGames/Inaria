#include "OptionsState.h"
#include "GameGlobals.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  OptionsState
////////////////////////////////////////////////////////////////////////////////

OptionsState::~OptionsState()
{

}

void OptionsState::Init(const std::string& configfile)
{
   m_Quitting = false;
   m_Returning = false;
}

void OptionsState::Shutdown(){}
void OptionsState::Update()
{
   if( g_InputSystem->WasKeyPressed(KEY_ESCAPE) )
   {
      g_StateMachine->PopState();
   }

}
void OptionsState::Draw()
{
   DrawImageRectAt( g_Mask, 0, 0, 228, 276, 76, 57);
   DrawBoxAt(76, 57, 228, 276);

   stringstream temp;

   DrawTextCenteredAt(g_font.get(), g_fontSize, "Options", 190, 67 );

   if( m_Returning )
   {

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Return to the", 190, 130 );
      DrawTextCenteredAt(g_font.get(), g_fontSize, "Main Menu? You will lose", 190, 150 );
      DrawTextCenteredAt(g_font.get(), g_fontSize, "any unsaved progress.", 190, 170 );

      if( DrawButton("Yes", 150, 190 ) )
      {
         g_StateMachine->MakeStateTransition(STATE_TITLESTATE);
      }

      if( DrawButton("No", 200, 190 ) )
      {
         m_Returning = false;
      }
   }
   else if( m_Quitting )
   {

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Do you wish to quit", 190, 130 );
      DrawTextCenteredAt(g_font.get(), g_fontSize, "the game? You will lose", 190, 150 );
      DrawTextCenteredAt(g_font.get(), g_fontSize, "any unsaved progress.", 190, 170 );

      if( DrawButton("Yes", 150, 190 ) )
      {
         exit(0);
      }

      if( DrawButton("No", 200, 190 ) )
      {
         m_Quitting = false;
      }
   }
   else
   {
      DrawTextCenteredAt(g_font.get(), g_fontSize, "Sound Volume", 190, 97 );

      if( DrawButton("<-", 110, 97 ) )
      {
         float currentvolume = g_SoundSystem->GetGlobalSoundVolume();
         g_SoundSystem->SetGlobalSoundVolume(currentvolume - 8.0f);
         g_SoundSystem->PlaySound("Sounds/level_up.wav");
      }

      if( DrawButton("->", 253, 97 ) )
      {
         float currentvolume = g_SoundSystem->GetGlobalSoundVolume();
         g_SoundSystem->SetGlobalSoundVolume(currentvolume + 8.0f);
         g_SoundSystem->PlaySound("Sounds/level_up.wav");
      }

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Music Volume", 190, 117 );
      if( DrawButton("<-", 110, 117 ) )
      {
         float currentvolume = g_SoundSystem->GetGlobalMusicVolume();
         g_SoundSystem->SetGlobalMusicVolume(currentvolume - 8.0f);
      }

      if( DrawButton("->", 253, 117 ) )
      {
         float currentvolume = g_SoundSystem->GetGlobalMusicVolume();
         g_SoundSystem->SetGlobalMusicVolume(currentvolume + 8.0f);
      }


      if( g_Player->m_PlayerSprite == 46 )
      {
         if( DrawButtonCentered("Female Avatar", 190, 147 ) )
         {
            g_Player->m_PlayerSprite = 80;
         }
      }
      else
      {
         if( DrawButtonCentered("Male Avatar", 190, 147 ) )
         {
            g_Player->m_PlayerSprite = 46;
         }
      }

      if( IsWindowFullscreen() )
      {
         if( DrawButtonCentered("Go Windowed", 190, 177 ) )
         {
            ToggleFullscreen();
            AddConsoleString("Please restart the game to switch to windowed mode.");
         }
      }
      else
      {
         if( DrawButtonCentered("Go Fullscreen", 190, 177 ) )
         {
            ToggleFullscreen();
            AddConsoleString("Please restart the game to switch to fullscreen mode.");
         }
      }


      if( DrawButtonCentered("Save Game", 190, 207 ) )
      {
         if( SaveGame() )
         {
            AddConsoleString("Game Saved.");
         }
         else
         {
            AddConsoleString("Save failed!");
         }
         
      }

      if( DrawButtonCentered("Load Game", 190, 227 ) )
      {
         if( LoadGame() )
         {
            AddConsoleString("Game Loaded.");
         }
         else
         {
            AddConsoleString("Load failed!");
         }
      }


      if( DrawButtonCentered("Main Menu", 190, 247 ) )
      {
         m_Returning = true;
      }

      if( DrawButtonCentered("Help", 190, 267 ) )
      {
         g_ShowBackstory = false;
         g_StateMachine->PopState();
         g_StateMachine->PushState(STATE_TUTORIALSTATE);
      }

      if( DrawButton("Return", 60 + (228 - GetStringMetrics(g_font.get(), g_fontSize, "Return")), 307 ) )
      {
         g_StateMachine->PopState();
      }

      if( DrawButton("Quit", 90, 307 ) )
      {
         m_Quitting = true;
      }

   }


   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());
}

void OptionsState::OnEnter()
{
   m_Quitting = false;
   m_Returning = false;
}

void OptionsState::OnExit(){}
void OptionsState::DoPlayerInput(){}

