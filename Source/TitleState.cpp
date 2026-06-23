#include "TitleState.h"
#include "GameGlobals.h"
#include "../Geist/Source/ResourceManager.h"
#include "../Geist/Source/Engine.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  TitleState
////////////////////////////////////////////////////////////////////////////////

TitleState::~TitleState()
{

}

void TitleState::Init(const std::string& configfile)
{

   m_Background = g_ResourceManager->GetTexture( "Images/background.png" );
   m_Overlay = g_ResourceManager->GetTexture( "Images/overlay.png" );
   m_Logo = g_ResourceManager->GetTexture("Images/logo.png");

   m_ShowingCredits = false;
   m_StartingNewGame = false;
}

void TitleState::Shutdown()
{

}

void TitleState::Update()
{

   if( static_cast<uint32_t>(g_Engine->GameTimeInMS()) - m_TitleTimer < 1000 )
      return;

   if( m_StartingNewGame && g_StateMachine->GetCurrentState() == STATE_TITLESTATE )
   {
      g_StateMachine->MakeStateTransition(STATE_MAINSTATE);
      g_StateMachine->PushState(STATE_LEVELUPSTATE);
      m_StartingNewGame = false;
   }


   //  Cull out any dead Animations
   list<InariaEffect*>::iterator AnimationNode;
   for(AnimationNode = g_AnimationList.begin(); AnimationNode != g_AnimationList.end();)
   {
      if((*AnimationNode)->IsDead())
      {
         delete (*AnimationNode);
         AnimationNode = g_AnimationList.erase(AnimationNode);
      }
      else
         ++AnimationNode;
   }



   //  Update all remaining animations.
   AnimationNode = g_AnimationList.begin();
   bool updatedblocker = false;
   for( AnimationNode; AnimationNode != g_AnimationList.end(); ++AnimationNode )
   {
      if( (*AnimationNode)->m_IsBlocking && !updatedblocker )
      {
         (*AnimationNode)->Update();
         updatedblocker = true;
      }
      else if( !(*AnimationNode)->m_IsBlocking )
      {
         (*AnimationNode)->Update();
      }
   }

   //  Only update everyone after a certain time.
   if( g_AnimationList.size() == 0 )
   {
         g_Map->Update();

   //  Cull out any dead items.
   list<Item*>::iterator itemNode;
   for(itemNode = g_Map->m_ItemList.begin(); itemNode != g_Map->m_ItemList.end();)
   {
      if((*itemNode)->IsDead())
      {
         delete (*itemNode);
         itemNode = g_Map->m_ItemList.erase(itemNode);
      }
      else
         ++itemNode;
   }

   itemNode = g_Map->m_ItemList.begin();
   for( itemNode; itemNode != g_Map->m_ItemList.end(); ++itemNode )
   {
      (*itemNode)->Update();
   }

   //  Cull out any dead NPCs
   list<NPC*>::iterator npcNode;
   for(npcNode = g_Map->m_NPCList.begin(); npcNode != g_Map->m_NPCList.end();)
   {
      if((*npcNode)->IsDead())
      {
         delete (*npcNode);
         npcNode = g_Map->m_NPCList.erase(npcNode);
      }
      else
         ++npcNode;
   }

   //  Cull out any dead inventory items
   vector<Item*>::iterator inventoryNode;
   for(inventoryNode = g_Player->m_PlayerInventory.begin(); inventoryNode != g_Player->m_PlayerInventory.end(); )
   {
      if( (*inventoryNode)->m_IsDead )
      {
         inventoryNode = g_Player->m_PlayerInventory.erase(inventoryNode);
      }
      else
         ++inventoryNode;
   }


      npcNode = g_Map->m_NPCList.begin();
      for( npcNode; npcNode != g_Map->m_NPCList.end(); ++npcNode )
      {
         (*npcNode)->Update();
      }
   }
}

