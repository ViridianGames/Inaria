
#include "GameGlobals.h"
#include <list>
#include <complex>

typedef BaseUnit parent;

using namespace std;

NPC::~NPC()
{
   parent::Shutdown();
   Shutdown();
}

void NPC::Init(const std::string& configfile)
{
   parent::Init(configfile);

   m_NumberOfTalks = 0;

   m_LookResponse = m_UnitConfig["lookresponse"].stringdata;

   m_Strength = 2;
   m_Dexterity = 2;
   m_Will = 2;
   m_Endurance = 2;
   m_Intelligence = 2;

   m_HitPoints    = ( m_Endurance + ( Factorial(g_Map->m_MapLevel) ) );
   m_MaxHitPoints = m_HitPoints;
   m_Armor        = 0;
   m_Attitude     = bool(m_UnitConfig["attitude"].numdata);
   m_AI           = m_UnitConfig["ai"].numdata;
   m_Name         = m_UnitConfig["name"].stringdata;
   m_Tile         = m_UnitConfig["tile"].numdata;
   m_Type         = m_UnitConfig["type"].numdata;
   if(m_UnitConfig.find("numberoftalks") != m_UnitConfig.end())
      m_NumberOfTalks = m_UnitConfig["numberoftalks"].numdata;
   else
      m_NumberOfTalks = 0;

   m_CurrentTalk = 0;

   m_Talks.clear();
   //  Load all the talks
   if(m_NumberOfTalks == 0)
   {
      m_Talks.push_back(m_UnitConfig["talkresponse"].stringdata);
      m_NumberOfTalks = 1;
   }
   else
   {
      for(int i = 0; i < m_NumberOfTalks; ++i)
      {
         stringstream output;
         output.str("");
         output << "talk" << i;
         m_Talks.push_back(m_UnitConfig[output.str()].stringdata);
      }
   }

   m_AttackRange = 1.5;
   m_AttackRange = m_UnitConfig["range"].numdata;

   m_Inventory.clear();


   m_MovedLastTurn = false;

   m_HasUsedSpecialAbility = false;
}

void NPC::Shutdown()
{
   parent::Shutdown();

}

