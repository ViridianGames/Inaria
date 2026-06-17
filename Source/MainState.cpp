#include "MainState.h"
#include "GameGlobals.h"
#include "Geist/ResourceManager.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  MainState
////////////////////////////////////////////////////////////////////////////////

MainState::~MainState()
{
   Shutdown();
}

void MainState::Init(const std::string& configfile)
{
   m_Background = g_ResourceManager->GetTexture( "Images/background.png" );
   m_LevelUp = g_ResourceManager->GetTexture( "Images/levelup.png" );
   m_Buy = g_ResourceManager->GetTexture( "Images/buy.png" );
   m_Overlay = g_ResourceManager->GetTexture( "Images/overlay.png" );
   m_Crossbar = g_ResourceManager->GetTexture( "Images/crossbar.png" );
   m_Button1 = g_ResourceManager->GetTexture( "Images/button1.png" );
   m_Button2 = g_ResourceManager->GetTexture( "Images/button2.png" );

   g_Cursors[0] = g_ResourceManager->GetTexture( "Images/cursor.png" );
   g_Cursors[1] = g_ResourceManager->GetTexture( "Images/getcursor.png" );
   g_Cursors[2] = g_ResourceManager->GetTexture( "Images/talkcursor.png" );
   g_Cursors[3] = g_ResourceManager->GetTexture( "Images/attackcursor.png" );
   g_Cursors[4] = g_ResourceManager->GetTexture( "Images/lookcursor.png" );
   g_Cursors[5] = g_ResourceManager->GetTexture( "Images/bashcursor.png" );
   g_Cursors[6] = g_ResourceManager->GetTexture( "Images/smitecursor.png" );
   g_Cursors[7] = g_ResourceManager->GetTexture( "Images/teleportcursor.png");
   g_Cursors[8] = g_ResourceManager->GetTexture( "Images/lockpickcursor.png");
   g_Cursors[9] = g_ResourceManager->GetTexture( "Images/magicunlockcursor.png");


   g_Map = new Map;
   g_Map->Init("");
   g_Map->LoadMap("Maps/inaria.map");


   g_Player = new Player;
   g_Player->Init("");

   g_IsPlayerTurn = TRUE;

   m_GameGui = new Gui;
   m_GameGui->Init("Data/game.txt");
   m_GameGui->m_Editing = true;

   g_InventoryX = 414;
   g_InventoryY = 175;
   g_InventoryTileSize = 32;

   AddConsoleString("Welcome to Inaria!");

   m_ActiveGUIState = 0;

   m_PlayerMoveTimer = SDL_GetTicks();

   m_AbilityInventorySwitch = false;

   //  Tell the player they have received new abilities if they did.
/*   if( g_Player->m_Intelligence >= 2 )
   {
      g_SpecialAbilities[0].m_Active = true;
      if( g_SpecialAbilities[0].m_Level == 0)
         g_SpecialAbilities[0].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 3 )
   {
      g_SpecialAbilities[1].m_Active = true;
      if( g_SpecialAbilities[1].m_Level == 0)
         g_SpecialAbilities[1].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 4 )
   {
      g_SpecialAbilities[2].m_Active = true;
      if( g_SpecialAbilities[2].m_Level == 0)
         g_SpecialAbilities[2].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 5 )
   {
      g_SpecialAbilities[3].m_Active = true;
      if( g_SpecialAbilities[3].m_Level == 0)
         g_SpecialAbilities[3].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 6 )
   {
      g_SpecialAbilities[4].m_Active = true;
      if( g_SpecialAbilities[4].m_Level == 0)
         g_SpecialAbilities[4].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 7 )
   {
      g_SpecialAbilities[5].m_Active = true;
      if( g_SpecialAbilities[5].m_Level == 0)
         g_SpecialAbilities[5].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 8 )
   {
      g_SpecialAbilities[6].m_Active = true;
      if( g_SpecialAbilities[6].m_Level == 0)
         g_SpecialAbilities[6].m_Level = 1;
   }

   if( g_Player->m_Intelligence >= 9 )
   {
      g_SpecialAbilities[7].m_Active = true;
      if( g_SpecialAbilities[7].m_Level == 0)
         g_SpecialAbilities[7].m_Level = 1;
   }

   if( g_Player->m_Strength >= 5 )
   {
      g_SpecialAbilities[8].m_Active = true;
      if( g_SpecialAbilities[8].m_Level == 0)
         g_SpecialAbilities[8].m_Level = 1;
   }

   if( g_Player->m_Strength >= 7 )
   {
      g_SpecialAbilities[9].m_Active = true;
      if( g_SpecialAbilities[9].m_Level == 0)
         g_SpecialAbilities[9].m_Level = 1;
   }

   if( g_Player->m_Dexterity >= 5 )
   {
      g_SpecialAbilities[10].m_Active = true;
      if( g_SpecialAbilities[10].m_Level == 0)
         g_SpecialAbilities[10].m_Level = 1;
   }

   if( g_Player->m_Dexterity >= 7 )
   {
      g_SpecialAbilities[11].m_Active = true;
      if( g_SpecialAbilities[11].m_Level == 0)
         g_SpecialAbilities[11].m_Level = 1;
   }

   if( g_Player->m_Will >= 5 )
   {
      g_SpecialAbilities[12].m_Active = true;
      if( g_SpecialAbilities[12].m_Level == 0)
         g_SpecialAbilities[12].m_Level = 1;
   }

   if( g_Player->m_Will >= 7 )
   {
      g_SpecialAbilities[13].m_Active = true;
      if( g_SpecialAbilities[13].m_Level == 0)
         g_SpecialAbilities[13].m_Level = 1;
   }

   if( g_Player->m_Endurance >= 5 )
   {
      g_SpecialAbilities[14].m_Active = true;
      if( g_SpecialAbilities[14].m_Level == 0)
         g_SpecialAbilities[14].m_Level = 1;
   }
      
   if( g_Player->m_Endurance >= 7 )
   {
      g_SpecialAbilities[15].m_Active = true;
      if( g_SpecialAbilities[15].m_Level == 0)
         g_SpecialAbilities[15].m_Level = 1;
   }*/

   g_IsFullscreen = false;
}

void MainState::OnEnter()
{
   m_AbilityInventorySwitch = false;
   g_DanceParty = false;
}

void MainState::OnExit()
{
   g_DanceParty = false;

}

void MainState::Shutdown()
{

}

void MainState::Update()
{
   g_CheckPathCount = 0;
   g_LineLengthCount = 0;
   m_UpdateTimer = SDL_GetTicks();
   if( g_Input->WasKeyPressed(SDLK_ESCAPE) )
   {
      g_StateMachine->PushState(STATE_OPTIONSSTATE);
   }

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

   if( g_Player->m_IsDead )
   {
      return;
   }

   //  Only update everyone after a certain time.
   if( g_AnimationList.empty() )
   {
      if( g_IsPlayerTurn )
      {
         DoPlayerInput();
         g_Player->Update();
      }
      //  Update everyone else
      else if( !g_IsPlayerTurn)
      {
         for(int i = 0; i < 16; ++i)
         {
            if( g_SpecialAbilities[i].m_Duration > 0 )
               --g_SpecialAbilities[i].m_Duration;
         }

         if( g_Player->m_SleepCounter > 0 )
            --g_Player->m_SleepCounter;

         if( g_Player->m_PoisonCounter > 0 )
         {
            int beforehits = g_Player->m_CurrentHitPoints;
            g_Player->m_CurrentHitPoints -= HalfNHalf( g_Player->m_CurrentHitPoints / 10 );
            AddConsoleString("POISONED!", 255, 64, 64);

            if( float(beforehits) / float(g_Player->m_MaxHitPoints) > .33 && float( g_Player->m_CurrentHitPoints)/ float(g_Player->m_MaxHitPoints) < .22 )
            {
               AddConsoleString("LOW HEALTH WARNING!", 255, 64, 64);
            }

            if(g_Player->m_CurrentHitPoints <= 0)
            {
               g_Player->m_CurrentHitPoints = 0;
               AddConsoleString("You have been killed!");
               g_Player->m_IsDead = true;
               g_Sound->StopMusic();
               g_CurrentMusic = -1;
               g_Sound->Play("Sounds/inaria-death_withfade.wav");
            }
            --g_Player->m_PoisonCounter;
            if( g_Player->m_PoisonCounter == 0 && !g_Player->m_IsDead )
            {
               AddConsoleString("The poison has worn off.");
            }
         }

         npcNode = g_Map->m_NPCList.begin();
         for( npcNode; npcNode != g_Map->m_NPCList.end(); ++npcNode )
         {
            (*npcNode)->Update();
         }

         itemNode = g_Map->m_ItemList.begin();
         for( itemNode; itemNode != g_Map->m_ItemList.end(); ++itemNode )
         {
            (*itemNode)->Update();
         }

         g_Player->Update();
         g_Player->m_IsPerceptive = DoRPGCheck( g_Player->GetIntelligenceWithBonus(), 20 - g_Player->GetIntelligenceWithBonus() );
         g_IsPlayerTurn = TRUE;
      }
   }

   g_Map->Update();

   m_UpdateTimer = SDL_GetTicks() - m_UpdateTimer;
}


