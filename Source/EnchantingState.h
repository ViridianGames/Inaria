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
#ifndef _EnchantingSTATE_H_
#define _EnchantingSTATE_H_

#include "Geist/State.h"
#include "Map.h"
#include "NPC.h"
#include "Geist/Gui.h"
#include <list>
#include <deque>
#include <math.h>

class EnchantingState : public State
{
public:
   EnchantingState(){};
   ~EnchantingState();


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

   Gui* m_GameGui;
   Gui* m_LevelUpGui;
   Gui* m_EnchantingGui;

   int m_ActiveGUIState;

   int m_TurnTimer;

   int x, y;

   std::vector<Item*> m_EnchantingInventory;
};

#endif