void NPC::Update()
{
   parent::Update();

   //  Quiet, you.  You're dead.
   if(m_IsDead == true)
   {
      return;
   }


   if( m_Name == "Oubliette Palsied Woman" )
   {
      for( list<NPC*>::iterator node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
      {
         if( (*node)->m_Name == "Mendikar the MadMaker" )
         {
            if( LineLength( m_PosX , m_PosY, (*node)->m_PosX, (*node)->m_PosY ) < 4 ) 
            {
               m_Attitude = 0;
               m_AI = 3;
               AddConsoleString(m_Talks[7], 255, 64, 255);
            }
         }
      }
   }


   if( m_Attitude == 0 && g_StateMachine->GetCurrentState() != STATE_TITLESTATE) // enemy barks
   {
      //  See how far we are from the avatar.  Ah, Pythagoras, how we adore you.
      int _distanceFromAvatar = int(pow((m_PosX - g_Player->m_PlayerPosX), 2.0f) + pow((m_PosY - g_Player->m_PlayerPosY), 2.0f));

      if(_distanceFromAvatar < 25)
      {

         if( m_Name == "Mendikar the MadMaker" )
         {
            if( g_RNG->Random(2) == 0 )
            {
               AddConsoleString( m_Talks[ g_RNG->Random( 7 )], 255, 64, 255 );
            }
         }

         if( m_Name == "Mind Thresher" )
         {
            if( g_RNG->Random(2) == 0 )
            {
               AddConsoleString( m_Talks[ g_RNG->Random( 7 )], 255, 64, 255 );
            }
         }

         
         else if( m_Name == "Oubliette Palsied Woman" )
         {
            if( g_RNG->Random(2) == 0 )
            {
               AddConsoleString( m_Talks[ 8 + g_RNG->Random( 2 )], 255, 64, 255 );
            }
         }

         else if( m_Name == "The Evil One" )
         {
            if( g_RNG->Random(2) == 0 )
            {
               if( float(m_HitPoints) / float(m_MaxHitPoints) > .75f )
               {
                  AddConsoleString( m_Talks[g_RNG->Random( 4 ) + 13], 255, 64, 255 );
               }
               else if( float(m_HitPoints) / float(m_MaxHitPoints) > .35f )
               {
                  AddConsoleString( m_Talks[g_RNG->Random( 4 ) + 17], 255, 64, 255 );
               }
               else
               {
                  AddConsoleString( m_Talks[g_RNG->Random( 2 ) + 21], 255, 64, 255 );
               }
            }
         }
         else if( g_RNG->Random(10) == 0 )
         {
            AddConsoleString( m_Talks[g_RNG->Random(m_NumberOfTalks)], 255, 64, 255);
         }
      }
   }

   //  If we're next to a non-aligned NPC, take a swipe at him.
   //  If we're hostile and next to the avatar, take a swipe at him.
   if( g_StateMachine->GetCurrentState() == STATE_TITLESTATE )
   {
         //  Try to heal someone first.
         bool healedsomebody = false;
      //  Do healing first.
      if( m_Name == "Last Front Healer" )
      {
         //  Find a nearby friend that needs healing.
         list<NPC*>::iterator node = g_Map->m_NPCList.begin();
         for( node; node != g_Map->m_NPCList.end(); ++node )
         {
            if( !(*node)->m_IsDead && (*node) != this && LineLength( m_PosX, m_PosY, (*node)->m_PosX, (*node)->m_PosY) < 10  )
            {
               if( (*node)->m_Attitude == m_Attitude )
               {
                  if( (*node)->m_HitPoints < (*node)->m_MaxHitPoints )
                  {
                     int healing = HalfNHalf(m_Will);
                     stringstream writer;
                     writer.str("");
                     writer << "Last Front Healer healed " << (*node)->m_Name << " for " << healing << "!";
                     //                           AddConsoleString( writer.str(), 255, 64, 255 );
                     (*node)->m_HitPoints += healing;
                     if( (*node)->m_HitPoints > (*node)->m_MaxHitPoints )
                        (*node)->m_HitPoints = (*node)->m_MaxHitPoints;
                     healedsomebody = true;

                     int xoffset = g_RNG->Random(8);
                     InariaEffect* tempanimation = new InariaEffect;
                     tempanimation->Init( g_Bubble,
                        g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4 + 24,
                        g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                        500, false, 1, "", "", 255, 255, 128, 128 );
                     g_AnimationList.push_back(tempanimation);

                     xoffset = g_RNG->Random(8);
                     tempanimation = new InariaEffect;
                     tempanimation->Init( g_Bubble,
                        g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4 + 24,
                        g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                        500, false, 1, "", "", 255, 255, 128, 128 );
                     g_AnimationList.push_back(tempanimation);

                     xoffset = g_RNG->Random(8);
                     tempanimation = new InariaEffect;
                     tempanimation->Init( g_Bubble,
                        g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4 + 24,
                        g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                        500, true, 1, "", "", 255, 255, 128, 128 );
                     g_AnimationList.push_back(tempanimation);

                     break;
                  }
               }
            }
         }
      }

      else if( m_Name != "Last Front Healer" || healedsomebody == false )
      {
         for( list<NPC*>::iterator node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
         {
            if( (*node)->m_Attitude != m_Attitude && !(*node)->m_IsDead && LineLength( m_PosX, m_PosY, (*node)->m_PosX, (*node)->m_PosY ) <= m_AttackRange )
            {
               if( m_Name == "Slornite Thief" || m_Name == "Last Front Archer" )
               {
                  InariaEffect* tempanimation = new InariaEffect;

                  Sprite* arrow = NULL;
                  if( m_PosX == (*node)->m_PosX && m_PosY < (*node)->m_PosY ) //  Firing right
                     arrow = g_Tiles[141];

                  else if( m_PosX == (*node)->m_PosX && m_PosY > (*node)->m_PosY ) //  Firing left
                     arrow = g_Tiles[137];

                  else if( m_PosX < (*node)->m_PosX && m_PosY == (*node)->m_PosY ) //  Firing down
                     arrow = g_Tiles[139];

                  else if( m_PosX > (*node)->m_PosX && m_PosY == (*node)->m_PosY ) //  Firing up
                     arrow = g_Tiles[143];

                  else if( m_PosX < (*node)->m_PosX && m_PosY < (*node)->m_PosY ) //  Firing down-right
                     arrow = g_Tiles[140];

                  else if( m_PosX < (*node)->m_PosX && m_PosY > (*node)->m_PosY ) //  Firing up-right
                     arrow = g_Tiles[138];

                  else if( m_PosX > (*node)->m_PosX && m_PosY < (*node)->m_PosY ) //  Firing down-left
                     arrow = g_Tiles[142];

                  else //  Firing up-left
                     arrow = g_Tiles[144];

                  tempanimation->Init( arrow,
                     g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize,
                     g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize,
                     g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize,
                     g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize,

                     LineLength( m_PosX, m_PosY, (*node)->m_PosX, (*node)->m_PosY ) * 50, true );

                  g_AnimationList.push_back(tempanimation);
                  //               AddConsoleString(m_Name + " fires an arrow at " + (*node)->m_Name + ".");

                  Attack(*node);

               }
               else if( m_Name == "Slornite Mage" )
               {
                  InariaEffect* tempanimation = new InariaEffect;

                  tempanimation->Init( g_Star,
                     g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
                     g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                     g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
                     g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                     LineLength( m_PosX, m_PosY, (*node)->m_PosX, (*node)->m_PosY ) * 50, true, 1, "", "", 255, 64, 64 );

                  g_AnimationList.push_back(tempanimation);
                  //               AddConsoleString(m_Name + " attacks " + (*node)->m_Name + " with a fireball.");
                  Attack(*node);
               }

               else if( m_Name == "Last Front Healer" )
               {
                  InariaEffect* tempanimation = new InariaEffect;

                  tempanimation->Init( g_Star,
                     g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
                     g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                     g_Offset + (((*node)->m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
                     g_Offset + (((*node)->m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                     LineLength( m_PosX, m_PosY, (*node)->m_PosX, (*node)->m_PosY ) * 50, true, 1, "", "", 255, 255, 64 );

                  g_AnimationList.push_back(tempanimation);
                  //                           AddConsoleString(m_Name + " attacks " + (*node)->m_Name + " with holy power.");
                  Attack(*node);
                  break;
               }

               else
               {
                  Attack( (*node) );
               }
               return;
            }
         }
      }
   }

   //  If we're hostile and next to the avatar, take a swipe at him.
   else if(m_Attitude == 0 && LineLength( m_PosX, m_PosY, g_Player->m_PlayerPosX, g_Player->m_PlayerPosY ) <= m_AttackRange && CheckPath(m_PosX, m_PosY, g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, false ) )
   {
      if( m_Name == "Slornite Thief" )
      {
         InariaEffect* tempanimation = new InariaEffect;

         Sprite* arrow = NULL;
         if( m_PosX == g_Player->m_PlayerPosX && m_PosY < g_Player->m_PlayerPosY ) //  Firing right
            arrow = g_Tiles[141];

         else if( m_PosX == g_Player->m_PlayerPosX && m_PosY > g_Player->m_PlayerPosY ) //  Firing left
            arrow = g_Tiles[137];

         else if( m_PosX < g_Player->m_PlayerPosX && m_PosY == g_Player->m_PlayerPosY ) //  Firing down
            arrow = g_Tiles[139];

         else if( m_PosX > g_Player->m_PlayerPosX && m_PosY == g_Player->m_PlayerPosY ) //  Firing up
            arrow = g_Tiles[143];

         else if( m_PosX < g_Player->m_PlayerPosX && m_PosY < g_Player->m_PlayerPosY ) //  Firing down-right
            arrow = g_Tiles[140];

         else if( m_PosX < g_Player->m_PlayerPosX && m_PosY > g_Player->m_PlayerPosY ) //  Firing up-right
            arrow = g_Tiles[138];

         else if( m_PosX > g_Player->m_PlayerPosX && m_PosY < g_Player->m_PlayerPosY ) //  Firing down-left
            arrow = g_Tiles[142];

         else //  Firing up-left
            arrow = g_Tiles[144];

         tempanimation->Init( arrow,
            g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize,
            g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize,
            g_Offset + (g_ViewRange * g_TileSize),
            g_Offset + (g_ViewRange * g_TileSize),

            LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true );

         g_AnimationList.push_back(tempanimation);
         AddConsoleString("Slornite Thief fires an arrow at you!");

         Attack();

      }

      else if( m_Name == "Slornite Mage" || m_Name == "The Evil One" || m_Name == "Mother Green Dragon" || m_Name == "Green Dragon" )
      {
         InariaEffect* tempanimation = new InariaEffect;

         tempanimation->Init( g_Star,
            g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
            g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true, 1, "", "", 255, 64, 64 );

         g_AnimationList.push_back(tempanimation);
         AddConsoleString(m_Name + " attacks you with a fireball!");
         Attack();
      }

      else if( m_Name == "Lightning Golem" )
      {
         InariaEffect* tempanimation = new InariaEffect;

         tempanimation->Init( g_Star,
            g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
            g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true, 1, "", "", 128, 128, 256 );

         g_AnimationList.push_back(tempanimation);
         AddConsoleString(m_Name + " throws lightning at you!");
         Attack();
      }

      else if( m_Name == "Mendikar the MadMaker" )
      {
         InariaEffect* tempanimation = new InariaEffect;

         tempanimation->Init( g_Star,
            g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
            g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true, 1, "", "", 255, 64, 255 );


         g_AnimationList.push_back(tempanimation);
         AddConsoleString(m_Name + " attacks you with a madness bolt!");
         Attack();
      }

      else if( m_Name == "Gazer" )
      {
         InariaEffect* tempanimation = new InariaEffect;

         tempanimation->Init( g_Star,
            g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
            g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            g_Offset + (g_ViewRange * g_TileSize) + 4,
            LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true, 1, "", "", 64, 64, 64 );


         g_AnimationList.push_back(tempanimation);
         AddConsoleString(m_Name + " attacks you with a death bolt!");
         Attack();
      }

      else if( m_Name == "Poisonous Froad" || m_Name == "Spitting Asp" || m_Name == "Carnivorous Mushroom" )
      {
         if( !m_HasUsedSpecialAbility )
         {
            InariaEffect* tempanimation = new InariaEffect;

            if( m_Name != "Carnivorous Mushroom")
            {
               tempanimation->Init( g_Star,
                  g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
                  g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                  g_Offset + (g_ViewRange * g_TileSize) + 4,
                  g_Offset + (g_ViewRange * g_TileSize) + 4,
                  LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true, 1, "", "", 64, 255, 64 );
            }
            else
            {
               tempanimation->Init( g_Star,
                  g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4,
                  g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                  g_Offset + (g_ViewRange * g_TileSize) + 4,
                  g_Offset + (g_ViewRange * g_TileSize) + 4,
                  LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, m_PosX, m_PosY) * 50, true, 1, "", "", 206, 101, 101 );
            }


            g_AnimationList.push_back(tempanimation);

            AddConsoleString(m_Name + " spits poison at you!");
            if( !DoRPGCheck(g_Player->GetWillWithBonus(), 20 - g_Player->GetWillWithBonus() ) )
            {
               tempanimation = new InariaEffect;
               int xoffset = g_RNG->Random(16);
               tempanimation->Init( g_Bubble,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + xoffset,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + 24,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + xoffset,
               g_Offset + (g_ViewRange * g_TileSize) + 4,
               500, false, 1, "", "", 64, 255, 64, 64 );

               g_AnimationList.push_back(tempanimation);


               xoffset = g_RNG->Random(16);
               tempanimation = new InariaEffect;
               tempanimation->Init( g_Bubble,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + xoffset,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + 24,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + xoffset,
               g_Offset + (g_ViewRange * g_TileSize) + 4,
               500, false, 1, "", "", 64, 255, 64, 64 );
               g_AnimationList.push_back(tempanimation);


               xoffset = g_RNG->Random(16);
               tempanimation = new InariaEffect;
               tempanimation->Init( g_Bubble,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + xoffset,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + 24,
               g_Offset + (g_ViewRange * g_TileSize) + 4 + xoffset,
               g_Offset + (g_ViewRange * g_TileSize) + 4,
               500, true, 1, "", "", 64, 255, 64, 64 );

               g_AnimationList.push_back(tempanimation);
               AddConsoleString("You have been poisoned!");

               g_Player->m_PoisonCounter += HalfNHalf(6);
            }
            else
            {
               AddConsoleString("You resist the poison.");
            }
            m_HasUsedSpecialAbility = true;
            m_AttackRange = 1.5;
         }
         else
         {
            Attack();
         }
      }

      else if( m_Name == "Slornite Cleric" )
      {
         if( !m_HasUsedSpecialAbility )
         {
            bool healedsomebody = false;
            //  Find a nearby enemy that needs healing.
            list<NPC*>::iterator node = g_Map->m_NPCList.begin();
            for( node; node != g_Map->m_NPCList.end(); ++node )
            {
               if( LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, (*node)->m_PosX, (*node)->m_PosY) < 5  )
               {
                  if( (*node)->m_Attitude == 0 )
                  {
                     if( (*node)->m_HitPoints < (*node)->m_MaxHitPoints )
                     {
                        int healing = HalfNHalf(m_Will);
                        stringstream writer;
                        writer.str("");
                        writer << "Slornite Cleric healed " << (*node)->m_Name << " for " << healing << "!";
                        AddConsoleString( writer.str(), 255, 64, 255 );
                        healedsomebody = true;

                        int xoffset = g_RNG->Random(16);
                        InariaEffect* tempanimation = new InariaEffect;
                        tempanimation->Init( g_Star,
                        g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4 + 24,
                        g_Offset + ((m_PosX - g_Player->m_PlayerPosX) + g_ViewRange) * g_TileSize + 4 + xoffset,
                        g_Offset + ((m_PosY - g_Player->m_PlayerPosY) + g_ViewRange) * g_TileSize + 4,
                        500, true, 1, "", "", 255, 255, 128, 64 );
                        g_AnimationList.push_back(tempanimation);

                        break;
                     }
                  }
               }
            }



            if( healedsomebody && g_RNG->Random(2) )
            {
               m_HasUsedSpecialAbility = true;
               m_AttackRange = 1.5;
            }
         }
         else
         {
            Attack();
         }
      }

      else if(m_Name == "Slornite King")
      {
         switch(rand() % 8)
         {
         case 0:
            AddConsoleString("Slornite King: \"I'll kill you dead!\"", 255, 64, 255);
            break;
         case 1:
            AddConsoleString("Slornite King: \"I smolder with generic rage!\"", 255, 64, 255);
            break;
         case 2:
            AddConsoleString("Slornite King: \"The only good you is a dead you!\""), 255, 64, 255;
            break;
         case 3:
            AddConsoleString("Slornite King: \"This is the end!\"", 255, 64, 255);
            break;
         case 4:
            AddConsoleString("Slornite King: \"Everything you know is wrong!\"", 255, 64, 255);
            break;
         case 5:
            AddConsoleString("Slornite King: \"You cannot win!  Surrender and die!\"", 255, 64, 255);
            break;
         case 6:
            AddConsoleString("Slornite King: \"Your head will look good on my wall!\"", 255, 64, 255);
            break;
         case 7:
            AddConsoleString("Slornite King: \"Don't you know?  Villains live forever!\"", 255, 64, 255);
            break;
         }
         Attack();
      }
      else
      {
         Attack();
      }
      return;
   }

   //  Otherwise, update the NPC's movement based on the AI code

   //  1 - wander
   else if(m_AI == 1)
   {
      if(m_MovedLastTurn == false && rand()%2 == 1)
      {
         int _deltaX = (rand() % 3) - 1;
         int _deltaY = (rand() % 3) - 1;

         int _targetX = m_PosX + _deltaX;
         int _targetY = m_PosY + _deltaY;

         //  Now vet the target and see if we can move in the specified direction.

         //  Are we going off the map?
         if(_targetX < 0 || _targetX > 63)
         {
            _targetX = m_PosX;
         }
         if(_targetY< 0 || _targetY > 63)
         {
            _targetY = m_PosY;
         }

         //  Are we walking into something solid?
         if(g_Map->m_map[_targetX][_targetY].m_passable == false)
         {
            _targetX = m_PosX;
            _targetY = m_PosY;
         }

         //  Are we walking into a person or monster?
         for(list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2)
         {
            if((*node2) != this)
            {
               if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY)
               {
                  _targetX = this->m_PosX;
                  _targetY = this->m_PosY;
               }
            }
         }

         for(list<Item*>::iterator node2 = g_Map->m_ItemList.begin(); node2 != g_Map->m_ItemList.end(); ++node2)
         {
            if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY && !(*node2)->m_Passable )
            {
               _targetX = this->m_PosX;
               _targetY = this->m_PosY;
            }
         }

         //  Are we walking into the player?
         if(g_Player->m_PlayerPosX  == _targetX && g_Player->m_PlayerPosY == _targetY)
         {
            _targetX = m_PosX;
            _targetY = m_PosY;
         }

         if(m_PosX != _targetX || m_PosY != _targetY)
         {
            m_MovedLastTurn = true;
         }
         else
         {
            m_MovedLastTurn = false;
         }

         m_PosX = _targetX;
         m_PosY = _targetY;
      }
      else
      {
         m_MovedLastTurn = false;
      }
   }

   //  2 - wander then pursue avatar
   if(m_AI == 2)
   {
      //  See how far we are from the avatar.  Ah, Pythagoras, how we adore you.
      int _distanceFromAvatar = int(pow((m_PosX - g_Player->m_PlayerPosX), 2.0f) + pow((m_PosY - g_Player->m_PlayerPosY), 2.0f));

      if(_distanceFromAvatar < 25)
      {
         int _deltaX = 0;
         int _deltaY = 0;

         if(g_Player->m_PlayerPosX > m_PosX)
         {
            _deltaX = 1;
         }
         else if(g_Player->m_PlayerPosX < m_PosX)
         {
            _deltaX = -1;
         }

         if(g_Player->m_PlayerPosY > m_PosY)
         {
            _deltaY = 1;
         }
         else if(g_Player->m_PlayerPosY < m_PosY)
         {
            _deltaY = -1;
         }

         int _targetX = m_PosX + _deltaX;
         int _targetY = m_PosY + _deltaY;

         //  Now vet the target and see if we can move in the specified direction.

         //  Are we going off the map?
         if(_targetX < 0 || _targetX > 63)
         {
            _targetX = m_PosX;
         }
         if(_targetY< 0 || _targetY > 63)
         {
            _targetY = m_PosY;
         }

         //  Are we walking into something solid?
         if(g_Map->m_map[_targetX][_targetY].m_passable == false)
         {
            _targetX = m_PosX;
            _targetY = m_PosY;
         }

         //  Are we walking into a person or monster?
         for(list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2)
         {
            if((*node2) != this)
            {
               if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY)
               {
                  _targetX = this->m_PosX;
                  _targetY = this->m_PosY;
               }
            }
         }

         //  Are we walking into an item marked unpassable?
         for(list<Item*>::iterator node2 = g_Map->m_ItemList.begin(); node2 != g_Map->m_ItemList.end(); ++node2)
         {
            if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY && !(*node2)->m_Passable )
            {
               _targetX = this->m_PosX;
               _targetY = this->m_PosY;
            }
         }

         //  Are we walking into the player?
         if(g_Player->m_PlayerPosX  == _targetX && g_Player->m_PlayerPosY == _targetY)
         {
            _targetX = m_PosX;
            _targetY = m_PosY;
         }

         m_PosX = _targetX;
         m_PosY = _targetY;
      }

      else if(m_MovedLastTurn == false && rand()%2 == 1)
      {
         int _deltaX = (rand() % 3) - 1;
         int _deltaY = (rand() % 3) - 1;

         int _targetX = m_PosX + _deltaX;
         int _targetY = m_PosY + _deltaY;

         //  Now vet the target and see if we can move in the specified direction.

         //  Are we going off the map?
         if(_targetX < 0 || _targetX > 63)
         {
            _targetX = m_PosX;
         }
         if(_targetY< 0 || _targetY > 63)
         {
            _targetY = m_PosY;
         }

         //  Are we walking into something solid?
         if(g_Map->m_map[_targetX][_targetY].m_passable == false)
         {
            _targetX = m_PosX;
            _targetY = m_PosY;
         }

         //  Are we walking into a person or monster?
         for(list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2)
         {
            if((*node2) != this)
            {
               if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY)
               {
                  _targetX = this->m_PosX;
                  _targetY = this->m_PosY;
               }
            }
         }

         //  Are we walking into an item marked unpassable?
         for(list<Item*>::iterator node2 = g_Map->m_ItemList.begin(); node2 != g_Map->m_ItemList.end(); ++node2)
         {
            if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY && !(*node2)->m_Passable )
            {
               _targetX = this->m_PosX;
               _targetY = this->m_PosY;
            }
         }

         //  Are we walking into the player?
         if(g_Player->m_PlayerPosX  == _targetX && g_Player->m_PlayerPosY == _targetY)
         {
            _targetX = m_PosX;
            _targetY = m_PosY;
         }

         if(m_PosX != _targetX || m_PosY != _targetY)
         {
            m_MovedLastTurn = true;
         }
         else
         {
            m_MovedLastTurn = false;
         }

         m_PosX = _targetX;
         m_PosY = _targetY;
      }
      else
      {
         m_MovedLastTurn = false;
      }
   }

   //  2 - Pursue Avatar from anywhere on the map.
   if(m_AI == 3)
   {
      //  See how far we are from the avatar.  Ah, Pythagoras, how we adore you.
      if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE )
      {
         int _distanceFromAvatar = LineLength(m_PosX, m_PosY, g_Player->m_PlayerPosX, g_Player->m_PlayerPosY);
         if(_distanceFromAvatar <= 7)
         {
            if( g_RNG->Random(10) == 0 )
            {
               AddConsoleString( m_Talks[0] );
            }
            int _deltaX = 0;
            int _deltaY = 0;

            if( g_Player->m_PlayerPosX > m_PosX)
            {
               _deltaX = 1;
            }
            else if( g_Player->m_PlayerPosX < m_PosX)
            {
               _deltaX = -1;
            }

            if( g_Player->m_PlayerPosY > m_PosY)
            {
               _deltaY = 1;
            }
            else if( g_Player->m_PlayerPosY < m_PosY)
            {
               _deltaY = -1;
            }

            int _targetX = m_PosX + _deltaX;
            int _targetY = m_PosY + _deltaY;

            //  Now vet the target and see if we can move in the specified direction.

            //  Are we going off the map?
            if(_targetX < 0 || _targetX > 63)
            {
               _targetX = m_PosX;
            }
            if(_targetY< 0 || _targetY > 63)
            {
               _targetY = m_PosY;
            }

            //  Are we walking into something solid?
            if(g_Map->m_map[_targetX][_targetY].m_passable == false)
            {
               _targetX = m_PosX;
               _targetY = m_PosY;
            }

            //  Are we walking into a person or monster?
            for(list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2)
            {
               if((*node2) != this)
               {
                  if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY)
                  {
                     _targetX = this->m_PosX;
                     _targetY = this->m_PosY;
                  }
               }
            }

            //  Are we walking into an item marked unpassable?
            for(list<Item*>::iterator node2 = g_Map->m_ItemList.begin(); node2 != g_Map->m_ItemList.end(); ++node2)
            {
               if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY && !(*node2)->m_Passable )
               {
                  _targetX = this->m_PosX;
                  _targetY = this->m_PosY;
               }
            }

            //  Are we walking into the player?
            if(g_Player->m_PlayerPosX  == _targetX && g_Player->m_PlayerPosY == _targetY)
            {
               _targetX = m_PosX;
               _targetY = m_PosY;
            }

            if( m_PosX != _targetX || m_PosY != _targetY )
            {
               m_PosX = _targetX;
               m_PosY = _targetY;
            }
         }
         else
         {
            m_MovedLastTurn = false;
         }
      }

      else if( g_StateMachine->GetCurrentState() == STATE_TITLESTATE )
      {
         for( list<NPC*>::iterator node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
         {
            if( (*node)->m_Attitude != this->m_Attitude )
            {
               int _distanceFromAvatar = int(pow((m_PosX - (*node)->m_PosX), 2.0f) + pow((m_PosY - (*node)->m_PosY), 2.0f));

               if(_distanceFromAvatar < 49)
               {
                  if( g_RNG->Random(10) == 0 )
                  {
//                     AddConsoleString( m_Talks[0] );
                  }
                  int _deltaX = 0;
                  int _deltaY = 0;

                  if( (*node)->m_PosX > m_PosX)
                  {
                     _deltaX = 1;
                  }
                  else if( (*node)->m_PosX < m_PosX)
                  {
                     _deltaX = -1;
                  }

                  if( (*node)->m_PosY > m_PosY)
                  {
                     _deltaY = 1;
                  }
                  else if( (*node)->m_PosY < m_PosY)
                  {
                     _deltaY = -1;
                  }

                  int _targetX = m_PosX + _deltaX;
                  int _targetY = m_PosY + _deltaY;

                  //  Now vet the target and see if we can move in the specified direction.

                  //  Are we going off the map?
                  if(_targetX < 0 || _targetX > 63)
                  {
                     _targetX = m_PosX;
                  }
                  if(_targetY< 0 || _targetY > 63)
                  {
                     _targetY = m_PosY;
                  }

                  //  Are we walking into something solid?
                  if(g_Map->m_map[_targetX][_targetY].m_passable == false)
                  {
                     _targetX = m_PosX;
                     _targetY = m_PosY;
                  }

                  //  Are we walking into a person or monster?
                  for(list<NPC*>::iterator node2 = g_Map->m_NPCList.begin(); node2 != g_Map->m_NPCList.end(); ++node2)
                  {
                     if((*node2) != this)
                     {
                        if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY)
                        {
                           _targetX = this->m_PosX;
                           _targetY = this->m_PosY;
                        }
                     }
                  }

                  //  Are we walking into an item marked unpassable?
                  for(list<Item*>::iterator node2 = g_Map->m_ItemList.begin(); node2 != g_Map->m_ItemList.end(); ++node2)
                  {
                     if(_targetX == (*node2)->m_PosX && _targetY == (*node2)->m_PosY && !(*node2)->m_Passable )
                     {
                        _targetX = this->m_PosX;
                        _targetY = this->m_PosY;
                     }
                  }

                  if( m_PosX != _targetX || m_PosY != _targetY )
                  {
                     m_PosX = _targetX;
                     m_PosY = _targetY;
                     break;
                  }
               }
               else
               {
                  m_MovedLastTurn = false;
               }
            }
         }
      }
   }
}

