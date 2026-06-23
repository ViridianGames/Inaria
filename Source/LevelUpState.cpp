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
   //   DrawImageRectAt( g_Mask, 0, 0, 352, 352, g_Offset, g_Offset);

   DrawImageRectAt( g_Mask, 0, 0, 427, 199, m_PosX + 1, m_PosY + 1);
   DrawBoxAt(m_PosX, m_PosY, 428, 200);

   char temp[64];
   if(g_FirstTime)
   {
      DrawTextCenteredAt(g_font.get(), g_fontSize,  "Create Your Character", m_PosX + (428 / 2), 112 );
   }
   else
      DrawTextCenteredAt(g_font.get(), g_fontSize,  "Improve Your Character", m_PosX + (428 / 2), 112 );

   sprintf(temp, "Points Left: %d", g_Player->m_StatPoints );
   int stringlength = GetStringMetrics(g_font.get(), g_fontSize,  string(temp) );
   DrawTextCenteredAt(g_font.get(), g_fontSize,  string(temp), m_PosX + (428 / 2), 128 );



   //  STATS

   // Strength
   DrawSpriteAt( g_Tiles[64], m_PosX + 20, m_PosY + m_StatsY );
   DrawTextAt(g_font.get(), g_fontSize,  "Strength", m_PosX + 40, m_PosY + m_StatsY);
   sprintf(temp, "%d", g_Player->m_Strength );
   if( m_StrAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Strength + m_StrAdd);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Strength );
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY);
   }

   // Dexterity
   DrawSpriteAt( g_Tiles[107], m_PosX + 20, m_PosY + m_StatsY + 25 );
   DrawTextAt(g_font.get(), g_fontSize,  "Dexterity", m_PosX + 40, m_PosY + m_StatsY + 25 );
   if( m_DexAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Dexterity + m_DexAdd);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 25, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Dexterity);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 25);
   }

   // Intelligence
   DrawSpriteAt( g_Tiles[109], m_PosX + 20, m_PosY + m_StatsY + 50 );
   DrawTextAt(g_font.get(), g_fontSize,  "Intelligence", m_PosX + 40, m_PosY + m_StatsY + 50 );
   sprintf(temp, "%d", g_Player->m_Intelligence);
   if( m_IntAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Intelligence + m_IntAdd);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 50, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Intelligence);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 50);
   }

   //  Endurance
   DrawSpriteAt( g_Tiles[108], m_PosX + 20, m_PosY + m_StatsY + 75 );
   DrawTextAt(g_font.get(), g_fontSize,  "Endurance", m_PosX + 40, m_PosY + m_StatsY + 75 );
   sprintf(temp, "%d", g_Player->m_Endurance);
   if( m_EndAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Endurance + m_EndAdd);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 75, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Endurance);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 75);
   }

   //  Will
   DrawSpriteAt( g_Tiles[110], m_PosX + 20, m_PosY + m_StatsY + 100);
   DrawTextAt(g_font.get(), g_fontSize,  "Will", m_PosX + 40, m_PosY + m_StatsY + 100 );
   sprintf(temp, "%d", g_Player->m_Will);
   if( m_WilAdd > 0 )
   {
      sprintf(temp, "%d", g_Player->m_Will + m_WilAdd);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 100, 255, 255, 128 );
   }
   else
   {
      sprintf(temp, "%d", g_Player->m_Will);
      DrawTextAt(g_font.get(), g_fontSize,  string(temp), m_PosX + 170, m_PosY + m_StatsY + 100);
   }


   //  ABILITIES
   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if( g_SpecialAbilities[invcount].m_Active == true )
      {
         DrawSpriteAt( g_Tiles[ g_SpecialAbilities[invcount].m_Tile], m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize) );
      }
      else
      {
         DrawSpriteAt( g_Tiles[ g_SpecialAbilities[invcount].m_Tile], m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize), 128, 128, 128 );
      }
   }


   //  FINALIZE
   sprintf(temp, "Finished" );
   stringlength = GetStringMetrics(g_font.get(), g_fontSize,  string(temp) );
   if( DrawButton("Finished", m_PosX + 428 - GetStringMetrics(g_font.get(), g_fontSize, "Finished") - 24, m_PosY + 180 ) )
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

   if( IsMouseInDesignRegion( m_PosX + 20, m_PosY + m_StatsY, m_PosX + 200, m_PosY + m_StatsY + 20 ) )
   {
      if( g_Player->m_Strength >= 12 )
      {
         thestream << "Your Strength is already at maximum.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }

      else if( g_Player->m_StatPoints >= g_Player->m_Strength + m_StrAdd )
      {
         thestream << "Upgrade Strength for " << g_Player->m_Strength + m_StrAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3 );
      }
      else
      {
         thestream << "Upgrade Strength for " << g_Player->m_Strength + m_StrAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }
   }

   if( IsMouseInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 25, m_PosX + 200, m_PosY + m_StatsY + 25 + 20 ) )
   {
      if( g_Player->m_Dexterity >= 12 )
      {
         thestream << "Your Dexterity is already at maximum.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Dexterity + m_DexAdd)
      {
         thestream << "Upgrade Dexterity for " << g_Player->m_Dexterity + m_DexAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3 );
      }
      else
      {
         thestream << "Upgrade Dexterity for " << g_Player->m_Dexterity + m_DexAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }
   }

   if( IsMouseInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 50, m_PosX + 200, m_PosY + m_StatsY + 50 + 20 ) )
   {
      if( g_Player->m_Intelligence >= 12 )
      {
         thestream << "Your Intelligence is already at maximum.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Intelligence + m_IntAdd )
      {
         thestream << "Upgrade Intelligence for " << g_Player->m_Intelligence + m_IntAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3 );
      }
      else
      {
         thestream << "Upgrade Intelligence for " << g_Player->m_Intelligence + m_IntAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }
   }

   if( IsMouseInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 75, m_PosX + 200, m_PosY + m_StatsY + 75 + 20 ) )
   {
      if( g_Player->m_Endurance >= 12 )
      {
         thestream << "Your Endurance is already at maximum.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Endurance + m_EndAdd)
      {
         thestream << "Upgrade Endurance for " << g_Player->m_Endurance + m_EndAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3 );
      }
      else
      {
         thestream << "Upgrade Endurance for " << g_Player->m_Endurance + m_EndAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }
   }

   if( IsMouseInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 100, m_PosX + 200, m_PosY + m_StatsY + 100 + 20 ) )
   {
      if( g_Player->m_Will >= 12 )
      {
         thestream << "Your Will is already at maximum.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }


      if( g_Player->m_StatPoints >= g_Player->m_Will + m_WilAdd)
      {
         thestream << "Upgrade Will for " << g_Player->m_Will + m_WilAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3 );
      }
      else
      {
         thestream << "Upgrade Will for " << g_Player->m_Will + m_WilAdd << " points.";
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 3, 255, 128, 128 );
      }
   }

   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if( IsMouseInDesignRegion( m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize), m_PosX + 220 + ((invcount % 4) * 47) + 32, m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize) + 32 ) )
      {
         if( g_SpecialAbilities[invcount].m_Active == false )
         {
            if( g_Player->m_StatPoints >= g_SpecialAbilities[invcount].m_SkillPointCost )
            {
               thestream.str("");
               thestream << "Purchase " << g_SpecialAbilities[invcount].m_Name << " for " << g_SpecialAbilities[invcount].m_SkillPointCost << " points.";
               DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 2 );
            }
            else
            {
               thestream.str("");
               thestream << "Purchase " << g_SpecialAbilities[invcount].m_Name << " for " << g_SpecialAbilities[invcount].m_SkillPointCost << " points.";
               DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 2, 255, 128, 128 );
            }
         }
         else
         {
            thestream.str("");
            thestream << g_SpecialAbilities[invcount].m_Name << " has already been purchased.";
            DrawDesignToolTip( g_smallFont.get(), g_smallFontSize,  thestream.str(), GetDesignMouseX(), GetDesignMouseY(), 2, 255, 128, 128 );
         }
      }
   }

   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());


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
   if( WasLButtonClickedInDesignRegion( m_PosX + 20, m_PosY + m_StatsY, m_PosX + 200, m_PosY + m_StatsY + 20 ) )
   {
      if( g_Player->m_Strength + m_StrAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Strength + m_StrAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Strength + m_StrAdd;
         ++m_StrAdd;

      }
   }

   if( WasLButtonClickedInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 25, m_PosX + 200, m_PosY + m_StatsY + 25 + 20 ) )
   {
      if( g_Player->m_Dexterity + m_DexAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Dexterity + m_DexAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Dexterity + m_DexAdd;
         ++m_DexAdd;

      }

   }

   if( WasLButtonClickedInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 50, m_PosX + 200, m_PosY + m_StatsY + 50 + 20 ) )
   {
      if( g_Player->m_Intelligence + m_IntAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Intelligence + m_IntAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Intelligence + m_IntAdd;
         ++m_IntAdd;

      }

   }

   if( WasLButtonClickedInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 75, m_PosX + 200, m_PosY + m_StatsY + 75 + 20 ) )
   {
      if( g_Player->m_Endurance + m_EndAdd == 12 )
         return;

      if( g_Player->m_StatPoints >= g_Player->m_Endurance + m_EndAdd  )
      {

         g_Player->m_StatPoints -= g_Player->m_Endurance + m_EndAdd;
         ++m_EndAdd;

      }

   }

   if( WasLButtonClickedInDesignRegion( m_PosX + 20, m_PosY + m_StatsY + 100, m_PosX + 200, m_PosY + m_StatsY + 100 + 20 ) )
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
      if( WasLButtonClickedInDesignRegion( m_PosX + 220 + ((invcount % 4) * 47), m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize), m_PosX + 220 + ((invcount % 4) * 47) + 32, m_PosY + 45 + ((invcount / 4) * g_InventoryTileSize) + 32 ) )
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


