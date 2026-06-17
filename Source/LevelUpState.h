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
#ifndef _LEVELUPSTATE_H_
#define _LEVELUPSTATE_H_

#include "Geist/State.h"
#include "Map.h"
#include "NPC.h"
#include "Geist/Gui.h"
#include <list>
#include <deque>
#include <math.h>

class LevelUpState : public State
{
public:
   LevelUpState(){};
   ~LevelUpState();


   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   virtual void OnEnter();
   virtual void OnExit();
   void DoPlayerInput();

private:

   Texture* m_LevelUp;

   int m_StrAdd;
   int m_DexAdd;
   int m_EndAdd;
   int m_WilAdd;
   int m_IntAdd;

   int m_PosX;
   int m_PosY;

   int m_StatsY;


   int m_OriginalStatPoints;

   std::vector<int> m_BoughtAbilities;

};

#endif