bool NPC::IsTurnDone()
{
   return true;
}

void NPC::Draw()
{
   if( g_StateMachine->GetCurrentState() == STATE_EDITORSTATE )
   {
      if( m_PosX >= 0 && m_PosY < g_AllMaps[g_CurrentEditorMap]->m_Width && m_PosX >= g_Player->m_PlayerPosX - g_ViewRange && m_PosX <= g_Player->m_PlayerPosX + g_ViewRange
         && m_PosY >= 0 && m_PosY < g_AllMaps[g_CurrentEditorMap]->m_Height && m_PosY >=  g_Player->m_PlayerPosY - g_ViewRange && m_PosY <= g_Player->m_PlayerPosY + g_ViewRange)
      {
         int adjustedx = m_PosX - (g_Player->m_PlayerPosX - ( g_ViewRange * 2 ));
         int adjustedy = m_PosY - (g_Player->m_PlayerPosY - ( g_ViewRange * 2 ));
         DrawSpriteResizedAt( g_Tiles[m_Tile], g_Offset + (adjustedx * ( g_TileSize / 2)) - (g_ViewRange * (g_TileSize / 2)), g_Offset + (adjustedy * ( g_TileSize / 2))  - (g_ViewRange * (g_TileSize / 2)), ( g_TileSize / 2), ( g_TileSize / 2) );
      }
      return;
   }

   if(m_PosX >= g_Player->m_PlayerPosX - g_ViewRange && m_PosX <= g_Player->m_PlayerPosX + g_ViewRange
      && m_PosY >=  g_Player->m_PlayerPosY - g_ViewRange && m_PosY <= g_Player->m_PlayerPosY + g_ViewRange)
   {
//      if( static_cast<uint32_t>(g_Engine->GameTimeInMS()) % 1000 > 500 && g_DanceParty && g_Map->m_MapTitle == "Inaria Special" ) 
//         g_Display->BlitImageReversed(g_Tiles[m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize);
//      else
         DrawSpriteAt(g_Tiles[m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize );

/*#ifdef _DEBUG
      int basex = g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize;
      int basey = g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize;
      DrawImageRectAt(g_Mask, 0, 0, 32, 32, basex, basey ); 


      stringstream temp;

      temp.str("");
      temp << "s:" << m_Strength;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 1, basey + 1, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex, basey);

      temp.str("");
      temp << "d:" << m_Dexterity;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 1, basey + 9, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex, basey + 8);

      temp.str("");
      temp << "i:" << m_Intelligence;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 1, basey + 17, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex, basey + 16);

      temp.str("");
      temp << "e:" << m_Endurance;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 1, basey + 24, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex, basey + 24);


      temp.str("");
      temp << "w:" << m_Will;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 17, basey + 1, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 16, basey );

      temp.str("");
      temp << "h:" << m_MaxHitPoints;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 17, basey + 9, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 16, basey + 8 );

      temp.str("");
      temp << "c:" << m_HitPoints;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 17, basey + 17, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 16, basey + 16);

      temp.str("");
      temp << "a:" << m_Armor;
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 17, basey + 25, 0, 0, 0);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, temp.str().c_str(), basex + 16, basey  + 24);


#endif*/
   }
}

void NPC::OnTalk()
{
   if( m_Name == "Emerald Dancing Girl" && m_CurrentTalk == 0 )
   {
      g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_DANCEPARTY]);
      g_DanceParty = true;
   }
   else if( m_Name == "Emerald Dancing Girl" )
   {
      g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_WIN]);
      g_DanceParty = false;
   }
   else if ( g_Player->m_KilledEvilOne &&
      m_Name != "Emerald Merchant" &&
      m_Name != "Ruby Merchant" &&
      m_Name != "Sapphire Merchant" &&
      m_Name != "Triumphant Inarian King" &&
      m_Name != "Emerald Healer" &&
      m_Name != "Baby Dragon" &&
      m_Name != "Firebird" &&
      m_Name != "Oubliette Insane Man" &&
      m_Name != "Oubliette Palsied Woman")
   {
      AddConsoleString( m_Name + ": \"" + g_HappyResponses[g_RNG->Random(30)] + "\"" );
      return;
   }


   AddConsoleString(m_Talks[m_CurrentTalk], 255, 255, 64);
   ++m_CurrentTalk;
   if(m_CurrentTalk >= m_NumberOfTalks)
      m_CurrentTalk = 0;
}