void MainState::Draw()
{
   m_DrawTimer = SDL_GetTicks();
   g_Display->BlitImage( m_Background, 0, 0 );


   DrawConsoleStrings();
   //  Draw the map

   g_Map->Draw();

   //  Draw the Player
   g_Player->Draw();

   if( m_AbilityInventorySwitch == false )
   {
      //  Draw the player's inventory
      for(unsigned int invcount = 0; invcount < 16; ++invcount)
      {
         if(invcount < g_Player->m_PlayerInventory.size())
         {
            g_Display->DrawSprite( g_Tiles[g_Player->m_PlayerInventory[invcount]->m_Tile], g_InventoryX + ((invcount % 4) * g_InventoryTileSize), g_InventoryY + ((invcount / 4) * g_InventoryTileSize) );
            if( g_Player->m_PlayerInventory[invcount]->m_Stackable )
            {
               stringstream tempstream;
               tempstream << g_Player->m_PlayerInventory[invcount]->m_CurrentNumber;
               g_SmallFont->DrawTextA( tempstream.str(), g_InventoryX + ((invcount % 4) * g_InventoryTileSize) + 23, g_InventoryY + ((invcount / 4) * g_InventoryTileSize) + 23, 0, 0, 0 );
               g_SmallFont->DrawTextA( tempstream.str(), g_InventoryX + ((invcount % 4) * g_InventoryTileSize) + 22, g_InventoryY + ((invcount / 4) * g_InventoryTileSize) + 22 );
            }
         }
         else
         {
            g_Display->DrawSprite( g_Tiles[70], g_InventoryX + ((invcount % 4) * g_InventoryTileSize), g_InventoryY + ((invcount / 4) * 32) );
         }
      }


      //  Weapon and Armor Slots

      g_SmallFont->DrawTextA("Weapon", g_InventoryX + 162 - (g_SmallFont->GetStringMetrics("Weapon") / 2), g_InventoryY + 6);
      if(g_Player->m_CurrentWeaponType == NULL)
      {
         g_Display->DrawSprite(g_Tiles[70], g_InventoryX + 144, g_InventoryY + 10 );
      }
      else
      {
         g_Display->DrawSprite(g_Tiles[g_Player->m_CurrentWeaponType->m_Tile], g_InventoryX + 144, g_InventoryY + 10 );
      }

      g_SmallFont->DrawTextA("Armor", g_InventoryX + 162 - (g_SmallFont->GetStringMetrics("Armor") / 2), g_InventoryY + 48 );
      if(g_Player->m_CurrentArmorType == NULL)
      {
         g_Display->DrawSprite(g_Tiles[70], g_InventoryX + 144, g_InventoryY + 53);
      }
      else
      {
         g_Display->DrawSprite(g_Tiles[g_Player->m_CurrentArmorType->m_Tile], g_InventoryX + 144, g_InventoryY + 53);
      }

      g_SmallFont->DrawTextA("Trinket", g_InventoryX + 162 - (g_SmallFont->GetStringMetrics("Trinket") / 2), g_InventoryY + 90);
      if(g_Player->m_CurrentTrinketType == NULL)
      {
         g_Display->DrawSprite(g_Tiles[70], g_InventoryX + 144, g_InventoryY + 96);
      }
      else
      {
         g_Display->DrawSprite(g_Tiles[g_Player->m_CurrentTrinketType->m_Tile], g_InventoryX + 144, g_InventoryY + 96);
      }
   }
   else
   {
      //  Draw the abilities list
      for(unsigned int invcount = 0; invcount < 16; ++invcount)
      {
         if( g_SpecialAbilities[invcount].m_Active == true )
         {
            g_Display->DrawSprite( g_Tiles[ g_SpecialAbilities[invcount].m_Tile], g_InventoryX + ((invcount % 4) * 47), g_InventoryY + ((invcount / 4) * g_InventoryTileSize) );
         }
         else
         {
            g_Display->DrawSprite( g_Tiles[ g_SpecialAbilities[invcount].m_Tile], g_InventoryX + ((invcount % 4) * 47), g_InventoryY + ((invcount / 4) * g_InventoryTileSize), 128, 128, 128 );
         }
      }
   }


   //  Draw the stats bar

   char buffer[64];

   static int xoffset = 384;
   static int yoffset = 20;

   //  Line 1

   //  Line 1 - Max HP and Max MP
   snprintf(buffer, 64, "%d/%d", g_Player->m_CurrentHitPoints, g_Player->m_MaxHitPoints);
   g_Display->DrawSprite(g_Tiles[73], xoffset, yoffset);
   float _endingpercent = (float)g_Player->m_CurrentHitPoints / (float)g_Player->m_MaxHitPoints;
   if( _endingpercent <= .33 || g_Player->m_PoisonCounter > 0 )
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset, 255, 64, 64);
   else
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset);


   snprintf(buffer, 64, "%d/%d", g_Player->m_CurrentManaPoints, g_Player->m_MaxManaPoints);
   g_Display->DrawSprite(g_Tiles[72], xoffset + 98, yoffset);
   g_Font->DrawTextA(buffer, xoffset + 115, yoffset);


   //  Line 2 - STR/Damage

   snprintf(buffer, 64, "%d", g_Player->GetStrengthWithBonus() );
   g_Display->DrawSprite(g_Tiles[64], xoffset, yoffset + 18);
   if( g_Player->GetStrengthBonus() > 0 )
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 18, 128, 255, 128);
   else
      g_Font->DrawTextA( buffer, xoffset + 17, yoffset + 18 );


   snprintf(buffer, 64, "%d", g_Player->GetDamage());
   g_Display->DrawSprite(g_Tiles[75], xoffset + 98, yoffset + 18);
   if ( g_SpecialAbilities[ABILITY_DAMAGEBONUS].m_Duration > 0 )
      g_Font->DrawTextA(buffer, xoffset + 115, yoffset + 18, 128, 255, 128 );
   else
      g_Font->DrawTextA(buffer, xoffset + 115, yoffset + 18);


   //  Line 3 - DEX/Armor
   snprintf(buffer, 64, "%d", g_Player->GetDexterityWithBonus());
   g_Display->DrawSprite(g_Tiles[107], xoffset, yoffset + 36);
   if( g_Player->GetDexterityBonus() > 0 )
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 36, 128, 255, 128);
   else
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 36);

   snprintf(buffer, 64, "%d", g_Player->GetArmor() );
   g_Display->DrawSprite(g_Tiles[66], xoffset + 98, yoffset + 36);
   if( g_SpecialAbilities[ABILITY_MAGICARMOR].m_Duration > 0 )
      g_Font->DrawTextA(buffer, xoffset + 115, yoffset + 36, 128, 255, 128);
   else
      g_Font->DrawTextA(buffer, xoffset + 115, yoffset + 36);


   //  Line 4 - INT/XP
   snprintf(buffer, 64, "%d", g_Player->GetIntelligenceWithBonus() );
   g_Display->DrawSprite(g_Tiles[109], xoffset, yoffset + 54);
   if( g_Player->GetIntelligenceBonus() > 0 )
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 54, 128, 255, 128);
   else
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 54);

   snprintf(buffer, 64, "%d/%d", g_Player->m_CurrentXP, g_Player->m_NextLevel);
   g_Display->DrawSprite(g_Tiles[74], xoffset + 98, yoffset + 54);
   g_Font->DrawTextA(buffer, xoffset + 115, yoffset + 54);


   //  Line 5 - Endurance/Level
   snprintf(buffer, 64, "%d", g_Player->GetEnduranceWithBonus() );
   g_Display->DrawSprite(g_Tiles[108], xoffset, yoffset + 72);
   if( g_Player->GetEnduranceBonus() > 0 )
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 72, 128, 255, 128);
   else
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 72);

   snprintf(buffer, 64, "%d", g_Player->m_Level);
   g_Display->DrawSprite(g_Tiles[67], xoffset + 98, yoffset+72);
   g_Font->DrawTextA(buffer, xoffset + 115, yoffset+72);


   //  Line 6 - Will/Coins
   snprintf(buffer, 64, "%d", g_Player->GetWillWithBonus() );
   g_Display->DrawSprite(g_Tiles[110], xoffset, yoffset + 90);
   if( g_Player->GetWillBonus() > 0 )
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 90, 128, 255, 128);
   else
      g_Font->DrawTextA(buffer, xoffset + 17, yoffset + 90);

   snprintf(buffer, 64, "%d", g_Player->m_CurrentGold);
   g_Display->DrawSprite(g_Tiles[65], xoffset + 98, yoffset+90);
   g_Font->DrawTextA(buffer, xoffset + 115, yoffset+90);


   //  Draw the GUI
   if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE )
   {
      int m_PreviousActiveState = m_ActiveGUIState;

      m_GameGui->Draw();
      m_GameGui->Update();

      for( auto& guinode : m_GameGui->m_GuiElementList )
      {
         if( guinode.second->m_Type == GUI_ICONBUTTON )
         {
            if( guinode.second->GetValue() )
            {
               m_ActiveGUIState = guinode.second->m_ID;
            }
         }
      }

      //  Can't do anything when dead except bring up the options menu.
      if( g_Player->m_IsDead && m_ActiveGUIState != GUI_QUIT )
         m_ActiveGUIState = 0;

      if( m_PreviousActiveState != m_ActiveGUIState )
      {
         switch( m_ActiveGUIState )
         {
         case GUI_GET:
            AddConsoleString("Click what you wish to get or use.  Right-click to cancel.");
            break;

         case GUI_FIGHT:
            AddConsoleString("Click what you wish to attack.  Right-click to cancel.");
            break;

         case GUI_TALK:
            AddConsoleString("Click who you wish to talk to.  Right-click to cancel.");
            break;

         case GUI_LOOK:
            AddConsoleString("Click what you wish to look at.  Right-click to cancel.");
            break;

         case GUI_LOAD:
            if( LoadGame() )
               AddConsoleString("Game loaded.");
            else
               AddConsoleString("Load failed!");
            m_ActiveGUIState = GUI_NOSTATE;
            break;

         case GUI_SAVE:
            if( SaveGame() )
               AddConsoleString("Game saved.");
            else
               AddConsoleString("Save failed!");
            m_ActiveGUIState = GUI_NOSTATE;
            break;

         case GUI_QUIT:
            g_StateMachine->PushState(STATE_OPTIONSSTATE);
            m_ActiveGUIState = GUI_NOSTATE;
            break;

         case GUI_DAMAGESPELL:
            break;

         case GUI_ARMORSPELL:
            break;

         case GUI_SMITESPELL:
            break;

         case GUI_HEALSPELL:
            break;

         case GUI_MAGICUNLOCK:
            break;

         case GUI_MAGICMAP:
            break;

         case GUI_TELEPORT:
            break;

         case GUI_TOWNPORTAL:
            break;

         case GUI_BASH:
            break;

         case GUI_PICKLOCK:
            break;

         }
      }
   }

   //  Draw the "Level Up" button.
   if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE )
   {
      if( g_Player->m_StatPoints > 0 )
      {
         int glower = (SDL_GetTicks() / 5) % 128;
         if( glower > 64 )
            glower = ( 128 - glower );
         glower += 127;

         g_Display->DrawSprite( g_Tiles[g_Player->m_PlayerSprite], 382 + 208 - 36, 328,
            glower, glower, glower);
      }
      else
      {
         g_Display->DrawSprite( g_Tiles[g_Player->m_PlayerSprite], 382 + 208 - 36, 328, 128, 128, 128);
      }
   }

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

   //  Draw Tooltips

   g_Display->BlitImage( m_Overlay, 0, 0 );

   g_Display->BlitImage( m_Crossbar, 372, 313 );

   if( !m_AbilityInventorySwitch )
      g_Display->BlitImage( m_Button1, 372, 133 );
   else
      g_Display->BlitImage( m_Button2, 372, 133 );





   if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE )
   {
      //  Do inventory tooltips.
      if( m_AbilityInventorySwitch == false && g_Input->IsMouseInRegion( g_InventoryX, g_InventoryY, g_InventoryX + (4 * 32), g_InventoryY + ( 4 * 32 ) ) )
      {
         //  Turn the screen coordinates into inventory coordinates.
         int mapx = ((g_Input->m_MouseX - g_InventoryX) / 32);
         int mapy = ((g_Input->m_MouseY - g_InventoryY) / 32);
         int inventoryindex = mapy * 4 + mapx;

         if(inventoryindex < g_Player->m_PlayerInventory.size())
         {
            DrawToolTip( g_SmallFont, ConstructItemTooltip(g_Player->m_PlayerInventory[inventoryindex]), g_InventoryX + mapx * 32, g_InventoryY + mapy * 32, 2 );
         }
      }
      //  Do ability tooltips.
      else if( m_AbilityInventorySwitch == true && g_Input->m_MouseX > g_InventoryX && g_Input->m_MouseX < g_InventoryX + 4 * 47 && g_Input->m_MouseY > g_InventoryY && g_Input->m_MouseY < g_InventoryY + 4 * 32 )
      {
         //  Turn the screen coordinates into inventory coordinates.
         int mapx = ((g_Input->m_MouseX - g_InventoryX) / 47);
         int mapy = ((g_Input->m_MouseY - g_InventoryY) / 32);
         int inventoryindex = mapy * 4 + mapx;

//         if( g_SpecialAbilities[inventoryindex].m_Active )
//         {
            DrawToolTip( g_SmallFont, ConstructAbilityTooltip(inventoryindex), g_InventoryX + mapx * 47, g_InventoryY + mapy * 32, 2 );
//         }
      }

      //  Do equipped item tooltips
      if( m_AbilityInventorySwitch == false && g_Input->IsMouseInRegion( g_InventoryX + 144, g_InventoryY + 10, g_InventoryX + 144 + 32, g_InventoryY + 10 + 32 ) && g_Player->m_CurrentWeaponType )
      {
         DrawToolTip( g_SmallFont, ConstructItemTooltip(g_Player->m_CurrentWeaponType), g_InventoryX + 144, g_InventoryY + 10, 2 );
      }

      if( m_AbilityInventorySwitch == false && g_Input->IsMouseInRegion( g_InventoryX + 144, g_InventoryY + 53, g_InventoryX + 144 + 32, g_InventoryY + 53 + 32 ) && g_Player->m_CurrentArmorType )
      {
         DrawToolTip( g_SmallFont, ConstructItemTooltip(g_Player->m_CurrentArmorType), g_InventoryX + 144, g_InventoryY + 53, 2 );
      }

      if( m_AbilityInventorySwitch == false && g_Input->IsMouseInRegion( g_InventoryX + 144, g_InventoryY + 96, g_InventoryX + 144 + 32, g_InventoryY + 96 + 32 ) && g_Player->m_CurrentTrinketType )
      {
         DrawToolTip( g_SmallFont, ConstructItemTooltip(g_Player->m_CurrentTrinketType), g_InventoryX + 144, g_InventoryY + 96, 2 );
      }

      //  Do stat tooltips
      //  Line 1 - Max HP and Max MP
      if( g_Input->IsMouseInRegion( xoffset, yoffset, xoffset + 16, yoffset + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Current/Max HP", xoffset, yoffset, 1 );
      }

      if( g_Input->IsMouseInRegion( xoffset + 98, yoffset, xoffset + 98 + 16, yoffset + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Current/Max MP", xoffset + 98, yoffset, 1 );
      }

      //  Line 2 - STR/Damage
      if( g_Input->IsMouseInRegion( xoffset, yoffset + 18, xoffset + 16, yoffset + 18 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Strength", xoffset, yoffset + 18, 1 );
      }

      if( g_Input->IsMouseInRegion( xoffset + 98, yoffset + 18, xoffset + 98 + 16, yoffset + 18 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Damage", xoffset + 98, yoffset + 18, 1 );
      }



      //  Line 3 - DEX/Armor
      if( g_Input->IsMouseInRegion( xoffset, yoffset + 36, xoffset + 16, yoffset + 36 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Dexterity", xoffset, yoffset + 36, 1 );
      }

      if( g_Input->IsMouseInRegion( xoffset + 98, yoffset + 36, xoffset + 98 + 16, yoffset + 36 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Armor", xoffset + 98, yoffset + 36, 1 );
      }


      //  Line 4 - INT/XP

      if( g_Input->IsMouseInRegion( xoffset, yoffset + 54, xoffset + 16, yoffset + 54 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Intelligence", xoffset, yoffset + 54, 1 );
      }

      if( g_Input->IsMouseInRegion( xoffset + 98, yoffset + 54, xoffset + 98 + 16, yoffset + 54 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Current XP/XP To Next Level", xoffset + 98, yoffset + 54, 1 );
      }

      //  Line 5 - Endurance/Level
      if( g_Input->IsMouseInRegion( xoffset, yoffset + 72, xoffset + 16, yoffset + 72 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Endurance", xoffset, yoffset + 72, 1 );
      }

      if( g_Input->IsMouseInRegion( xoffset + 98, yoffset + 72, xoffset + 98 + 16, yoffset + 72 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Level", xoffset + 98, yoffset + 72, 1 );
      }


      //  Line 6 - Will/Coins
      if( g_Input->IsMouseInRegion( xoffset, yoffset + 90, xoffset + 16, yoffset + 90 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Will", xoffset, yoffset + 90, 1 );
      }

      if( g_Input->IsMouseInRegion( xoffset + 98, yoffset + 90, xoffset + 98 + 16, yoffset + 90 + 16 ) )
      {
         DrawToolTip( g_SmallFont, "Viridian Coins", xoffset + 98, yoffset + 90, 1 );
      }

      if( g_Input->IsMouseInRegion( 382, 328, 382 + 31, 328 + 32 ) )
      {
         DrawToolTip( g_SmallFont, "Get", 382, 328, 2 );
      }

      if( g_Input->IsMouseInRegion( 382 + 32, 328, 382 + 32 + 31, 328 + 32 ) )
      {
         DrawToolTip( g_SmallFont, "Fight", 382 + 32, 328, 2 );
      }

      if( g_Input->IsMouseInRegion( 382 + 64, 328, 382 + 64 + 31, 328 + 32 ) )
      {
         DrawToolTip( g_SmallFont, "Talk", 382 + 64, 328, 2 );
      }

      if( g_Input->IsMouseInRegion( 382 + 96, 328, 382 + 96 + 31, 328 + 32 ) )
      {
         DrawToolTip( g_SmallFont, "Look", 382 + 96, 328, 2 );
      }

      if( g_Input->IsMouseInRegion( 382 + 172, 328, 382 + 172 + 31, 328 + 32 ) )
      {
         stringstream writer;
         writer.str("");
         writer << "Level Up";

         vector<ColoredString> temp;

         ColoredString tempstring(writer.str() );
         temp.push_back(tempstring);

         if( g_Player->m_StatPoints > 0 )
         {
            writer.str("");
            writer << "You have " << g_Player->m_StatPoints << " unspent skill points.";
            ColoredString tempstring2(writer.str() );
            temp.push_back(tempstring2);

            DrawToolTip( g_SmallFont, temp, 382 + 172, 328, 2 );
         }
      }

      if( g_Input->IsMouseInRegion( 382 + 208, 328, 382 + 208 + 31, 328 + 32 ) )
      {
         DrawToolTip( g_SmallFont, "Options", 382 + 208, 328, 2 );
      }
   }


   if( g_Player->m_IsDead )
   {
      g_Display->BlitImageRect( g_Mask, 0, 0, 228, 256, 76, 57);
      g_Display->DrawBox(76, 57, 228, 256);

      g_Font->DrawTextCentered("You have died.", 190, 130 );


      if( DrawButtonCentered("Load Game", 190, 200 ) )
      {
         LoadGame();
      }

      if( DrawButtonCentered("Main Menu", 190, 220 ) )
      {
         g_StateMachine->MakeStateTransition(STATE_TITLESTATE);
      }
   }




   //  Do custom cursors
   switch( m_ActiveGUIState )
   {
   case GUI_GET:
      g_Display->BlitImage(g_Cursors[1], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

   case GUI_TALK:
      g_Display->BlitImage(g_Cursors[2], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

   case GUI_FIGHT:
      g_Display->BlitImage(g_Cursors[3], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

   case GUI_LOOK:
      g_Display->BlitImage(g_Cursors[4], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

    case GUI_BASH:
      g_Display->BlitImage(g_Cursors[5], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

    case GUI_SMITESPELL:
      g_Display->BlitImage(g_Cursors[6], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

    case GUI_TELEPORT:
      g_Display->BlitImage(g_Cursors[7], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

    case GUI_PICKLOCK:
      g_Display->BlitImage(g_Cursors[8], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

    case GUI_MAGICUNLOCK:
      g_Display->BlitImage(g_Cursors[9], g_Input->m_MouseX, g_Input->m_MouseY);
      break;

   default:
      g_Display->BlitImage(g_Cursors[0], g_Input->m_MouseX, g_Input->m_MouseY);
      break;
   }

   m_DrawTimer = SDL_GetTicks() - m_DrawTimer;

/*   stringstream framestream;
   framestream.str("");
   framestream << "Milliseconds per update: " << m_UpdateTimer;
   g_SmallFont->DrawTextA(framestream.str(), g_Offset, g_Offset);

   framestream.str("");
   framestream << "Milliseconds per draw: " << m_DrawTimer;
   g_SmallFont->DrawText(framestream.str(), g_Offset, g_Offset + 12 );

   framestream.str("");
   framestream << "CheckPaths this update: " << g_CheckPathCount;
   g_SmallFont->DrawText(framestream.str(), g_Offset, g_Offset + 24 );

   framestream.str("");
   framestream << "LineLengths this update: " << g_LineLengthCount;
   g_SmallFont->DrawText(framestream.str(), g_Offset, g_Offset + 36 );*/

}


void MainState::DoPlayerInput()
{
   //  First things first - if we're peering, ANY input at ALL takes off the peer
   if( g_Map->m_Peer )
   {
      if( g_Input->WasLButtonClickedInRegion(0, 0, 640, 480 ) || g_Input->WasRButtonClickedInRegion(0, 0, 640, 480 ) || g_Input->WasMButtonClickedInRegion(0, 0, 640, 480 ) )
      {
         g_Map->m_Peer = false;
         g_Input->DumpInput();
         return;
      }
      return;
   }


   if( g_Input->WasKeyPressed(SDLK_SPACE) || g_Input->WasKeyPressed(SDLK_KP5) )
   {
      AddConsoleString("Passed!", 255, 255, 255 );
      g_IsPlayerTurn = false;
      return;
   }

   if( g_Player->m_SleepCounter > 0 )
   {
      AddConsoleString("You are asleep!", 255, 255, 255 );
      g_IsPlayerTurn = false;
      return;
   }

   if(g_Input->WasKeyPressed(SDLK_e) && g_Input->IsKeyDown(SDLK_LCTRL) )
   {
      g_StateMachine->MakeStateTransition(STATE_EDITORSTATE);
   }

   if( g_Input->WasLButtonClickedInRegion( 372, 133, 372 + 259, 133 + 17 ) )
   {
      m_AbilityInventorySwitch = !m_AbilityInventorySwitch;
   }

   int targetx = g_Player->m_PlayerPosX;
   int targety = g_Player->m_PlayerPosY;

   bool playermoved = false;

   if( SDL_GetTicks() - m_PlayerMoveTimer > 100 && playermoved == false )
   {
      if( g_Input->m_KeyboardState[SDLK_a] || g_Input->m_KeyboardState[SDLK_LEFT] || g_Input->m_KeyboardState[SDLK_KP4] && playermoved == false )
      {
         targetx = g_Player->m_PlayerPosX - 1;
         playermoved = true;
      }
      else if( g_Input->m_KeyboardState[SDLK_d] || g_Input->m_KeyboardState[SDLK_RIGHT] || g_Input->m_KeyboardState[SDLK_KP6] && playermoved == false )
      {
         targetx = g_Player->m_PlayerPosX + 1;
         playermoved = true;
      }
      else if( g_Input->m_KeyboardState[SDLK_w] || g_Input->m_KeyboardState[SDLK_UP] || g_Input->m_KeyboardState[SDLK_KP8] && playermoved == false )
      {
         targety = g_Player->m_PlayerPosY - 1;
         playermoved = true;
      }
      else if( g_Input->m_KeyboardState[SDLK_s] || g_Input->m_KeyboardState[SDLK_x] || g_Input->m_KeyboardState[SDLK_DOWN] || g_Input->m_KeyboardState[SDLK_KP2] && playermoved == false )
      {
         targety = g_Player->m_PlayerPosY + 1;
         playermoved = true;
      }

      //  DIAGONALS
      if( g_Input->m_KeyboardState[SDLK_q] ||  g_Input->m_KeyboardState[SDLK_KP7] && playermoved == false )
      {
         targetx = g_Player->m_PlayerPosX - 1;
         targety = g_Player->m_PlayerPosY - 1;
         playermoved = true;
      }
      else if( g_Input->m_KeyboardState[SDLK_e] || g_Input->m_KeyboardState[SDLK_KP9] && playermoved == false )
      {
         targetx = g_Player->m_PlayerPosX + 1;
         targety = g_Player->m_PlayerPosY - 1;
         playermoved = true;
      }
      else if( g_Input->m_KeyboardState[SDLK_c] || g_Input->m_KeyboardState[SDLK_KP3] && playermoved == false )
      {
         targetx = g_Player->m_PlayerPosX + 1;
         targety = g_Player->m_PlayerPosY + 1;
         playermoved = true;
      }
      else if( g_Input->m_KeyboardState[SDLK_z] || g_Input->m_KeyboardState[SDLK_KP1] && playermoved == false )
      {
         targetx = g_Player->m_PlayerPosX - 1;
         targety = g_Player->m_PlayerPosY + 1;
         playermoved = true;
      }
   }

   //else if( g_Input->WasKeyPressed(SDLK_a) || g_Input->WasKeyPressed(SDLK_LEFT) && playermoved == false )
   //{
   //   targetx = g_Player->m_PlayerPosX - 1;
   //   playermoved = true;
   //}
   //else if( g_Input->WasKeyPressed(SDLK_d) || g_Input->WasKeyPressed(SDLK_RIGHT) && playermoved == false )
   //{
   //   targetx = g_Player->m_PlayerPosX + 1;
   //   playermoved = true;
   //}
   //else if( g_Input->WasKeyPressed(SDLK_w) || g_Input->WasKeyPressed(SDLK_UP) && playermoved == false )
   //{
   //   targety = g_Player->m_PlayerPosY - 1;
   //   playermoved = true;
   //}
   //else if( g_Input->WasKeyPressed(SDLK_s) || g_Input->WasKeyPressed(SDLK_DOWN) && playermoved == false )
   //{
   //   targety = g_Player->m_PlayerPosY + 1;
   //   playermoved = true;
   //}

   if( g_Input->WasLButtonClickedInRegion( 382 + 208 - 36, 328, 382 + 208 - 36 + 32, 328 + 32 ) )
   {
      g_StateMachine->PushState(STATE_LEVELUPSTATE);
   }


   if( playermoved )
      m_PlayerMoveTimer = SDL_GetTicks();

   if( g_Input->m_WasRightButtonClicked )
   {
      if( !g_Player->m_IsDead )
      {
         if( m_ActiveGUIState == GUI_NOSTATE )
         {
            if( DoInventoryUse() )
            {
               g_IsPlayerTurn = false;
            }
            else if( DoItemPickup() )
            {
               g_IsPlayerTurn = false;
            }
            else if (DoNPCTalk() )
            {
               g_IsPlayerTurn = false;
            }
            else if( DoNPCFight() )
            {
               g_IsPlayerTurn = false;
            }
         }
         else
         {
            switch( m_ActiveGUIState)
            {
            case GUI_GET:
               AddConsoleString("Get/Use mode cancelled.");
                  break;

            case GUI_FIGHT:
               AddConsoleString("Attack mode cancelled.");
                  break;

            case GUI_TALK:
               AddConsoleString("Talk mode cancelled.");
                  break;

            case GUI_LOOK:
               AddConsoleString("Look mode cancelled.");
                  break;

            case GUI_SMITESPELL:
               AddConsoleString("Smite mode cancelled.");
                  break;

            case GUI_MAGICUNLOCK:
               AddConsoleString("Magic Unlock cancelled.");
                  break;

            case GUI_TELEPORT:
               AddConsoleString("Teleport cancelled.");
                  break;

            case GUI_BASH:
               AddConsoleString("Bash cancelled.");
                  break;

            case GUI_PICKLOCK:
               AddConsoleString("Lockpicking cancelled.");
                  break;

            default:
               AddConsoleString("Cancelled!");
            }

            m_ActiveGUIState = GUI_NOSTATE;
         }
      }
   }

   else if( g_Input->m_WasLeftButtonClicked )
   {
      if( !g_Player->m_IsDead)      
      {
         //  Check for clicking on special abilities.
         if( m_AbilityInventorySwitch )
         {
            if( g_Input->WasLButtonClickedInRegion( g_InventoryX, g_InventoryY, g_InventoryX + (4 * 47), g_InventoryY + (4 * 32 ) ) )
            {
               //  Turn the screen coordinates into inventory coordinates.
               int mapx = ((g_Input->m_MouseX - g_InventoryX) / 47);
               int mapy = ((g_Input->m_MouseY - g_InventoryY) / 32);
               int ability = mapy * 4 + mapx;

               if( ability < 16 && g_SpecialAbilities[ability].m_Active > 0 )
               {
                  switch(ability)
                  {
                  case ABILITY_SMITE:
                     m_ActiveGUIState = GUI_SMITESPELL;
                     AddConsoleString("Click what you wish to attack.  Right-click to cancel.");
                     break;

                  case ABILITY_DAMAGEBONUS:
                     m_ActiveGUIState = GUI_DAMAGESPELL;
                     break;

                  case ABILITY_HEAL:
                     m_ActiveGUIState = GUI_HEALSPELL;
                     break;

                  case ABILITY_MAGICARMOR:
                     m_ActiveGUIState = GUI_ARMORSPELL;
                     break;

                  case ABILITY_MAGICUNLOCK:
                     AddConsoleString("Click the door or trap you wish to disarm.  Right-click to cancel.");
                     m_ActiveGUIState = GUI_MAGICUNLOCK;
                     break;

                  case ABILITY_MAP:
                     m_ActiveGUIState = GUI_MAGICMAP;
                     break;

                  case ABILITY_TELEPORT:
                     AddConsoleString("Click where you wish to teleport.  Right-click to cancel.");
                     m_ActiveGUIState = GUI_TELEPORT;
                     break;

                  case ABILITY_TOWNPORTAL:
                     m_ActiveGUIState = GUI_TOWNPORTAL;
                     break;

                  case ABILITY_BASH:
                     AddConsoleString("Click the door you wish to try to bash open.  Right-click to cancel.");
                     m_ActiveGUIState = GUI_BASH;
                     break;

                  case ABILITY_PICKLOCK:
                     AddConsoleString("Click the door or trap you wish to disarm.  Right-click to cancel.");
                     m_ActiveGUIState = GUI_PICKLOCK;
                     break;
                  }
               }
            }
         }


         if( m_ActiveGUIState == GUI_NOSTATE )  //  No special GUI state
         {
            //  Check to see if we are using an item
            if( m_AbilityInventorySwitch == false && g_Input->m_MouseX > g_InventoryX && g_Input->m_MouseX < g_InventoryX + 4 * 32 && g_Input->m_MouseY > g_InventoryY && g_Input->m_MouseY < g_InventoryY + 4 * 32 )
            {
               //  Turn the screen coordinates into inventory coordinates.
               int mapx = ((g_Input->m_MouseX - g_InventoryX) / 32);
               int mapy = ((g_Input->m_MouseY - g_InventoryY) / 32);
               int inventoryindex = mapy * 4 + mapx;

               if(inventoryindex < g_Player->m_PlayerInventory.size())
               {
                  g_Player->m_PlayerInventory[inventoryindex]->OnUse();
               }
            }
            //  Check to see if we are moving.
            if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
            {
               //  Turn the screen coordinates into map coordinates.
               int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
               mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
               int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
               mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

               if(mapx > g_Player->m_PlayerPosX)
               {
                  targetx = g_Player->m_PlayerPosX + 1;
                  playermoved = true;
               }

               else if(mapx < g_Player->m_PlayerPosX)
               {
                  targetx = g_Player->m_PlayerPosX - 1;
                  playermoved = true;
               }

               if(mapy > g_Player->m_PlayerPosY)
               {
                  targety = g_Player->m_PlayerPosY + 1;
                  playermoved = true;
               }

               else if(mapy < g_Player->m_PlayerPosY)
               {
                  targety = g_Player->m_PlayerPosY - 1;
                  playermoved = true;
               }
            }

            //  Unequip equipped items.
            //  Do equipped item tooltips
            if( m_AbilityInventorySwitch == false && g_Input->WasLButtonClickedInRegion( g_InventoryX + 144, g_InventoryY + 10, g_InventoryX + 144 + 32, g_InventoryY + 10 + 32 ) && g_Player->m_CurrentWeaponType )
            {
               if( g_Player->m_PlayerInventory.size() <= 15 )  //  At least one empty space.
               {
                  g_Player->m_PlayerInventory.push_back( g_Player->m_CurrentWeaponType );
                  g_Player->m_CurrentWeaponType = NULL;
                  g_Player->RecalculateHitpointsAndMana();
               }
               else
               {
                  AddConsoleString("Can't unequip - no room in inventory!");
               }
            }

            if( m_AbilityInventorySwitch == false && g_Input->WasLButtonClickedInRegion( g_InventoryX + 144, g_InventoryY + 53, g_InventoryX + 144 + 32, g_InventoryY + 53 + 32 ) && g_Player->m_CurrentArmorType )
            {
               if( g_Player->m_PlayerInventory.size() <= 15 )  //  At least one empty space.
               {
                  g_Player->m_PlayerInventory.push_back( g_Player->m_CurrentArmorType );
                  g_Player->m_CurrentArmorType = NULL;
                  g_Player->RecalculateHitpointsAndMana();
               }
               else
               {
                  AddConsoleString("Can't unequip - no room in inventory!");
               }
            }

            if( m_AbilityInventorySwitch == false && g_Input->WasLButtonClickedInRegion( g_InventoryX + 144, g_InventoryY + 96, g_InventoryX + 144 + 32, g_InventoryY + 96 + 32 ) && g_Player->m_CurrentTrinketType )
            {
               if( g_Player->m_PlayerInventory.size() <= 15 )  //  At least one empty space.
               {
                  g_Player->m_PlayerInventory.push_back( g_Player->m_CurrentTrinketType );
                  g_Player->m_CurrentTrinketType = NULL;
                  g_Player->RecalculateHitpointsAndMana();
               }
               else
               {
                  AddConsoleString("Can't unequip - no room in inventory!");
               }
            }
         }

         else if( m_ActiveGUIState == GUI_LOOK )
         {
            //  Mode 2 = Look
            //  Check to see if there is an NPC under the mouse cursor.
            if( DoLookAt() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
            }
         }


         //  Mode 3 = Take

         else if( m_ActiveGUIState == GUI_GET )
         {
            if( DoItemPickup() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
               g_IsPlayerTurn = false;

            }
         }

         //  Mode 4 = Attack
         else if( m_ActiveGUIState == GUI_FIGHT )
         {
            //  Check to see if there is an NPC under the mouse cursor.
            if( DoNPCFight() )
            {
               g_IsPlayerTurn = false;
            }
         }

         else if( m_ActiveGUIState == GUI_TALK )
         {
            //  Mode 1 = Talk
            //  Check to see if there is an NPC under the mouse cursor.
            if( DoNPCTalk() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
            }
         }

         else if( m_ActiveGUIState == GUI_SMITESPELL )
         {
            if( DoSmite() )
            {
               g_IsPlayerTurn = false;
            }
         }

         else if( m_ActiveGUIState == GUI_BASH )
         {
            if( DoBash() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
               g_IsPlayerTurn = false;
            }
            else
            {
               g_IsPlayerTurn = false;
            }
         }

         else if( m_ActiveGUIState == GUI_PICKLOCK )
         {
            if( DoPickLock() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
               g_IsPlayerTurn = false;
            }
            else
            {
               g_IsPlayerTurn = false;
            }
         }

         else if( m_ActiveGUIState == GUI_MAGICUNLOCK )
         {
            if( DoMagicUnlock() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
               g_IsPlayerTurn = false;
            }
            else
            {
               g_IsPlayerTurn = false;
            }
         }

         else if( m_ActiveGUIState == GUI_TELEPORT )
         {
            if( DoTeleport() )
            {
               m_ActiveGUIState = GUI_NOSTATE;
               g_IsPlayerTurn = false;
            }
         }

         if( m_ActiveGUIState == GUI_ARMORSPELL )
         {
            if( DoMagicArmor() )
            {
               g_IsPlayerTurn = false;
            }
            m_ActiveGUIState = GUI_NOSTATE;
         }

         if( m_ActiveGUIState == GUI_DAMAGESPELL )
         {
            if( DoExtraDamage() )
            {
               g_IsPlayerTurn = false;
            }
            m_ActiveGUIState = GUI_NOSTATE;
         }

         if( m_ActiveGUIState == GUI_HEALSPELL )
         {
            if( DoHeal() )
            {
               g_IsPlayerTurn = false;
            }
            m_ActiveGUIState = GUI_NOSTATE;
         }

         if( m_ActiveGUIState == GUI_MAGICMAP )
         {
            if( DoMap() )
            {
               g_IsPlayerTurn = false;
            }
            m_ActiveGUIState = GUI_NOSTATE;
         }

         if( m_ActiveGUIState == GUI_TOWNPORTAL )
         {
            if( DoTownPortal() )
            {
               g_IsPlayerTurn = false;
            }
            m_ActiveGUIState = GUI_NOSTATE;
         }
      }
   }

   //  Now vet the target and see if we can move in the specified direction.

   if( playermoved )
   {
      //  Are we going off the map?
      if(targetx < 0 || targetx >= g_Map->m_Width)
      {
         targetx = g_Player->m_PlayerPosX;
      }
      if(targety < 0 || targety >= g_Map->m_Height)
      {
         targety = g_Player->m_PlayerPosY;
      }

      //  Are we walking into something solid?
      if( g_Map->m_map[targetx][targety].m_passable == false)
      {
         targetx = g_Player->m_PlayerPosX;
         targety = g_Player->m_PlayerPosY;
      }

      //  Are we walking into a person or monster?
      for(list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2)
      {
         if((*node2)->m_PosX  == targetx && (*node2)->m_PosY == targety)
         {
            targetx = g_Player->m_PlayerPosX;
            targety = g_Player->m_PlayerPosY;
         }
      }

      //  Are we walking into an item marked unpassable?
      for(list<Item*>::iterator node2 = g_Map->m_ItemList.begin(); node2 != g_Map->m_ItemList.end(); ++node2)
      {
         if(targetx == (*node2)->m_PosX && targety == (*node2)->m_PosY && !(*node2)->m_Passable )
         {
            targetx = g_Player->m_PlayerPosX;
            targety = g_Player->m_PlayerPosY;
         }
      }

      bool checkforstep = false;
      if( targetx != g_Player->m_PlayerPosX )
      {
         g_Player->m_PlayerPosX = targetx;
         g_IsPlayerTurn = false;
         checkforstep = true;
      }

      if( targety != g_Player->m_PlayerPosY )
      {
         g_IsPlayerTurn = false;
         g_Player->m_PlayerPosY = targety;
         checkforstep = true;
      }

      if( checkforstep )
      {
         //  If the player is standing on an item, do that item's OnStand().
         list<Item*>::iterator node = g_Map->m_ItemList.begin();
         for( node; node != g_Map->m_ItemList.end(); ++node )
         {
            if( g_Player->m_PlayerPosX == (*node)->m_PosX && g_Player->m_PlayerPosY == (*node)->m_PosY )
            {
               (*node)->OnStand();
            }
         }
      }
   }
}



bool MainState::DoInventoryUse()
{
   //  Do right-click context item use.
   if( m_AbilityInventorySwitch == false && g_Input->m_MouseX > g_InventoryX && g_Input->m_MouseX < g_InventoryX + 4 * 32 && g_Input->m_MouseY > g_InventoryY && g_Input->m_MouseY < g_InventoryY + 4 * 32 )
   {
      //  Turn the screen coordinates into inventory coordinates.
      int mapx = ((g_Input->m_MouseX - g_InventoryX) / 32);
      int mapy = ((g_Input->m_MouseY - g_InventoryY) / 32);
      int inventoryindex = mapy * 4 + mapx;

      if(inventoryindex < g_Player->m_PlayerInventory.size())
      {
         g_Player->m_PlayerInventory[inventoryindex]->OnUse();
         return true;
      }
   }
   return false;
}

bool MainState::DoItemPickup()
{
   //  Do right-click pickup-open
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      //  Did we right-click on an item?  If so, try to pick it up.
      list<Item*>::iterator itemnode = g_Map->m_ItemList.begin();
      for(itemnode; itemnode != g_Map->m_ItemList.end(); ++itemnode)
      {
         //  Is it close enough to the player?
         if((mapx - g_Player->m_PlayerPosX <= 1 && mapx - g_Player->m_PlayerPosX >= -1 &&
            mapy - g_Player->m_PlayerPosY <= 1 && mapy - g_Player->m_PlayerPosY >= -1))
         {
            //  Does it match the given map coordinates?
            if((*itemnode)->m_PosX == mapx && (*itemnode)->m_PosY == mapy )
            {
               if( (*itemnode)->m_Pickupable )
               {
                  //  Pitck it up.
                  (*itemnode)->OnPickup();

                  if( (*itemnode)->IsDead() == false && !(*itemnode)->m_PickupHandled )
                  {
                     if(g_Player->m_PlayerInventory.size() == 16)
                     {
                        AddConsoleString("Not enough room!");
                     }
                     else
                     {
                        g_Player->m_PlayerInventory.push_back( (*itemnode) );
                        if( (*itemnode)->m_Name == "Viridian's Armor" )
                           g_Player->m_GotViridianArmor = true;
                        if( (*itemnode)->m_Name == "Sol's Sword of Wisdom" )
                           g_Player->m_GotSolsSword = true;
                        itemnode = g_Map->m_ItemList.erase( itemnode );
                        return true;
                     }
                  }
                  else
                  {
                     return true;
                  }
               }
               else
               {
                  AddConsoleString("You cannot pick that up.");
               }
            }
         }
      }
   }
   return false;
}

bool MainState::DoNPCTalk()
{
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      list<NPC*>::iterator node = g_Map->m_NPCList.begin();
      for(node; node != g_Map->m_NPCList.end(); ++node)
      {
         if((*node)->m_PosX == mapx && (*node)->m_PosY == mapy && (*node)->m_Attitude != 0 )
         {

            if(!CheckPath( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY, false ) )
            {
               AddConsoleString("You cannot see that person.");
               return false;
            }

            if( LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY ) > 3 )
            {
               AddConsoleString("You are too far away to talk to that person.");
               return false;
            }
            

            if( (*node)->m_Name == "Emerald Merchant" || (*node)->m_Name == "Sapphire Merchant" || (*node)->m_Name == "Ruby Merchant" )
            {
               (*node)->OnTalk();
               m_AbilityInventorySwitch = false;
               g_StateMachine->PushState(STATE_STORESTATE);
            }

            else if( (*node)->m_Name == "Emerald Healer" || (*node)->m_Name == "Sapphire Healer" || (*node)->m_Name == "Ruby Healer" || (*node)->m_Name == "Last Front Healer" )
            {
               (*node)->OnTalk();
               g_Player->m_CurrentHitPoints = g_Player->m_MaxHitPoints;
               g_Player->m_CurrentManaPoints = g_Player->m_MaxManaPoints;
            }

            else if( (*node)->m_Name == "Mother Green Dragon" )
            {
               //  Check for whelp death
               if( g_Player->m_KilledWhelp )
               {
                  (*node)->m_Attitude = 0;
                  (*node)->m_CurrentTalk = 3;
                  (*node)->OnTalk();
                  return true;
               }

               //  Check for nearby whelp
               for( list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2 )
               {
                  if( (*node2)->m_Name == "Baby Dragon")
                  {
                     if( LineLength( (*node)->m_PosX, (*node)->m_PosY, (*node2)->m_PosX, (*node2)->m_PosY ) < 5 )
                     {
                        if( (*node)->m_CurrentTalk < 4 ) (*node)->m_CurrentTalk = 4;
                        (*node)->OnTalk();
                        return true;
                     }
                  }
               }

               //  Normal cycle
               (*node)->OnTalk();
               if( (*node)->m_CurrentTalk >= 3 )
                  (*node)->m_CurrentTalk = 0;
            }

            else if( (*node)->m_Name == "The Evil One" )
            {  
               //  Normal cycle
               (*node)->OnTalk();
               if( (*node)->m_CurrentTalk >= 12 )
                  (*node)->m_CurrentTalk = 0;
            }

            else if( (*node)->m_Name == "Inarian King" )
            {

               if( g_Player->m_KilledSlornKing && !g_Player->m_KilledEvilOne )
               {
                  if( (*node)->m_CurrentTalk < 8 )
                     (*node)->m_CurrentTalk = 8;

                  if( g_Player->m_PaidByInarianKing )
                  {
                     (*node)->OnTalk();
                     if( (*node)->m_CurrentTalk == 11 )
                        (*node)->m_CurrentTalk = 8;
                  }
                  else
                  {
                     (*node)->OnTalk();
                     if( (*node)->m_CurrentTalk == 0 )
                     {
                        (*node)->m_CurrentTalk = 8;
                        g_Player->m_CurrentGold += 500;
                        g_Sound->Play("/Sounds/level_up.wav");
                        g_Player->m_PaidByInarianKing = true;
                     }
                  }
               }
               else
               {
                  (*node)->OnTalk();
                  if( (*node)->m_CurrentTalk == 8)
                     (*node)->m_CurrentTalk = 0;
               }
            }

            else if( (*node)->m_Name == "Oubliette Palsied Woman" )
            {
               if( !g_Player->m_KilledMendikar )
               {
                  (*node)->OnTalk();
                  if( (*node)->m_CurrentTalk >= 7 )
                     (*node)->m_CurrentTalk = 0;
               }
               else
               {
                  (*node)->OnTalk();
                  if( (*node)->m_CurrentTalk == 0 )
                     (*node)->m_CurrentTalk = 10;
                  if( (*node)->m_CurrentTalk >= (*node)->m_NumberOfTalks )
                     (*node)->m_CurrentTalk = 10;
               }
            }

            else
            {
               (*node)->OnTalk();
            }

            return true;
         }

      }
   }
   return false; //  Talking is a free action.
}

bool MainState::DoNPCFight()
{
   //  If we're on the intro or inaria maps, DON'T ALLOW ATTACKS!
   if( g_Map->m_MapTitle == "Inaria" || g_Map->m_MapTitle == "Inaria Special")
   {
//      AddConsoleString("You cannot attack that.");
      return false;
   }
 
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  If this is the first attack on the Evil One, say his bark.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      list<NPC*>::iterator node = g_Map->m_NPCList.begin();
      for(node; node != g_Map->m_NPCList.end(); ++node)
      {
         if((*node)->m_PosX == mapx && (*node)->m_PosY == mapy && (*node)->m_Name == "The Evil One" && (*node)->m_Attitude == 1 )
         {
            AddConsoleString( (*node)->m_Talks[12], 255, 64, 255  );
         }
      }

      //  Missile combat
      if( (g_Player->m_CurrentWeaponType != NULL && g_Player->m_CurrentWeaponType->GetItemType() == ITEM_BOW) )
         return DoMissileCombat();
      else
         return DoMeleeCombat();
   }


   return false;
}

bool MainState::DoMissileCombat()
{
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      //  Do hittable switches.
      list<Item*>::iterator itemattacknode = g_Map->m_ItemList.begin();
      for( itemattacknode; itemattacknode != g_Map->m_ItemList.end(); ++itemattacknode)
      {
         if( (*itemattacknode)->m_PosX == mapx && (*itemattacknode)->m_PosY == mapy )
         {
            InariaEffect* tempanimation = new InariaEffect;

            LegacySprite* arrow = NULL;
            if( (*itemattacknode)->m_PosX == g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY > g_Player->m_PlayerPosY ) //  Firing right
               arrow = g_Tiles[141];

            else if( (*itemattacknode)->m_PosX == g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY < g_Player->m_PlayerPosY ) //  Firing left
               arrow = g_Tiles[137];

            else if( (*itemattacknode)->m_PosX > g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY == g_Player->m_PlayerPosY ) //  Firing down
               arrow = g_Tiles[139];

            else if( (*itemattacknode)->m_PosX < g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY == g_Player->m_PlayerPosY ) //  Firing up
               arrow = g_Tiles[143];

            else if( (*itemattacknode)->m_PosX > g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY > g_Player->m_PlayerPosY ) //  Firing down-right
               arrow = g_Tiles[140];

            else if( (*itemattacknode)->m_PosX > g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY < g_Player->m_PlayerPosY ) //  Firing up-right
               arrow = g_Tiles[138];

            else if( (*itemattacknode)->m_PosX < g_Player->m_PlayerPosX && (*itemattacknode)->m_PosY > g_Player->m_PlayerPosY ) //  Firing down-left
               arrow = g_Tiles[142];

            else //  Firing up-left
               arrow = g_Tiles[144];

            tempanimation->Init( arrow, g_Offset + (g_ViewRange * g_TileSize),
               g_Offset + (g_ViewRange * g_TileSize),
               g_Offset + ((*itemattacknode)->m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize,
               g_Offset + ((*itemattacknode)->m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize,
               LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*itemattacknode)->m_PosX, (*itemattacknode)->m_PosY) * 50, true );

            g_AnimationList.push_back(tempanimation);

            (*itemattacknode)->OnAttack();
         }
      }

      list<NPC*>::iterator node = g_Map->m_NPCList.begin();
      for(node; node != g_Map->m_NPCList.end(); ++node)
      {
         if( (*node)->m_PosX == mapx && (*node)->m_PosY == mapy )
         {
            if( !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, mapx, mapy ) )
            {
               AddConsoleString("Path blocked!");
               return false;
            }

            //  Put the HURTING on him!
            (*node)->m_AI = 3;  //  If it wasn't hostile before, it is now.
            (*node)->m_Attitude = false;

            if( DoRPGCheck( g_Player->GetDexterityWithBonus() + g_Player->GetIntelligenceWithBonus(), (*node)->m_Dexterity ) )
            {
               int damage = HalfNHalf( g_Player->GetDamage() );
               if( damage <= 0 )
                  damage = 1;

               //  Check for a critical hit.
               bool critical = false;
               if( g_SpecialAbilities[ABILITY_SNIPER].m_Active )
               {
                  critical = DoRPGCheck( g_Player->GetDexterityWithBonus() + 5, 20 - ( g_Player->GetDexterityWithBonus() + 5 ) );
               }
               else
               {
                  critical = DoRPGCheck( g_Player->GetDexterityWithBonus(), 20 - ( g_Player->m_Dexterity ) );
               }

               if(critical)
               {
                  int criticaldamage = damage * .5;
                  if (criticaldamage < 1)
                     criticaldamage = 1;

                  damage += criticaldamage;
               }


               InariaEffect* tempanimation = new InariaEffect;

               LegacySprite* arrow = NULL;
               if( (*node)->m_PosX == g_Player->m_PlayerPosX && (*node)->m_PosY > g_Player->m_PlayerPosY ) //  Firing right
                  arrow = g_Tiles[141];

               else if( (*node)->m_PosX == g_Player->m_PlayerPosX && (*node)->m_PosY < g_Player->m_PlayerPosY ) //  Firing left
                  arrow = g_Tiles[137];

               else if( (*node)->m_PosX > g_Player->m_PlayerPosX && (*node)->m_PosY == g_Player->m_PlayerPosY ) //  Firing down
                  arrow = g_Tiles[139];

               else if( (*node)->m_PosX < g_Player->m_PlayerPosX && (*node)->m_PosY == g_Player->m_PlayerPosY ) //  Firing up
                  arrow = g_Tiles[143];

               else if( (*node)->m_PosX > g_Player->m_PlayerPosX && (*node)->m_PosY > g_Player->m_PlayerPosY ) //  Firing down-right
                  arrow = g_Tiles[140];

               else if( (*node)->m_PosX > g_Player->m_PlayerPosX && (*node)->m_PosY < g_Player->m_PlayerPosY ) //  Firing up-right
                  arrow = g_Tiles[138];

               else if( (*node)->m_PosX < g_Player->m_PlayerPosX && (*node)->m_PosY > g_Player->m_PlayerPosY ) //  Firing down-left
                  arrow = g_Tiles[142];

               else //  Firing up-left
                  arrow = g_Tiles[144];

               tempanimation->Init( arrow, g_Offset + (g_ViewRange * g_TileSize),
                  g_Offset + (g_ViewRange * g_TileSize),
                  g_Offset + ((*node)->m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize,
                  g_Offset + ((*node)->m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize,
                  LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY) * 50, true );

               g_AnimationList.push_back(tempanimation);

               (*node)->DoDamage(damage, critical);
            }
            else
            {
               InariaEffect* tempanimation = new InariaEffect;

               LegacySprite* arrow = NULL;
               if( (*node)->m_PosX == g_Player->m_PlayerPosX && (*node)->m_PosY > g_Player->m_PlayerPosY ) //  Firing right
                  arrow = g_Tiles[141];

               else if( (*node)->m_PosX == g_Player->m_PlayerPosX && (*node)->m_PosY < g_Player->m_PlayerPosY ) //  Firing left
                  arrow = g_Tiles[137];

               else if( (*node)->m_PosX > g_Player->m_PlayerPosX && (*node)->m_PosY == g_Player->m_PlayerPosY ) //  Firing down
                  arrow = g_Tiles[139];

               else if( (*node)->m_PosX < g_Player->m_PlayerPosX && (*node)->m_PosY == g_Player->m_PlayerPosY ) //  Firing up
                  arrow = g_Tiles[143];

               else if( (*node)->m_PosX > g_Player->m_PlayerPosX && (*node)->m_PosY > g_Player->m_PlayerPosY ) //  Firing down-right
                  arrow = g_Tiles[140];

               else if( (*node)->m_PosX > g_Player->m_PlayerPosX && (*node)->m_PosY < g_Player->m_PlayerPosY ) //  Firing up-right
                  arrow = g_Tiles[138];

               else if( (*node)->m_PosX < g_Player->m_PlayerPosX && (*node)->m_PosY > g_Player->m_PlayerPosY ) //  Firing down-left
                  arrow = g_Tiles[142];

               else //  Firing up-left
                  arrow = g_Tiles[144];

               tempanimation->Init( arrow, g_Offset + (g_ViewRange * g_TileSize),
                  g_Offset + (g_ViewRange * g_TileSize),
                  g_Offset + ((*node)->m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize,
                  g_Offset + ((*node)->m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize,
                  LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY) * 50, true );

               g_AnimationList.push_back(tempanimation);

               string _tempString = "You missed the " + (*node)->m_Name + ".";
               AddConsoleString(_tempString);

               InariaEffect* missanimation = new InariaEffect;
               missanimation->Init( NULL, 0, 0, 0, 0, 250, true, 0, "Sounds/miss1.wav" );
               g_AnimationList.push_back(missanimation);
            }

            return true; //  Either way, they used their turn.
         }
      }
   }
   return false;
}

bool MainState::DoMeleeCombat()
{
   //  Turn the screen coordinates into map coordinates.
   int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
   mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
   int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
   mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

   //  Do hittable switches.

   list<Item*>::iterator itemattacknode = g_Map->m_ItemList.begin();
   for( itemattacknode; itemattacknode != g_Map->m_ItemList.end(); ++itemattacknode)
   {
      if((*itemattacknode)->m_PosX == mapx && (*itemattacknode)->m_PosY == mapy)
      {
         if( LineLength( (*itemattacknode)->m_PosX, (*itemattacknode)->m_PosY, g_Player->m_PlayerPosX, g_Player->m_PlayerPosY ) < 1.5)
            (*itemattacknode)->OnAttack();
         else
            AddConsoleString("You are too far away from that.");
      }
   }


   //  Did we right-click on an NPC?  If so, do fight.
   int cleavecounter = 0;
   bool cleaving = false;
   if( g_SpecialAbilities[ABILITY_CLEAVE].m_Active && g_SpecialAbilities[ABILITY_CLEAVE].m_Level > 0 )
      cleavecounter = (g_SpecialAbilities[ABILITY_CLEAVE].m_Level / 16) + 1;

   //  Initial hit.
   bool foundtarget = false;
   list<NPC*>::iterator node = g_Map->m_NPCList.begin();
   for(node; node != g_Map->m_NPCList.end(); ++node)
   {
      if( (*node)->m_PosX == mapx && (*node)->m_PosY == mapy ) 
      {
         //   Melee combat
         if( ((mapx - g_Player->m_PlayerPosX <= 1 && mapx - g_Player->m_PlayerPosX >= -1 &&
            mapy - g_Player->m_PlayerPosY <= 1 && mapy - g_Player->m_PlayerPosY >= -1) ) )
         {
            foundtarget = true;
            //  Put the HURTING on him!
            (*node)->m_AI = 3;  //  If it wasn't hostile before, it is now.
            (*node)->m_Attitude = false;

            if( DoRPGCheck( g_Player->GetStrengthWithBonus() + g_Player->GetDexterityWithBonus(), (*node)->m_Dexterity ) )
            {
               int damage = HalfNHalf( g_Player->GetDamage() );
               if( damage <= 0 )
                  damage = 1;

               //  Check for a critical hit.
               bool critical = false;
               critical = DoRPGCheck( g_Player->GetDexterityWithBonus(), 20 - ( g_Player->GetDexterityWithBonus() ) );

               if(critical)
               {
                  int criticaldamage = damage * .5;
                  if (criticaldamage < 1)
                     criticaldamage = 1;

                  damage += criticaldamage;
               }

               (*node)->DoDamage(damage, critical);

               if( cleavecounter == 0 )
                  return true;
               else
               {
                  cleaving = true;
                  break;
               }
            }
            else
            {
               string _tempString = "You missed the " + (*node)->m_Name + ".";
               AddConsoleString(_tempString);
               InariaEffect* tempanimation = new InariaEffect;
               tempanimation->Init( NULL, 0, 0, 0, 0, 250, true, 0, "Sounds/miss1.wav" );
               g_AnimationList.push_back(tempanimation);
               return true;
            }
         }
         else
         {
            AddConsoleString("Out of range!");
            return false;
         }
      }
   }
   if( !foundtarget)
   {
      return false;
   }

   // Cleaves
   node = g_Map->m_NPCList.begin();
   for(node; node != g_Map->m_NPCList.end(); ++node)
   {
      if( (*node)->m_PosX != mapx || (*node)->m_PosY != mapy ) //  Can't cleave back to the original foe.
      {
         //   Melee combat
         if( (((*node)->m_PosX - g_Player->m_PlayerPosX <= 1 && (*node)->m_PosX - g_Player->m_PlayerPosX >= -1 &&
            (*node)->m_PosY - g_Player->m_PlayerPosY <= 1 && (*node)->m_PosY - g_Player->m_PlayerPosY >= -1) ) && (*node)->m_Attitude == false && (*node)->m_Name != "Oubliette Palsied Woman" )
         {
            //  Put the HURTING on him!
            (*node)->m_AI = 3;  //  If it wasn't hostile before, it is now.
            (*node)->m_Attitude = false;

            if( DoRPGCheck( g_Player->GetStrengthWithBonus() + g_Player->GetDexterityWithBonus(), (*node)->m_Dexterity ) )
            {
               int damage = HalfNHalf( g_Player->GetDamage() );
               if( damage <= 0 )
                  damage = 1;

               //  Check for a critical hit.
               bool critical = false;
               critical = DoRPGCheck( g_Player->GetDexterityWithBonus(), 20 - ( g_Player->GetDexterityWithBonus() ) );

               if(critical)
               {
                  int criticaldamage = damage * .5;
                  if (criticaldamage < 1)
                     criticaldamage = 1;

                  damage += criticaldamage;
               }

               --cleavecounter;
               AddConsoleString("You Cleave to a new foe!");
               (*node)->DoDamage(damage, critical);
               ++g_SpecialAbilities[ABILITY_CLEAVE].m_Level; //  A successful cleave raises its level.
               if( g_SpecialAbilities[ABILITY_CLEAVE].m_Level % 16 == 0 ) //  New cleave level.
                  AddConsoleString("Your Cleave skill has improved!  It will now hit more enemies.", 255, 255, 128 );

               if( cleavecounter == 0 )
                  return true;
            }
            else //  Missing stops a cleave
            {
               string _tempString = "You missed the " + (*node)->m_Name + ".";
               AddConsoleString(_tempString);
               InariaEffect* tempanimation = new InariaEffect;
               tempanimation->Init( NULL, 0, 0, 0, 0, 250, true, 0, "Sounds/miss1.wav" );
               g_AnimationList.push_back(tempanimation);
               return true;
            }
         }
      }
   }
   return false;
}


bool MainState::DoLookAt()
{
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      list<NPC*>::iterator node = g_Map->m_NPCList.begin();
      for(node; node != g_Map->m_NPCList.end(); ++node)
      {
         if((*node)->m_PosX == mapx && (*node)->m_PosY == mapy )
         {
            if(!CheckPath( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY, false ) )
            {
               AddConsoleString("You cannot see that person.");
               return false;
            }

            (*node)->OnLook();
         }
      }

      list<Item*>::iterator itemnode = g_Map->m_ItemList.begin();
      for( itemnode; itemnode != g_Map->m_ItemList.end(); ++itemnode )
      {
         if((*itemnode)->m_PosX == mapx && (*itemnode)->m_PosY == mapy )
         {
            if(!CheckPath( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*itemnode)->m_PosX, (*itemnode)->m_PosY, false ) )
            {
               AddConsoleString("You cannot see that object.");
               return false;
            }

            (*itemnode)->OnLook();
         }
      }
   }
   return false; //  Looking is a free action.
}


//  Non-spell special abilities
bool MainState::DoBash()
{
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      list<Item*>::iterator itemnode = g_Map->m_ItemList.begin();
      for( itemnode; itemnode != g_Map->m_ItemList.end(); ++itemnode)
      {
         if((*itemnode)->m_PosX == mapx && (*itemnode)->m_PosY == mapy )
         {           
            if( (*itemnode)->m_Bashable )
            {
               if( LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*itemnode)->m_PosX, (*itemnode)->m_PosY) < 1.5 )
               {
                  if( DoRPGCheck( g_Player->m_Strength, 20 - g_Player->m_Strength ) )
                  {
                     AddConsoleString( (*itemnode)->m_Name + " bashed open!" );
                     itemnode = g_Map->m_ItemList.erase(itemnode);
                     return true;
                  }
                  else
                  {
                     //  A failed bash still takes a turn.
                     AddConsoleString("Bash failed!");
                     return false;
                  }
               }
               else
               {
                  AddConsoleString("You are too far away from that.");
                  return true;
               }
            }
         }
      }
   }
   return false;
}

bool MainState::DoPickLock()
{
   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      list<Item*>::iterator itemnode = g_Map->m_ItemList.begin();
      for( itemnode; itemnode != g_Map->m_ItemList.end(); ++itemnode)
      {
         if((*itemnode)->m_PosX == mapx && (*itemnode)->m_PosY == mapy )
         {
            if( (*itemnode)->m_Pickable )
            {
               if( LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*itemnode)->m_PosX, (*itemnode)->m_PosY) < 1.5 )
               {
                  if( DoRPGCheck( g_Player->m_Dexterity, 20 - g_Player->m_Strength ) )
                  {
                     AddConsoleString( (*itemnode)->m_Name + " removed!");
                     itemnode = g_Map->m_ItemList.erase(itemnode);
                     return true;
                  }
                  else
                  {
                     AddConsoleString("Lockpicking failed!");
                     return false;
                  }
               }
               else
               {
                  AddConsoleString("You are too far away from that.");
                  return true;
               }
            }
         }
      }
   }
   return false;
}

//  Spells
bool MainState::DoSmite()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_SMITE].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }



   int smitecounter = g_SpecialAbilities[ABILITY_SMITE].m_Level / 16;
   NPC* previoustarget = NULL;

   //  Check to see if there is an NPC under the mouse cursor.

   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {

      if( g_Map->m_MapTitle == "Inaria" || g_Map->m_MapTitle == "Inaria Special") //  Can't smite friendlies
      {
         AddConsoleString("You cannot attack that.");
         return false;
      }

      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      //  Do hittable switches.

      list<Item*>::iterator itemattacknode = g_Map->m_ItemList.begin();
      for( itemattacknode; itemattacknode != g_Map->m_ItemList.end(); ++itemattacknode)
      {
         if((*itemattacknode)->m_PosX == mapx && (*itemattacknode)->m_PosY == mapy)
         {
            //  This is the basic "bolt" animation"
            InariaEffect* tempanimation = new InariaEffect;
            tempanimation->Init( g_Star,
               g_Offset + ((g_ViewRange) * g_TileSize + 4),
               g_Offset + ((g_ViewRange) * g_TileSize + 4),
               g_Offset + (((*itemattacknode)->m_PosX - g_Player->m_PlayerPosX + g_ViewRange) * g_TileSize + 4),
               g_Offset + (((*itemattacknode)->m_PosY - g_Player->m_PlayerPosY + g_ViewRange) * g_TileSize + 4),
               50 * LineLength(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*itemattacknode)->m_PosX, (*itemattacknode)->m_PosY), true, 1, "", "", 255, 255, 128 );
            g_AnimationList.push_back(tempanimation);
            
            (*itemattacknode)->OnAttack();
         }
      }

      bool foundtarget = false;
      list<NPC*>::iterator node = g_Map->m_NPCList.begin();
      for(node; node != g_Map->m_NPCList.end(); ++node)
      {
         if((*node)->m_PosX == mapx && (*node)->m_PosY == mapy)
         {
            if( !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, mapx, mapy) )
            {
               AddConsoleString("Path blocked!");
               return false;
            }

            foundtarget = true;

            g_Player->m_CurrentManaPoints -=  g_SpecialAbilities[ABILITY_SMITE].m_ManaCost;

            //  This is the basic "bolt" animation"
            InariaEffect* tempanimation = new InariaEffect;
            tempanimation->Init( g_Star,
               g_Offset + ((g_ViewRange) * g_TileSize + 4),
               g_Offset + ((g_ViewRange) * g_TileSize + 4),
               g_Offset + ((mapx - g_Player->m_PlayerPosX + g_ViewRange) * g_TileSize + 4),
               g_Offset + ((mapy - g_Player->m_PlayerPosY + g_ViewRange) * g_TileSize + 4),
               50 * LineLength(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY), true, 1, "", "", 255, 255, 128 );
            g_AnimationList.push_back(tempanimation);

            //  Put the HURTING on him!
            (*node)->m_AI = 3;  //  If it wasn't hostile before, it is now.
            (*node)->m_Attitude = false;

            if( DoRPGCheck(g_Player->GetIntelligenceBonus() + g_Player->GetWillWithBonus(), (*node)->m_Will ) )
            {
               (*node)->DoDamage( HalfNHalf( g_Player->GetWillWithBonus()) );
               g_SpecialAbilities[ABILITY_SMITE].m_Level++;
               previoustarget = (*node);
               if( g_SpecialAbilities[ABILITY_SMITE].m_Level % 16 == 0 )
                  AddConsoleString("Your Smite spell has improved! It will now hit more targets.", 255, 128, 255 );

               //                        DrawSprite(23, g_Input->m_MouseX - (int(g_Input->m_MouseX) % 32) + 8, g_Input->m_MouseY - (int(g_Input->m_MouseY) % 32) + 8);
            }
            else
            {
               string _tempString = "You missed the " + (*node)->m_Name + ".";
               AddConsoleString(_tempString);
               InariaEffect* tempanimation = new InariaEffect;
               tempanimation->Init( NULL, 0, 0, 0, 0, 250, true, 0, "Sounds/miss1.wav" );
               g_AnimationList.push_back(tempanimation);
               return true; //  Can't bounce off a miss.
            }

         }
      }
      if( !foundtarget )
      {
         AddConsoleString("No one there!");
         return false;
      }
      if( smitecounter == 0 )
         return true;
   }
   else
   {
      return false;
   }

   //  Now do smites.
   //  Find another target that is not the previous target, is alive, and is in range of the player.
   while(smitecounter != 0)
   {
      list<NPC*>::iterator node = g_Map->m_NPCList.begin();
      bool foundtarget = false;
      for(node; node != g_Map->m_NPCList.end(); ++node)
      {
         if( (*node) != previoustarget && !(*node)->m_IsDead && (*node)->m_Attitude == 0  && (*node)->m_Name != "Oubliette Palsied Woman" )
         {
            int _distanceFromAvatar = int(pow(((*node)->m_PosX - g_Player->m_PlayerPosX), 2.0f) + pow(((*node)->m_PosY - g_Player->m_PlayerPosY), 2.0f));
            if(_distanceFromAvatar <= 25 )
            {
				if( CheckPath( previoustarget->m_PosX, previoustarget->m_PosY, (*node)->m_PosX, (*node)->m_PosY ) )
               {
                  foundtarget = true;
                  InariaEffect* tempanimation = new InariaEffect;
                  tempanimation->Init( g_Tiles[136],
                     g_Offset + ((previoustarget->m_PosX - g_Player->m_PlayerPosX + g_ViewRange) * g_TileSize),
                     g_Offset + ((previoustarget->m_PosY - g_Player->m_PlayerPosY + g_ViewRange) * g_TileSize),
                     g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX + g_ViewRange) * g_TileSize),
                     g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY + g_ViewRange) * g_TileSize),
                     150 );
                  g_AnimationList.push_back(tempanimation);
                  //  Put the HURTING on him!
                  (*node)->m_AI = 3;  //  If it wasn't hostile before, it is now.
                  (*node)->m_Attitude = false;

                  if( DoRPGCheck( g_Player->GetIntelligenceWithBonus() + g_Player->GetWillWithBonus(), (*node)->m_Will) )
                  {
                     (*node)->DoDamage( HalfNHalf( g_Player->GetWillWithBonus() ) );
                     g_SpecialAbilities[ABILITY_SMITE].m_Level++;
                     if( g_SpecialAbilities[ABILITY_SMITE].m_Level % 16 == 0 )
                        AddConsoleString("Your Smite spell has improved! It will now hit more targets.", 255, 128, 255 );
                     previoustarget = (*node);
                     --smitecounter;
                     if( smitecounter == 0 )
                        return true;
                     //                        DrawSprite(23, g_Input->m_MouseX - (int(g_Input->m_MouseX) % 32) + 8, g_Input->m_MouseY - (int(g_Input->m_MouseY) % 32) + 8);
                  }
                  //  If a smite misses, that's it - no more bounces.
                  else
                  {
                     return true;
                  }
               }
            }
         }
      }
      if(!foundtarget)
         return true; //  Couldn't find a smite target, stop looping.
   }
   return true;
}

