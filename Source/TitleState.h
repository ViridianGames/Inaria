///////////////////////////////////////////////////////////////////////////
//
// Name:     MAINSTATE.H
// Author:   Anthony Salter
// Date:     2/03/05
// Purpose:  The game startup state.  This is the default state the
//           framework gives you so that the project will compile without
//           any modifications.  But it doesn't do anything...yet.
//
///////////////////////////////////////////////////////////////////////////
#ifndef _TITLESTATE_H_
#define _TITLESTATE_H_

#include "../Geist/Source/State.h"
#include "Map.h"
#include "NPC.h"
#include "../Geist/Source/Gui.h"
#include <list>
#include <deque>
#include <math.h>

class TitleState : public State
{
public:
   TitleState(){};
   ~TitleState();


   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   virtual void OnEnter();
   virtual void OnExit();
   void DoPlayerInput();

private:

   Gui* m_TitleGui;

   int m_ActiveGUIState;

   int m_TurnTimer;
   
   int m_OriginalX;
   int m_OriginalY;

   Texture* m_Background;
   Texture* m_Overlay;
   Texture* m_Logo;

   bool m_ShowingCredits;

   int m_TitleTimer;

   bool m_StartingNewGame;

};

#endif