void NPC::OnAttack()
{

}

bool NPC::Attack()
{
   //  Put the HURTING on him!
   bool didwehit = false;
   int damage = 0;

   if( m_Name == "Slornite Mage" || m_Name == "Spitting Asp" || m_Name == "Poisonous Froad" )
   {
      didwehit = DoRPGCheck( m_Intelligence + m_Will, g_Player->GetWillWithBonus() );
      damage = HalfNHalf( m_Will );
   }
   else
   {
      didwehit = DoRPGCheck( m_Dexterity + m_Strength, g_Player->GetDexterityWithBonus() );
      damage = HalfNHalf( m_Strength );
   }

   if( didwehit )
   {
      if( damage <= 0 )
         damage = 1;

      damage -= g_RNG->Random(g_Player->GetArmor());
      if( damage <= 0 )
      {
         string _tempString = m_Name + " missed you!";
         AddConsoleString(_tempString);
         InariaEffect* tempanimation = new InariaEffect;
         tempanimation->Init( NULL, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, 250, true, 0, "Sounds/miss1.wav" );
         g_AnimationList.push_back(tempanimation);
      }
      else
         g_Player->DoDamage(m_Name, damage);
   }
   else
   {
      string _tempString = m_Name + " missed you!";
      AddConsoleString(_tempString);
      InariaEffect* tempanimation = new InariaEffect;
      tempanimation->Init( NULL, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, 250, true, 0, "Sounds/miss1.wav" );
      g_AnimationList.push_back(tempanimation);

   }

   return true;
}

