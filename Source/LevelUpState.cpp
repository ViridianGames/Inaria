#include "LevelUpState.h"
#include "GameGlobals.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  LevelUpState
////////////////////////////////////////////////////////////////////////////////

LevelUpState::~LevelUpState()
{

}

void LevelUpState::Init(const std::string& configfile)
{
   m_PosX = 76;
   m_PosY = 107;

   m_StatsY = 50;

   g_FirstTime = false;
}

void LevelUpState::Shutdown()
{

}

void LevelUpState::Update()
{
   DoPlayerInput();


}

void LevelUpState::Draw()
{
   //   g_Display->BlitImageRect( g_Mask, 0, 0, 352, 352, g_Offset, g_Offset);

   g_Display->BlitImageRect( g_Mask, 0, 0, 427, 199, m_PosX + 1, m_PosY + 1);
   g_Display->DrawBox(m_PosX, m_PosY, 428, 200);

   char temp[64];
   if(g_FirstTime)
   {
      g_Font->DrawTextCentered( "Create Your Character", m_PosX + (428 / 2), 112 );
   }
   else
      g_Font->DrawTextCentered( "Improve Your Character", m_PosX + (428 / 2), 112 );

   sprintf(temp, "Points Left: %d", g_Player->m_StatPoints );
   int stringlength = g_Font->GetStringMetrics( string(temp) );
   g_Font->DrawTextCentered( string(temp), m_PosX + (428 / 2), 128 );



   //  STATS

   // Strength
   g_Display->DrawSprite( g_Tiles[64], m_PosX + 20, m_PosY + m_StatsY );
   g_Font->DrawTextA( "Strength", m_PosX + 40, m_PosY + m_StatsY);
   sprintf(temp, "%d", g_Player->m_Strength );
   if( m_StrAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Strength + m_StrAdd);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Strength );
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY);
   }

   // Dexterity
   g_Display->DrawSprite( g_Tiles[107], m_PosX + 20, m_PosY + m_StatsY + 25 );
   g_Font->DrawTextA( "Dexterity", m_PosX + 40, m_PosY + m_StatsY + 25 );
   if( m_DexAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Dexterity + m_DexAdd);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 25, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Dexterity);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 25);
   }

   // Intelligence
   g_Display->DrawSprite( g_Tiles[109], m_PosX + 20, m_PosY + m_StatsY + 50 );
   g_Font->DrawTextA( "Intelligence", m_PosX + 40, m_PosY + m_StatsY + 50 );
   sprintf(temp, "%d", g_Player->m_Intelligence);
   if( m_IntAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Intelligence + m_IntAdd);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 50, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Intelligence);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 50);
   }

   //  Endurance
   g_Display->DrawSprite( g_Tiles[108], m_PosX + 20, m_PosY + m_StatsY + 75 );
   g_Font->DrawTextA( "Endurance", m_PosX + 40, m_PosY + m_StatsY + 75 );
   sprintf(temp, "%d", g_Player->m_Endurance);
   if( m_EndAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Endurance + m_EndAdd);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 75, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Endurance);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 75);
   }

   //  Will
   g_Display->DrawSprite( g_Tiles[110], m_PosX + 20, m_PosY + m_StatsY + 100);
   g_Font->DrawTextA( "Will", m_PosX + 40, m_PosY + m_StatsY + 100 );
   sprintf(temp, "%d", g_Player->m_Will);
   if( m_WilAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Will + m_WilAdd);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 100, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Will);
      g_Font->DrawTextA( string(temp), m_PosX + 170, m_PosY + m_StatsY + 100);
   }


   //  ABILITIES
   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if( g_SpecialAbilities[invcount].m_Active == true )
      {
         g_Display->DrawSprite( g_Tiles[ g_SpecialAbilities[invcount].m_Tile], m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize) );
      }
      else
      {
         g_Display->DrawSprite( g_Tiles[ g_SpecialAbilities[invcount].m_Tile], m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize), 128, 128, 128 );
      }
   }


   //  FINALIZE
   sprintf(temp, "Finished" );
   stringlength = g_Font->GetStringMetrics( string(temp) );
   if( DrawButton("Finished", m_PosX + 428 - g_Font->GetStringMetrics("Finished") - 24, m_PosY + 180 ) )
   {
      //  Finalize the player's selections
      g_Player->m_Strength += m_StrAdd;
      g_Player->m_Dexterity += m_DexAdd;
      g_Player->m_Intelligence += m_IntAdd;
      g_Player->m_Endurance += m_EndAdd;
      g_Player->m_Will += m_WilAdd;

      g_StateMachine->PopState();
   }

   if( DrawButton("Undo", m_PosX + 20, m_PosY + 180 ) )
   {
      g_Player->m_StatPoints = m_OriginalStatPoints;
      m_StrAdd = 0;
      m_DexAdd = 0;
      m_IntAdd = 0;
      m_EndAdd = 0;
      m_WilAdd = 0;

      for( int i = 0; i < m_BoughtAbilities.size(); ++i )
      {
         g_SpecialAbilities[m_BoughtAbilities[i]].m_Active = false;
      }
   }

   //  TOOLTIPS
   stringstream thestream;
   thestream.str("");

   if( g_Input->IsMouseInRegion( m_PosX + 20, m_PosY + m_StatsY, m_PosX + 200, m_PosY + m_StatsY + 20 ) )
   {
      if( g_Player->m_Strength >= 12 )
      {
         thestream << "Your Strength is already at maximum.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }

      else if( g_Player->m_StatPoints >= g_Player->m_Strength + m_StrAdd )
      {
         thestream << "Upgrade Strength for " << g_Player->m_Strength + m_StrAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3 );
      }
      else
      {
         thestream << "Upgrade Strength for " << g_Player->m_Strength + m_StrAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }
   }

   if( g_Input->IsMouseInRegion( m_PosX + 20, m_PosY + m_StatsY + 25, m_PosX + 200, m_PosY + m_StatsY + 25 + 20 ) )
   {
      if( g_Player->m_Dexterity >= 12 )
      {
         thestream << "Your Dexterity is already at maximum.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Dexterity + m_DexAdd)
      {
         thestream << "Upgrade Dexterity for " << g_Player->m_Dexterity + m_DexAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3 );
      }
      else
      {
         thestream << "Upgrade Dexterity for " << g_Player->m_Dexterity + m_DexAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }
   }

   if( g_Input->IsMouseInRegion( m_PosX + 20, m_PosY + m_StatsY + 50, m_PosX + 200, m_PosY + m_StatsY + 50 + 20 ) )
   {
      if( g_Player->m_Intelligence >= 12 )
      {
         thestream << "Your Intelligence is already at maximum.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Intelligence + m_IntAdd )
      {
         thestream << "Upgrade Intelligence for " << g_Player->m_Intelligence + m_IntAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3 );
      }
      else
      {
         thestream << "Upgrade Intelligence for " << g_Player->m_Intelligence + m_IntAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }
   }

   if( g_Input->IsMouseInRegion( m_PosX + 20, m_PosY + m_StatsY + 75, m_PosX + 200, m_PosY + m_StatsY + 75 + 20 ) )
   {
      if( g_Player->m_Endurance >= 12 )
      {
         thestream << "Your Endurance is already at maximum.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Endurance + m_EndAdd)
      {
         thestream << "Upgrade Endurance for " << g_Player->m_Endurance + m_EndAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3 );
      }
      else
      {
         thestream << "Upgrade Endurance for " << g_Player->m_Endurance + m_EndAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }
   }

   if( g_Input->IsMouseInRegion( m_PosX + 20, m_PosY + m_StatsY + 100, m_PosX + 200, m_PosY + m_StatsY + 100 + 20 ) )
   {
      if( g_Player->m_Will >= 12 )
      {
         thestream << "Your Will is already at maximum.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Will + m_WilAdd)
      {
         thestream << "Upgrade Will for " << g_Player->m_Will + m_WilAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3 );
      }
      else
      {
         thestream << "Upgrade Will for " << g_Player->m_Will + m_WilAdd << " points.";
         DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 3, 255, 128, 128 );
      }
   }

   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if( g_Input->IsMouseInRegion( m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize), m_PosX + 220 + ((invcount % 4) * 47) + 32, m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize) + 32 ) )
      {
         if( g_SpecialAbilities[invcount].m_Active == false )
         {
            if( g_Player->m_StatPoints >= g_SpecialAbilities[invcount].m_SkillPointCost )
            {
               thestream.str("");
               thestream << "Purchase " << g_SpecialAbilities[invcount].m_Name << " for " << g_SpecialAbilities[invcount].m_SkillPointCost << " points.";
               DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 2 );
            }
            else
            {
               thestream.str("");
               thestream << "Purchase " << g_SpecialAbilities[invcount].m_Name << " for " << g_SpecialAbilities[invcount].m_SkillPointCost << " points.";
               DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 2, 255, 128, 128 );
            }
         }
         else
         {
            thestream.str("");
            thestream << g_SpecialAbilities[invcount].m_Name << " has already been purchased.";
            DrawToolTip( g_SmallFont,  thestream.str(), g_Input->m_MouseX, g_Input->m_MouseY, 2, 255, 128, 128 );
         }
      }
   }

   g_Display->BlitImage(g_Cursors[0], g_Input->m_MouseX, g_Input->m_MouseY);


}