void TitleState::Draw()
{
   DrawImageAt(m_Background, 0, 0);

   g_Map->Draw();

   //  Update all remaining animations.
   list<InariaEffect*>::iterator AnimationNode = g_AnimationList.begin();
   bool drewblocker = false;
   for( AnimationNode; AnimationNode != g_AnimationList.end(); ++AnimationNode )
   {
      if( (*AnimationNode)->m_IsBlocking && !drewblocker )
      {
         (*AnimationNode)->Draw();
         drewblocker = true;
      }
      else if( !(*AnimationNode)->m_IsBlocking )
      {
         (*AnimationNode)->Draw();
      }
   }

   DrawImageAt(m_Overlay, 0, 0);

   if( !m_ShowingCredits )
   {
      DrawImageAt(m_Logo, 405, 50 );

      if( DrawButtonCentered("New Game", 501, 130 ) )
      {
         m_StartingNewGame = true;
         g_Player->Init("");
         g_Map->LoadMap("Maps/inaria.map");

         g_Offset = (int)g_Engine->m_EngineConfig.GetNumber("offset");
         g_ViewRange = (int)g_Engine->m_EngineConfig.GetNumber("viewrange");

         g_InputSystem->DumpInput();

         g_AnimationList.clear();
         g_ConsoleStrings.clear();

         
         AddConsoleString("Welcome to Inaria!");

         g_FirstTime = true;

         if( g_Player->m_ShowTutorials )
            g_StateMachine->PushState(STATE_TUTORIALSTATE);

         g_Player->m_StatPoints = 5;

      }

      if( DrawButtonCentered("Continue", 501, 150 ) )
      {
         if( !LoadGame() )
         {
            m_StartingNewGame = true;
            g_Player->Init("");
            g_Map->LoadMap("Maps/inaria.map");

            g_Offset = (int)g_Engine->m_EngineConfig.GetNumber("offset");
            g_ViewRange = (int)g_Engine->m_EngineConfig.GetNumber("viewrange");

            g_InputSystem->DumpInput();

            g_AnimationList.clear();
            g_ConsoleStrings.clear();


            AddConsoleString("Welcome to Inaria!");

            g_FirstTime = true;

            if( g_Player->m_ShowTutorials )
               g_StateMachine->PushState(STATE_TUTORIALSTATE);

            g_Player->m_StatPoints = 5;
         }
         else
         {
            g_StateMachine->MakeStateTransition(STATE_MAINSTATE);
            g_Offset = (int)g_Engine->m_EngineConfig.GetNumber("offset");
            g_ViewRange = (int)g_Engine->m_EngineConfig.GetNumber("viewrange");

            g_InputSystem->DumpInput();

            g_AnimationList.clear();
            g_ConsoleStrings.clear();
            AddConsoleString("Welcome to Inaria!");
         }
      }

      if( DrawButtonCentered("Credits", 501, 280 ) )
      {
         m_ShowingCredits = true;

      }

      if( DrawButtonCentered("Quit", 501, 300 ) )
      {
         exit(0);
      }

      DrawTextAt(g_smallFont.get(), g_smallFontSize,  g_Version, 620 - GetStringMetrics(g_smallFont.get(), g_smallFontSize, g_Version), 350 );
      
      DrawConsoleStrings();
   }
   else
   {
      DrawImageResizedAt(g_Mask, 40, 20, 560, 430 );
      DrawBoxAt(40, 20, 560, 430);

      DrawTextCenteredAt(g_font.get(), g_fontSize, "INARIA", 320, 40, 255, 255, 64);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "by Anthony Salter", 320, 60, 255, 255, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "viridiangames.com", 320, 76, 255, 255, 128);

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Music by", 200, 100, 128, 255, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "H. Arnold Jones", 200, 120, 128, 255, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "cfxmusic.com", 200, 136, 128, 255, 128 );

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Additional Art by", 440, 100, 255, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Jeffrey Pfau", 440, 120, 255, 128, 255 );


      DrawTextCenteredAt(g_font.get(), g_fontSize, "8bitfunding.com Donors:", 200, 160, 255, 128, 128 );

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Noble Kale", 200, 180, 255, 128, 128 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "GBGames", 200, 196, 255, 128, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Joshua J. Slone", 200, 212, 255, 128, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "ChaosKnight127", 200, 228, 255, 128, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "D. Moonfire", 200, 244, 255, 128, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Scurvy Lobster", 200, 260, 255, 128, 128);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "(And several anonymous donors)", 200, 276, 255, 128, 128);

      DrawTextCenteredAt(g_font.get(), g_fontSize,  "Beta Testers:", 440, 160, 128, 128, 255);

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Henry Kropf",       390, 180, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "E. Roberts",        390, 196, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Seth Norton",       390, 212, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "ChaosKnight127",    390, 228, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Wolfengange",       390, 244, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "irwin1139",         390, 260, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Jari Komppa",       390, 276, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Marko Turunen",     390, 292, 128, 128, 255 );

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Wondermellon",      490, 180, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Dan Cardin",        490, 196, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Nick Anderson",     490, 212, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "H. Arnold Jones",   490, 228, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Martin Damiano",    490, 244, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Lorenzo Stoakes",   490, 260, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Jay Barnson",       490, 276, 128, 128, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Brian Critser",     490, 292, 128, 128, 255 );

      DrawTextCenteredAt(g_font.get(), g_fontSize, "For inspiring me and keeping me on track over the years:", 320, 314, 128, 255, 255 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Ryan Clark, Jay Barnson and Jari Komppa", 320, 334, 128, 255, 255 );

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Inaria Official Soundtrack available for purchase at:", 320, 360, 255, 192, 128 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "store.cfxmusic.com", 320, 380, 255, 192, 128 );


      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "\"I'd like to thank each and every person listed above.", 320, 400, 255, 255, 64 );
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Without you, Inaria would not be what it is today.\" - Anthony Salter", 320, 412, 255, 255, 64 );


      if( DrawButtonCentered("Wow, these are all such cool people!", 320, 430 ) )
      {
         m_ShowingCredits = false;
      }
   }

   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());

}

