///////////////////////////////////////////////////////////////////////////
//
// Name:     EditorState.H
// Author:   Anthony Salter
// Date:     2/03/05
// Purpose:  The game startup state.  This is the default state the
//           framework gives you so that the project will compile without
//           any modifications.  But it doesn't do anything...yet.
//
///////////////////////////////////////////////////////////////////////////
#ifndef _EditorState_H_
#define _EditorState_H_

#include "Geist/State.h"
#include "InariaCompat.h"
#include "Map.h"
#include "NPC.h"
#include "Item.h"
#include "Trigger.h"
#include "Geist/Gui.h"
#include <list>
#include <deque>
#include <math.h>

enum MapModes
{
   MM_DrawTerrain = 0,
   MM_DropItem,
   MM_DropNPC,
   MM_PlacingTriggerSource,
   MM_PlacingTriggerDestination,
   MM_DeleteMode,
   MM_PlacingMapLinkSource,
   MM_PlacingMapLinkDestination
};

class EditorState : public State
{
public:
   EditorState(){};
   ~EditorState();


   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   virtual void OnEnter();
   virtual void OnExit();
   void DoPlayerInput();

private:

   std::list<Item*> m_AllItems;
   std::list<NPC*> m_AllNPCs;
   std::list<Trigger*> m_AllTriggers;

   int m_MapMode;

   Item* m_CurrentItem;
   NPC* m_CurrentNPC;
   Trigger* m_CurrentTrigger;
   int m_CurrentTerrainType;

   int m_OriginalMap;
   MapLink* m_CurrentMapLink;

   int m_ItemListX;
   int m_ItemListY;
};

#endif