bool NPC::Attack( NPC* target )
{
   //  Put the HURTING on him!
   if( DoRPGCheck( m_Strength, target->m_Dexterity ) )
   {
      int damage = 1 + g_RNG->Random( this->m_Strength );
      if( damage <= 0 )
         damage = 1;

      damage -= target->m_Armor;
      if( damage >= 1 )
         target->DoDamage(damage, false, this);
      else
      {  
         char temp[64];
         sprintf(temp, "%s misses %s!", this->m_Name.c_str(), target->m_Name.c_str() );
//         AddConsoleString(string(temp));
         InariaEffect* tempanimation = new InariaEffect;
         tempanimation->Init( NULL, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, 250, true, 0 );
         g_AnimationList.push_back(tempanimation);

      }

   }
   else
   {
      char temp[64];
      sprintf(temp, "%s misses %s!", this->m_Name.c_str(), target->m_Name.c_str() );
//      AddConsoleString(string(temp));
      InariaEffect* tempanimation = new InariaEffect;
      tempanimation->Init( NULL, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, 250, true, 0 );
      g_AnimationList.push_back(tempanimation);

   }
   return true;
}

void NPC::OnLook()
{
   AddConsoleString(m_LookResponse);
}

void NPC::OnSmite()
{

}

void NPC::SetPos(int x, int y)
{
   m_PosX = x;
   m_PosY = y;
}