void TitleState::OnEnter()
{
   g_Map->LoadMap("Maps/intro.map");
   m_TitleTimer = static_cast<uint32_t>(g_Engine->GameTimeInMS());
   AddConsoleString("Slornite King: \"Your land, your goods, your souls - all mine!\"", 255, 128, 255 );


   g_ShowBackstory = true;

   //  Kill all the normal NPCs

   g_Map->m_NPCList.clear();

   //  Set up the combatants


   //  Bad guys

   // Thieves
   NPC* temp = UnitFactory(13);
   temp->m_PosX = 38;
   temp->m_PosY = 11;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(13);
   temp->m_PosX = 38;
   temp->m_PosY = 15;
   g_Map->m_NPCList.push_back(temp);

   // Mages
   temp = UnitFactory(18);
   temp->m_PosX = 38;
   temp->m_PosY = 12;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(18);
   temp->m_PosX = 38;
   temp->m_PosY = 14;
   g_Map->m_NPCList.push_back(temp);

   //  King
   temp = UnitFactory(15);
   temp->m_PosX = 37;
   temp->m_PosY = 13;
   g_Map->m_NPCList.push_back(temp);

   //  Fighters
   temp = UnitFactory(19);
   temp->m_PosX = 39;
   temp->m_PosY = 12;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(19);
   temp->m_PosX = 39;
   temp->m_PosY = 13;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(19);
   temp->m_PosX = 39;
   temp->m_PosY = 14;
   g_Map->m_NPCList.push_back(temp);

   //  Good guys

   // Healer
   temp = UnitFactory(35);
   temp->m_PosX = 45;
   temp->m_PosY = 13;
   g_Map->m_NPCList.push_back(temp);

   //  Archers
/*   temp = UnitFactory(34);
   temp->m_PosX = 44;
   temp->m_PosY = 11;
   g_Map->m_NPCList.push_back(temp);*/

   temp = UnitFactory(34);
   temp->m_PosX = 44;
   temp->m_PosY = 12;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(34);
   temp->m_PosX = 44;
   temp->m_PosY = 13;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(34);
   temp->m_PosX = 44;
   temp->m_PosY = 14;
   g_Map->m_NPCList.push_back(temp);

   /*temp = UnitFactory(34);
   temp->m_PosX = 44;
   temp->m_PosY = 15;
   g_Map->m_NPCList.push_back(temp);*/

   //  Fighters
   temp = UnitFactory(33);
   temp->m_PosX = 43;
   temp->m_PosY = 12;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(33);
   temp->m_PosX = 43;
   temp->m_PosY = 13;
   g_Map->m_NPCList.push_back(temp);

   temp = UnitFactory(33);
   temp->m_PosX = 43;
   temp->m_PosY = 14;
   g_Map->m_NPCList.push_back(temp);


   for( list<NPC*>::iterator node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
   {
      //  War.
      (*node)->m_AI = 3;
   }

   g_Map->m_DrawInvisibles = false;
   g_Map->m_Peer = false;


   m_OriginalX = g_Player->m_PlayerPosX;
   m_OriginalY = g_Player->m_PlayerPosY;

   g_Player->m_PlayerPosX = 41;
   g_Player->m_PlayerPosY = 12;

//   g_Offset = 0;
//   g_ViewRange = 10;
}

void TitleState::OnExit()
{


}

void TitleState::DoPlayerInput()
{

}