bool MainState::DoHeal()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_HEAL].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   if( g_Player->m_CurrentHitPoints == g_Player->m_MaxHitPoints && g_Player->m_PoisonCounter == 0 )
   {
      AddConsoleString("No need to heal!");
      return false;
   }

   g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_HEAL].m_ManaCost;
   char temp[64];

   float amounttoheal;
   if( g_Player->GetWillWithBonus() < 5 )
      amounttoheal = g_Player->m_MaxHitPoints * .15f;
   else if( g_Player->GetWillWithBonus() < 9 )
      amounttoheal = g_Player->m_MaxHitPoints * .33f;
   else
      amounttoheal = g_Player->m_MaxHitPoints * .50f;
   if(amounttoheal < 1)
      amounttoheal = 1;

   sprintf(temp, "You heal yourself for %d points.", int(amounttoheal) );
   AddConsoleString(string(temp), 255, 255, 128 );

   g_Player->m_CurrentHitPoints += amounttoheal;
   if(g_Player->m_CurrentHitPoints > g_Player->m_MaxHitPoints )
      g_Player->m_CurrentHitPoints = g_Player->m_MaxHitPoints;

   if( g_Player->m_PoisonCounter > 0 )
   {
      AddConsoleString("Healing cures your poison.");
      g_Player->m_PoisonCounter = 0;
   }

   return true;
}

