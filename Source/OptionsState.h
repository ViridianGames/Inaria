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
#ifndef _OPTIONSSTATE_H_
#define _OPTIONSSTATE_H_

#include "Geist/State.h"
#include "InariaCompat.h"
#include "Map.h"
#include "NPC.h"
#include "Geist/Gui.h"
#include <list>
#include <deque>
#include <math.h>

class OptionsState : public State
{
public:
   OptionsState(){};
   ~OptionsState();


   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   virtual void OnEnter();
   virtual void OnExit();
   void DoPlayerInput();

private:

   Texture* m_Background;
   Texture* m_LevelUp;
	Texture* m_Buy;

   Gui* m_GameGui;
   Gui* m_LevelUpGui;
   Gui* m_OptionsGui;

   int m_ActiveGUIState;

   int m_TurnTimer;

   bool m_Quitting;
   bool m_Returning;
};

#endif