void NPC::DoDamage(int damage, bool critical, NPC* hitter )
{
   if( m_IsDead )
      return; //  We already died this turn, no need to do it again.

   if (!hitter )
   {
      if(m_PosX >= g_Player->m_PlayerPosX - g_ViewRange && m_PosX <= g_Player->m_PlayerPosX + g_ViewRange
         && m_PosY >=  g_Player->m_PlayerPosY - g_ViewRange && m_PosY <= g_Player->m_PlayerPosY + g_ViewRange)
      {
         InariaEffect* tempanimation = new InariaEffect;
         tempanimation->Init( g_Tiles[this->m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, 250, false );
         g_AnimationList.push_back(tempanimation);
      }

      if(m_PosX >= g_Player->m_PlayerPosX - g_ViewRange && m_PosX <= g_Player->m_PlayerPosX + g_ViewRange
         && m_PosY >=  g_Player->m_PlayerPosY - g_ViewRange && m_PosY <= g_Player->m_PlayerPosY + g_ViewRange)
      {
         InariaEffect* tempanimation = new InariaEffect;
         tempanimation->Init(g_Tiles[37], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize - 8, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize - 8, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize + 8, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize + 8, 50 );
         g_AnimationList.push_back(tempanimation);
      }


      char _tempString[64];
      int _actualDamage = damage - g_RNG->Random(m_Armor);
      if (_actualDamage < 0) _actualDamage = 0;
      m_HitPoints -= _actualDamage;
      if( critical )
         sprintf(_tempString, "You critically hit the %s for %d points!", m_Name.c_str(), _actualDamage);
      else
         sprintf(_tempString, "You hit the %s for %d points.", m_Name.c_str(), _actualDamage);

      AddConsoleString(_tempString);
      if(m_HitPoints <= 0)
      {
         if( m_Name == "Baby Dragon" )
         {
            g_Player->m_KilledWhelp = true; //  What an idiot.
         }

         if( m_Name == "The Evil One" )
         {
            g_Player->m_KilledEvilOne = true; //  What a hero!

            //  Have him say his last lines
            AddConsoleString( m_Talks[23], 255, 64, 255 );
            AddConsoleString( m_Talks[24], 255, 64, 255 );
            AddConsoleString( "A secret passage opens!", 255, 255, 64 );

            //  We now have to make a lot of changes to the map to take the player to the end.
            g_Map->m_ItemList.clear(); //  Get rid of the transformation field
            g_Map->m_MapLinks.clear(); //  Can't go back.
            g_Map->m_map[23][30].m_TerrainType = 14;
            MapLink* link = new MapLink;
            link->m_DestinationMap = "Maps/intro.map";
            link->m_XCoord = 23;
            link->m_YCoord = 30;
            link->m_XDest = 52;
            link->m_YDest = 13;

            g_Map->m_MapLinks.push_back(link);

         }

         if( m_Name == "Mendikar the MadMaker" )
         {
            g_Player->m_KilledMendikar = true;
            //  Find the palsied woman.
            for( list<NPC*>::iterator node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
            {
               if( (*node)->m_Name == "Oubliette Palsied Woman" )
               {
                  AddConsoleString( (*node)->m_Talks[10], 255, 255, 64 );
                  (*node)->m_AI = 0;
                  (*node)->m_Attitude = 1;
                  (*node)->m_CurrentTalk = 11;
               }
            }
         }

         sprintf(_tempString, "You kill %s!", m_Name.c_str());
         PlaySoundGroup("Sounds/Kill", 5, ".wav");
         AddConsoleString(_tempString);

         int _gainedXP = (this->m_Strength + this->m_Dexterity + this->m_Endurance + this->m_Intelligence + this->m_Will) - 5; 
         sprintf(_tempString, "You gain %d XP!", _gainedXP);
         AddConsoleString(_tempString);
         g_Player->m_CurrentXP += _gainedXP;

         int _gainedCoins = g_RNG->Random( _gainedXP / 2 ) + 1;
         sprintf(_tempString, "You gain %d Viridian Coins!", _gainedCoins);
         AddConsoleString(_tempString);
         g_Player->m_CurrentGold += _gainedCoins;

         m_IsDead = true;
         if(m_Name == "Slornite King")
         {
            AddConsoleString("Slornite King: \"Oh, man, are those MY guts on the floor?  Dammit!\"");
         }
      }
      else
      {
         //  Hit 'em, didn't kill 'em.
         PlaySoundGroup("Sounds/Hit", 5, ".wav");
      }
   }
   else
   {
      if(m_PosX >= hitter->m_PosX - g_ViewRange && m_PosX <= hitter->m_PosX + g_ViewRange
         && m_PosY >=  hitter->m_PosY - g_ViewRange && m_PosY <= hitter->m_PosY + g_ViewRange)
      {
         InariaEffect* tempanimation = new InariaEffect;
         tempanimation->Init( g_Tiles[this->m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize, 400, false );
         g_AnimationList.push_back(tempanimation);
      }

      if(m_PosX >= hitter->m_PosX - g_ViewRange && m_PosX <= hitter->m_PosX + g_ViewRange
         && m_PosY >=  hitter->m_PosY - g_ViewRange && m_PosY <= hitter->m_PosY + g_ViewRange)
      {
         InariaEffect* tempanimation = new InariaEffect;
         tempanimation->Init(g_Tiles[37], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize - 8, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize - 8, g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize + 8, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize + 8, 50 );
         g_AnimationList.push_back(tempanimation);
      }


      char _tempString[64];
      int _actualDamage = damage - m_Armor;
      if (_actualDamage < 0) _actualDamage = 0;
      m_HitPoints -= _actualDamage;
      if( critical )
         sprintf(_tempString, "%s critically hits the %s for %d points!", hitter->m_Name.c_str(), m_Name.c_str(), _actualDamage);
      else
         sprintf(_tempString, "%s hits the %s for %d points.", hitter->m_Name.c_str(), m_Name.c_str(), _actualDamage);

//      AddConsoleString(_tempString);
      if(m_HitPoints <= 0)
      {
         sprintf(_tempString, "%s kills the %s!", hitter->m_Name.c_str(), m_Name.c_str());
//         PlaySoundGroup("Sounds/Kill", 5, ".wav");
//         AddConsoleString(_tempString);
         m_IsDead = true;
         if(m_Name == "Slornite King")
         {
            AddConsoleString("Slornite King: \"You cannot truly defeat me!\"", 255, 128, 255);
            AddConsoleString("Slornite King teleports away!");
         }
      }
      else
      {
         //  Hit 'em, didn't kill 'em.
//         g_SoundSystem->PlaySound("Sounds/Hit1.wav");
      }
   }
}

string NPC::GetName()
{
   return m_Name;
}

int NPC::GetDamage()
{
   return m_Strength;
}

void NPC::DrawBark( std::string text )
{
   DrawParagraphAt(g_font.get(), g_fontSize, text, (5 + (abs(m_PosX) - abs(g_Player->m_PlayerPosX))) * 32, (4 +  (abs(m_PosY) - abs(g_Player->m_PlayerPosY))) * 32, 250, 16 );


/*	int _TextWidth = GetStringMetrics(g_font.get(), g_fontSize,  text );

	int x = m_PosX;
	int y = m_PosY;

	if( x + 16 + _TextWidth > 400 )
	{
		x = ( 400 - _TextWidth ) - 16;
	}

	for(int i = 0; i < _TextWidth; ++i)
	{
		DrawImageAt(g_TextBackground, x + i, y - 8);
		DrawImageAt(g_TextBackground, x + i, y + 8);
	}

	for(int i = 0; i < 8; ++i)
	{
		DrawImageAt(g_TextBackground, x - 8 + i,          y);
		DrawImageAt(g_TextBackground, x + _TextWidth + i, y);
	}


	DrawImageAt(g_TBUL, x - 8,          y -  8 );
	DrawImageAt(g_TBUR, x + _TextWidth, y -  8 );
	DrawImageAt(g_TBLL, x - 8,          y + 16 );
	DrawImageAt(g_TBLR, x + _TextWidth, y + 16 );

/*	if((*node).m_Attribute5 == 0)
		gp_Display->BlitImage(g_TalkSpurLeft, x + (_TextWidth * .25) - 16, y + 24, r2, g2, b2);
	else if((*node).m_Attribute5 == 1)
		gp_Display->BlitImage(g_TalkSpurRight, x + (_TextWidth * .75) - 16, y + 24, r2, g2, b2);
	else if((*node).m_Attribute5 == 2)
		gp_Display->BlitImage(g_TalkSpurRightUp, x + (_TextWidth * .75) - 16, y - 16, r2, g2, b2);*/

//	DrawTextAt(g_smallFont.get(), g_smallFontSize, text, x, y, 255, 255, 255, 255);
}

/*		else if( (*node).m_Event == GE_BARKUNIT )
		{
			PlanitiaUnit* _Unit = GetPointerFromID( (*node).m_UnitID );

			if(_Unit)
			{
				float _ScreenX, _ScreenY;
				_Unit->GetScreenPosition(_ScreenX, _ScreenY);
				//                 _ScreenX = _ScreenX + 48 + ((_Unit->m_Texture->m_Width / 2));// * _Unit->m_ScalingMatrix._11);
				float _OriginalX = _ScreenX; 
				//                 if(_OriginalX < 8) _OriginalX = 8;
				//                 if(_OriginalX > gp_Display->m_HRes - 16) _OriginalX = gp_Display->m_HRes - 16;


				int _TextWidth = 0;

				vector<string> _SplitStrings = g_Font.SplitStrings((*node).m_String, (gp_Display->m_HRes * .60) - 16);

				for(vector<string>::iterator node = _SplitStrings.begin(); node != _SplitStrings.end(); ++node)
				{
					int _ThisWidth = g_Font.GetStringMetrics(*node);
					if(_ThisWidth > _TextWidth)
						_TextWidth = _ThisWidth;
				}

				_ScreenX -= _TextWidth / 2;

				if(_ScreenX < 8) _ScreenX = 8;
				if(_ScreenX + _TextWidth + 8 > gp_Display->m_HRes) _ScreenX = gp_Display->m_HRes - _TextWidth - 8;

				_ScreenY -= 64;

				for(int j = 0; j < _SplitStrings.size(); ++j)
				{
					for(int i = -8; i < _TextWidth + 8; ++i)
					{
						gp_Display->BlitImage(m_TextBackground, _ScreenX + i, _ScreenY - 8 - ((_SplitStrings.size() - 1 - j) * g_Font.m_FontHeight));
						gp_Display->BlitImage(m_TextBackground, _ScreenX + i, _ScreenY + 8 - ((_SplitStrings.size() - 1 - j) * g_Font.m_FontHeight));
					}
				}
				for(int j = 0; j < _SplitStrings.size(); ++j)
				{
					for(int i = 0; i < 8; ++i)
					{
						gp_Display->BlitImage(m_TextBackground, _ScreenX - 8 + i, _ScreenY - ((_SplitStrings.size() - 1 - j) * g_Font.m_FontHeight));
						gp_Display->BlitImage(m_TextBackground, _ScreenX + _TextWidth + i, _ScreenY - ((_SplitStrings.size() - 1 - j) * g_Font.m_FontHeight));
					}
				}


				gp_Display->BlitImage(m_TBUL, _ScreenX - 8, _ScreenY - 8);
				gp_Display->BlitImage(m_TBUR, _ScreenX + _TextWidth, _ScreenY - 8);
				gp_Display->BlitImage(m_TBLL, _ScreenX - 8, _ScreenY + 16);
				gp_Display->BlitImage(m_TBLR, _ScreenX + _TextWidth, _ScreenY + 16);

				if(_OriginalX < (gp_Display->m_HRes / 2))
					gp_Display->BlitImage(m_TalkSpurLeft, _OriginalX, _ScreenY + 24);
				else
					gp_Display->BlitImage(m_TalkSpurRight, _OriginalX, _ScreenY + 24);

				for(int i = 0; i < _SplitStrings.size(); ++i)
				{
					g_SmallFont.DrawText(_SplitStrings[i],_ScreenX, _ScreenY + ((16 - g_Font.m_FontHeight) / 2) - ((_SplitStrings.size() - 1 - i) * g_Font.m_FontHeight), 0, 0, 0);
				}
			}

			if( g_LocalUpdateCounter > (*node).m_Attribute1 + (*node).m_UpdateIndex )
			{
				node = m_CurrentBarks.erase(node);
			}
			else
			{
				++ node;
			}
		}
	}
}*/