bool MainState::DoMagicArmor()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_MAGICARMOR].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_MAGICARMOR].m_ManaCost;

   char temp[64];
   sprintf(temp, "Your armor is now enchanted for 10 turns.", g_Player->GetIntelligenceWithBonus() / 2 );
   AddConsoleString(string(temp), 255, 255, 128 );
   g_SpecialAbilities[ABILITY_MAGICARMOR].m_Duration = 11;

   return true;
}

bool MainState::DoExtraDamage()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_DAMAGEBONUS].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_DAMAGEBONUS].m_ManaCost;

   char temp[64];
   sprintf(temp, "Your weapon is now enchanted for 10 turns.", g_Player->GetIntelligenceWithBonus() / 2 );
   AddConsoleString(string(temp), 255, 255, 128 );
   g_SpecialAbilities[ABILITY_DAMAGEBONUS].m_Duration = 11;

   return true;
}

bool MainState::DoMagicUnlock()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_MAGICUNLOCK].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      list<Item*>::iterator itemnode = g_Map->m_ItemList.begin();
      for( itemnode; itemnode != g_Map->m_ItemList.end(); ++itemnode)
      {
         if((*itemnode)->m_PosX == mapx && (*itemnode)->m_PosY == mapy )
         {
            if( (*itemnode)->m_Pickable )
            {
               if( LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*itemnode)->m_PosX, (*itemnode)->m_PosY) < 1.5 )
               {

                  g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_MAGICUNLOCK].m_ManaCost;
                  if( DoRPGCheck( g_Player->m_Intelligence, 20 - g_Player->m_Intelligence ) )
                  {
                     AddConsoleString( (*itemnode)->m_Name + " removed!");
                     itemnode = g_Map->m_ItemList.erase(itemnode);
                     return true;
                  }
                  else
                  {
                     AddConsoleString("Spell failed!");
                     return false;
                  }
               }
               else
               {
                  AddConsoleString("You are too far away from that.");
               }
            }
         }
      }
   }
   return false;
}