void LevelUpState::OnEnter()
{
   m_StrAdd = 0;
   m_DexAdd = 0;
   m_EndAdd = 0;
   m_WilAdd = 0;
   m_IntAdd = 0;

   m_OriginalStatPoints = g_Player->m_StatPoints;
   m_BoughtAbilities.clear();
}

void LevelUpState::OnExit()
{
   if( m_EndAdd > 0 || m_WilAdd > 0 )
      g_Player->RecalculateHitpointsAndMana();

   g_FirstTime = false;
}

void LevelUpState::DoPlayerInput()
{
   if( g_Input->WasLButtonClickedInRegion( m_PosX + 20, m_PosY + m_StatsY, m_PosX + 200, m_PosY + m_StatsY + 20 ) )
   {
      if( g_Player->m_Strength + m_StrAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Strength + m_StrAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Strength + m_StrAdd;
         ++m_StrAdd;

      }
   }

   if( g_Input->WasLButtonClickedInRegion( m_PosX + 20, m_PosY + m_StatsY + 25, m_PosX + 200, m_PosY + m_StatsY + 25 + 20 ) )
   {
      if( g_Player->m_Dexterity + m_DexAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Dexterity + m_DexAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Dexterity + m_DexAdd;
         ++m_DexAdd;

      }

   }

   if( g_Input->WasLButtonClickedInRegion( m_PosX + 20, m_PosY + m_StatsY + 50, m_PosX + 200, m_PosY + m_StatsY + 50 + 20 ) )
   {
      if( g_Player->m_Intelligence + m_IntAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Intelligence + m_IntAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Intelligence + m_IntAdd;
         ++m_IntAdd;

      }

   }

   if( g_Input->WasLButtonClickedInRegion( m_PosX + 20, m_PosY + m_StatsY + 75, m_PosX + 200, m_PosY + m_StatsY + 75 + 20 ) )
   {
      if( g_Player->m_Endurance + m_EndAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Endurance + m_EndAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Endurance + m_EndAdd;
         ++m_EndAdd;

      }

   }

   if( g_Input->WasLButtonClickedInRegion( m_PosX + 20, m_PosY + m_StatsY + 100, m_PosX + 200, m_PosY + m_StatsY + 100 + 20 ) )
   {
      if( g_Player->m_Will + m_WilAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Will + m_WilAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Will + m_WilAdd;
         ++m_WilAdd;

      }
   }

   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if( g_Input->WasLButtonClickedInRegion( m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize), m_PosX + 220 + ((invcount % 4) * 47) + 32, m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize) + 32 ) )
      {
         if( g_SpecialAbilities[invcount].m_Active == false )
         {
            if( g_Player->m_StatPoints >= g_SpecialAbilities[invcount].m_SkillPointCost )
            {
               g_Player->m_StatPoints -= g_SpecialAbilities[invcount].m_SkillPointCost;
               g_SpecialAbilities[invcount].m_Active = true;
               AddConsoleString("You have unlocked a new ability!", 64, 255, 64);
               AddConsoleString(g_SpecialAbilities[invcount].m_Name + ": " + g_SpecialAbilities[invcount].m_Description, 64, 255, 64);
               m_BoughtAbilities.push_back(invcount);
            }
         }
      }
   }
}


