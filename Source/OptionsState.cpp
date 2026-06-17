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
   if( g_Input->WasKeyPressed(SDLK_ESCAPE) )
   {
      g_StateMachine->PopState();
   }

}
void OptionsState::Draw()
{
   g_Display->BlitImageRect( g_Mask, 0, 0, 228, 276, 76, 57);
   g_Display->DrawBox(76, 57, 228, 276);

   stringstream temp;

   g_Font->DrawTextCentered("Options", 190, 67 );

   if( m_Returning )
   {

      g_Font->DrawTextCentered("Return to the", 190, 130 );
      g_Font->DrawTextCentered("Main Menu? You will lose", 190, 150 );
      g_Font->DrawTextCentered("any unsaved progress.", 190, 170 );

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

      g_Font->DrawTextCentered("Do you wish to quit", 190, 130 );
      g_Font->DrawTextCentered("the game? You will lose", 190, 150 );
      g_Font->DrawTextCentered("any unsaved progress.", 190, 170 );

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
      g_Font->DrawTextCentered("Sound Volume", 190, 97 );

      if( DrawButton("<-", 110, 97 ) )
      {
         int currentvolume = g_Sound->GetSoundVolume();
         g_Sound->SetSoundVolume( currentvolume -= 8);
         g_Sound->Play("Sounds/level_up.wav");
      }

      if( DrawButton("->", 253, 97 ) )
      {
         int currentvolume = g_Sound->GetSoundVolume();
         g_Sound->SetSoundVolume( currentvolume += 8);
         g_Sound->Play("Sounds/level_up.wav");
      }

      g_Font->DrawTextCentered("Music Volume", 190, 117 );
      if( DrawButton("<-", 110, 117 ) )
      {
         int currentvolume = g_Sound->GetMusicVolume();
         g_Sound->SetMusicVolume( currentvolume -= 8);
      }

      if( DrawButton("->", 253, 117 ) )
      {
         int currentvolume = g_Sound->GetMusicVolume();
         g_Sound->SetMusicVolume( currentvolume += 8);
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

      if( g_Display->m_FullScreen )
      {
         if( DrawButtonCentered("Go Windowed", 190, 177 ) )
         {
            g_Display->ChangeToWindowed();
            AddConsoleString("Please restart the game to switch to windowed mode.");
         }
      }
      else
      {
         if( DrawButtonCentered("Go Fullscreen", 190, 177 ) )
         {
            g_Display->ChangeToFullscreen();
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

      if( DrawButton("Return", 60 + (228 - g_Font->GetStringMetrics("Return")), 307 ) )
      {
         g_StateMachine->PopState();
      }

      if( DrawButton("Quit", 90, 307 ) )
      {
         m_Quitting = true;
      }

   }


   g_Display->BlitImage(g_Cursors[0], g_Input->m_MouseX, g_Input->m_MouseY);
}

void OptionsState::OnEnter()
{
   m_Quitting = false;
   m_Returning = false;
}

void OptionsState::OnExit(){}
void OptionsState::DoPlayerInput(){}