bool MainState::DoMap()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_MAP].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_MAP].m_ManaCost;
   g_Map->m_Peer = true;

   return true;
}

bool MainState::DoTeleport()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_TELEPORT].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   if(g_Input->m_MouseX < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) && g_Input->m_MouseX > g_Offset && g_Input->m_MouseY > g_Offset && g_Input->m_MouseY < g_Offset + ( (g_ViewRange * 2 + 1) * g_TileSize) )
   {
      //  Turn the screen coordinates into map coordinates.
      int mapx = ((g_Input->m_MouseX - g_Offset) / g_TileSize);
      mapx = mapx - g_ViewRange + g_Player->m_PlayerPosX;
      int mapy = ((g_Input->m_MouseY - g_Offset) / g_TileSize);
      mapy = mapy - g_ViewRange + g_Player->m_PlayerPosY;

      //  Make sure the map cell is passable.
      if( !g_Map->m_map[mapx][mapy].m_passable )
      {
            AddConsoleString("Target blocked!");
            return false;
      }

      //  Make sure there's no impassable item on the cell.

      list<Item*>::iterator itemnode = g_Map->m_ItemList.begin();
      for( itemnode; itemnode != g_Map->m_ItemList.end(); ++itemnode)
      {
         if((*itemnode)->m_PosX == mapx && (*itemnode)->m_PosY == mapy && !(*itemnode)->m_Passable )
         {
            AddConsoleString("Target blocked!");
            return false;
         }
      }

      //  Make sure there's no NPC on the cell.
      list<NPC*>::iterator npcnode = g_Map->m_NPCList.begin();
      for( npcnode; npcnode != g_Map->m_NPCList.end(); ++npcnode)
      {
         if((*npcnode)->m_PosX == mapx && (*npcnode)->m_PosY == mapy )
         {
            AddConsoleString("Target blocked!");
            return false;
         }
      }

      //  Make sure there's a clear path.
      if(! CheckPath( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, mapx, mapy, false ) )
      {
         AddConsoleString("Path blocked!");
         return false;
      }

      //  Teleport.
      g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_TELEPORT].m_ManaCost;
      g_Player->m_PlayerPosX = mapx;
      g_Player->m_PlayerPosY = mapy;
      //  If the player is standing on an item, do that item's OnStand().
      list<Item*>::iterator node = g_Map->m_ItemList.begin();
      for( node; node != g_Map->m_ItemList.end(); ++node )
      {
         if( g_Player->m_PlayerPosX == (*node)->m_PosX && g_Player->m_PlayerPosY == (*node)->m_PosY )
         {
            (*node)->OnStand();
         }
      }
      return true;
   }
   return false;
}

bool MainState::DoTownPortal()
{
   if( g_Player->m_CurrentManaPoints < g_SpecialAbilities[ABILITY_TOWNPORTAL].m_ManaCost )
   {
      AddConsoleString("Not enough mana.");
      return false;
   }

   g_Player->m_CurrentManaPoints -= g_SpecialAbilities[ABILITY_TOWNPORTAL].m_ManaCost;

   if( g_Player->m_KilledEvilOne )
      g_Map->LoadMap("Maps/intro.map");
   else
      g_Map->LoadMap("Maps/inaria.map");

   g_Player->m_PlayerPosX = 50;
   g_Player->m_PlayerPosY = 34;
   return 1;
}

