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
#ifndef _MAINSTATE_H_
#define _MAINSTATE_H_

#include "../Geist/Source/State.h"
#include "Map.h"
#include "NPC.h"
#include "../Geist/Source/Gui.h"
#include <list>
#include <deque>
#include <math.h>

class MainState : public State
{
public:
   MainState(){};
   ~MainState();


   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   virtual void OnEnter();
   virtual void OnExit();

   void DoPlayerInput();

   bool DoInventoryUse();   //DONE
   bool DoItemPickup();     //DONE
   bool DoLookAt();
   bool DoNPCTalk();
   bool DoNPCFight();
   bool DoMissileCombat();
   bool DoMeleeCombat();

   //  Non-spell special abilities
   bool DoBash();
   bool DoPickLock();

   //  Spells
   bool DoSmite();
   bool DoHeal();
   bool DoExtraDamage();
   bool DoMagicArmor();
   bool DoMagicUnlock();
   bool DoMap();
   bool DoTeleport();
   bool DoTownPortal();

   

private:

   Texture* m_Background;
   Texture* m_LevelUp;
	Texture* m_Buy;
   Texture* m_Overlay;
   Texture* m_Crossbar;
   Texture* m_Button1;
   Texture* m_Button2;

   Gui* m_GameGui;
   Gui* m_LevelUpGui;
   Gui* m_StoreGui;

   int m_ActiveGUIState;

   uint32_t m_PlayerMoveTimer;
   bool m_AbilityInventorySwitch;

   uint32_t m_UpdateTimer;
   uint32_t m_DrawTimer;

   

};

#endif