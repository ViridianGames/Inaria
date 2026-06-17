
#include "GameGlobals.h"
#include <list>
#include <algorithm>

typedef BaseUnit parent;

using namespace std;

Item::Item()
{
   m_Name         = "";
   m_ItemType     = 0;
   m_Tile         = 0;
   m_LookResponse = "";

   //  Attributes for the various things items can do
   m_STRBonus = 0;
   m_INTBonus = 0;
   m_WILBonus = 0;
   m_ENDBonus = 0;
   m_DEXBonus = 0;
   
   m_DamageMultiplier = 0;
   m_ArmorBonus = 0;

   m_TargetX = 0; //  Teleport Target X
   m_TargetY = 0; //  Teleport Target Y

   m_Bashable = false; //  For bashable doors/walls
   m_Pickable = false;
   m_Dispellable = false;
   m_HasRandomStats = false;

   m_Equippable = 0;

   m_Passable = true;

   m_Pickupable = true;

   m_PickupHandled = false;
   m_DidPlayerSee = true;
}

Item::~Item()
{
   parent::Shutdown();
   Shutdown();
}

void Item::Init(const std::string& configfile)
{
   parent::Init(configfile);
   m_Name         = m_UnitConfig["name"].stringdata;
   m_ItemType     = m_UnitConfig["item"].numdata;
   m_Tile         = m_UnitConfig["tile"].numdata;
   m_LookResponse = m_UnitConfig["lookresponse"].stringdata;

   //  Attributes for the various things items can do
   m_STRBonus = m_UnitConfig["str"].numdata;
   m_INTBonus = m_UnitConfig["int"].numdata;
   m_WILBonus = m_UnitConfig["wil"].numdata;
   m_ENDBonus = m_UnitConfig["end"].numdata;
   m_DEXBonus = m_UnitConfig["dex"].numdata;
   
   m_DamageMultiplier = m_UnitConfig["damage"].numdata;
   m_ArmorBonus = m_UnitConfig["armor"].numdata;

   m_TargetX = m_UnitConfig["targetx"].numdata; //  Teleport Target X
   m_TargetY = m_UnitConfig["targety"].numdata; //  Teleport Target Y

   m_Bashable = m_UnitConfig["bashable"].numdata; //  For bashable doors/walls
   m_Pickable = m_UnitConfig["pickable"].numdata;
   m_Dispellable = m_UnitConfig["dispellable"].numdata;

   m_Passable = m_UnitConfig["passable"].numdata;

   if( g_StateMachine->GetCurrentState() == STATE_STORESTATE )
      m_HasRandomStats = false;
   else
      m_HasRandomStats = m_UnitConfig["hasrandomstats"].numdata;

   m_Equippable = m_UnitConfig["equippable"].numdata;

   m_Pickupable = m_UnitConfig["pickupable"].numdata;

   m_Stackable = false;
   m_CurrentNumber = 1;

   m_ConfigFile = configfile;

   m_Price = 0;

   //  If we have random stats, make some random stats!
   if( (m_HasRandomStats  && (rand() % 2) == 0) || m_ItemType == ITEM_RING || m_ItemType == ITEM_NECKLACE )
   {
      for(int i = 0; i < g_Map->m_MapLevel; ++i)
      {
         int stat = g_RNG->Random(7);
         switch(stat)
         {
         case 0:
            if(m_STRBonus < 5 )
            {
               ++m_STRBonus;
            }
            break;

         case 1:
            if(m_INTBonus < 5 )
            {
               ++m_INTBonus;
            }
            break;

         case 2:
            if(m_WILBonus < 5 )
            {
               ++m_WILBonus;
            }
            break;

         case 3:
            if(m_ENDBonus < 5 )
            {
               ++m_ENDBonus;
            }
            break;

         case 4:
            if(m_DEXBonus < 5 )
            {
               ++m_DEXBonus;
            }
            break;

         case 5:
            ++m_DamageMultiplier;
            break;

         case 6:
            ++m_ArmorBonus;
            break;
         }
      }
   }

   if( m_ItemType == ITEM_HEALTHPOTION || m_ItemType == ITEM_MANAPOTION )
   {
      m_Stackable = true;
      m_Price = 5 * g_Player->m_Level;
   }

   if( m_ItemType == ITEM_DAGGER || m_ItemType == ITEM_CLOTHARMOR || m_ItemType == ITEM_RING || m_ItemType == ITEM_NECKLACE)
   {
      m_Price = 5 * ( 1 + m_STRBonus + m_INTBonus + m_WILBonus + m_ENDBonus + m_DEXBonus );
   }

   if( m_ItemType == ITEM_BOW || m_ItemType == ITEM_LEATHERARMOR )
   {
      m_Price = 10 * ( 1 + m_STRBonus + m_INTBonus + m_WILBonus + m_ENDBonus + m_DEXBonus );
   }

   if( m_ItemType == ITEM_SWORD || m_ItemType == ITEM_CHAINARMOR )
   {
      m_Price = 15 * ( 1 + m_STRBonus + m_INTBonus + m_WILBonus + m_ENDBonus + m_DEXBonus );
   }

   if( m_ItemType == ITEM_WARHAMMER || m_ItemType == ITEM_PLATEARMOR || m_ItemType == ITEM_VIRIDIANARMOR || m_ItemType == ITEM_RIGHTEOUSSWORD )
   {
      m_Price = 20 * ( 1 + m_STRBonus + m_INTBonus + m_WILBonus + m_ENDBonus + m_DEXBonus );
   }

}

void Item::Shutdown()
{
   parent::Shutdown();

}

void Item::Update()
{
   parent::Update();

   m_DidPlayerSee = DoRPGCheck( g_Player->GetIntelligenceWithBonus(), 20 - g_Player->GetIntelligenceBonus() );

}

void Item::Draw()
{
   if( g_StateMachine->GetCurrentState() == STATE_EDITORSTATE )
   {
      if(m_PosX >= g_Player->m_PlayerPosX - g_ViewRange && m_PosX <= g_Player->m_PlayerPosX + g_ViewRange
         && m_PosY >=  g_Player->m_PlayerPosY - g_ViewRange && m_PosY <= g_Player->m_PlayerPosY + g_ViewRange )
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
      if( m_ItemType == ITEM_SLEEPFIELD || m_ItemType == ITEM_POISONFIELD || m_ItemType == ITEM_FIREFIELD || m_ItemType == ITEM_ELECTRICFIELD || m_ItemType == ITEM_TRANSFORMATIONFIELD )
      {
         DrawSpriteAt(g_Tiles[m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize,
            255, 255, 255, 255);
      }
      else if( m_ItemType == ITEM_STEPSWITCH )
      {
         //  If the player passes an INT check, draw the switch
         if( m_DidPlayerSee 
            || g_StateMachine->GetCurrentState() == STATE_EDITORSTATE )
         {
            DrawSpriteAt(g_Tiles[m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize);
         }
      }
      else if( m_ItemType == ITEM_SECRETDOOR )
      {
         //  If the player passes an INT check, draw the switch
         if( m_DidPlayerSee
            || g_StateMachine->GetCurrentState() == STATE_EDITORSTATE )
         {
            DrawSpriteAt(g_Tiles[m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize);
         }
         else
         {
            DrawSpriteAt(g_Tiles[4], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize);
         }

      }
      else
      {
         DrawSpriteAt(g_Tiles[m_Tile], g_Offset + (m_PosX - (g_Player->m_PlayerPosX - g_ViewRange)) * g_TileSize, g_Offset + (m_PosY - (g_Player->m_PlayerPosY - g_ViewRange)) * g_TileSize);
      }
   }
}

void Item::OnTalk()
{
   AddConsoleString("You cannot talk to that.");
}

void Item::OnLook()
{
   AddConsoleString(m_LookResponse);
}

void Item::SetPos(int x, int y)
{
   m_PosX = x;
   m_PosY = y;
}

void Item::OnUse()
{
   switch(m_Equippable)
   {
   case EQUIPPABLE_WEAPON:
      {
         //  Make sure we meet strength requirements
         if(m_Name == "Sword" && g_Player->m_Strength < 4)
         {
            AddConsoleString("Sword requires a Strength of 4 to equip.");
            return;
         }

         if(m_Name == "Bow" && g_Player->m_Dexterity < 5)
         {
            AddConsoleString("Bow requires a Dexterity of 5 to equip.");
            return;
         }

         if(m_Name == "Warhammer" && g_Player->m_Strength < 8)
         {
            AddConsoleString("Warhammer requires a Strength of 8 to equip.");
            return;
         }

         if( g_Player->m_CurrentWeaponType == NULL )
         {
            g_Player->m_CurrentWeaponType = this;
            string temp = this->m_Name + " equipped!";
            g_Player->m_PlayerInventory.erase( std::remove(g_Player->m_PlayerInventory.begin(), g_Player->m_PlayerInventory.end(), this), g_Player->m_PlayerInventory.end() );
            AddConsoleString(temp);
            g_Player->RecalculateHitpointsAndMana();
         }
         else
         {
            g_Player->m_PlayerInventory.push_back(g_Player->m_CurrentWeaponType);
            g_Player->m_CurrentWeaponType = this;
            string temp = this->m_Name + " equipped!";
            g_Player->m_PlayerInventory.erase( std::remove(g_Player->m_PlayerInventory.begin(), g_Player->m_PlayerInventory.end(), this), g_Player->m_PlayerInventory.end() );
            AddConsoleString(temp);
            g_Player->RecalculateHitpointsAndMana();
         }
      }
      break;

   case EQUIPPABLE_ARMOR:
      {
         //  Make sure we meet strength requirements.
         if(m_Name == "Leather Armor" && g_Player->m_Strength < 3)
         {
            AddConsoleString("Leather Armor requires a Strength of 3 to equip.");
            return;
         }

         if(m_Name == "Chain Armor" && g_Player->m_Strength < 5)
         {
            AddConsoleString("Chain Armor requires a Strength of 5 to equip.");
            return;
         }

         if(m_Name == "Plate Armor" && g_Player->m_Strength < 7)
         {
            AddConsoleString("Plate Armor requires a Strength of 7 to equip.");
            return;
         }

         if( g_Player->m_CurrentArmorType == NULL )
         {
            g_Player->m_CurrentArmorType = this;
            string temp = this->m_Name + " equipped!";
            g_Player->m_PlayerInventory.erase( std::remove(g_Player->m_PlayerInventory.begin(), g_Player->m_PlayerInventory.end(), this), g_Player->m_PlayerInventory.end() );
            AddConsoleString(temp);
            g_Player->RecalculateHitpointsAndMana();
         }
         else
         {
            g_Player->m_PlayerInventory.push_back(g_Player->m_CurrentArmorType);
            g_Player->m_CurrentArmorType = this;
            string temp = this->m_Name + " equipped!";
            g_Player->m_PlayerInventory.erase( std::remove(g_Player->m_PlayerInventory.begin(), g_Player->m_PlayerInventory.end(), this), g_Player->m_PlayerInventory.end() );
            AddConsoleString(temp);
            g_Player->RecalculateHitpointsAndMana();
         }
      }
      break;


   case EQUIPPABLE_TRINKET:
      {
         if( g_Player->m_CurrentTrinketType == NULL )
         {
            g_Player->m_CurrentTrinketType = this;
            string temp = this->m_Name + " equipped!";
            g_Player->m_PlayerInventory.erase( std::remove(g_Player->m_PlayerInventory.begin(), g_Player->m_PlayerInventory.end(), this), g_Player->m_PlayerInventory.end() );
            AddConsoleString(temp);
            g_Player->RecalculateHitpointsAndMana();
         }
         else
         {
            g_Player->m_PlayerInventory.push_back(g_Player->m_CurrentTrinketType);
            g_Player->m_CurrentTrinketType = this;
            string temp = this->m_Name + " equipped!";
            g_Player->m_PlayerInventory.erase( std::remove(g_Player->m_PlayerInventory.begin(), g_Player->m_PlayerInventory.end(), this), g_Player->m_PlayerInventory.end() );
            AddConsoleString(temp);
            g_Player->RecalculateHitpointsAndMana();
         }
      }
      break;
   }


   switch(m_ItemType)
   {
   case ITEM_HEALTHPOTION:  //  Health potion
      {
         float amounttoheal;
         if(g_Player->m_CurrentHitPoints < g_Player->m_MaxHitPoints || g_Player->m_PoisonCounter > 0)
         {
            if( g_Player->GetWillWithBonus() < 5 )
               amounttoheal = g_Player->m_MaxHitPoints * .15;
            else if( g_Player->GetWillWithBonus() < 9 )
               amounttoheal = g_Player->m_MaxHitPoints * .33;
            else
               amounttoheal = g_Player->m_MaxHitPoints * .50;
            if(amounttoheal < 1)
               amounttoheal = 1;
            char _tempString[64];
            snprintf(_tempString, 64, "The health potion heals you for %d.", int(amounttoheal));
            AddConsoleString(_tempString);
            g_Player->m_CurrentHitPoints += amounttoheal;
            if(g_Player->m_CurrentHitPoints > g_Player->m_MaxHitPoints)
               g_Player->m_CurrentHitPoints = g_Player->m_MaxHitPoints;

            if( g_Player->m_PoisonCounter > 0 )
            {
               AddConsoleString("The health potion cures your poison.");
               g_Player->m_PoisonCounter = 0;
            }

            if( m_CurrentNumber > 1 )
               --m_CurrentNumber;
            else
               m_IsDead = true;
         }
         else
         {
            AddConsoleString("Already at full health!");
         }
      }
      break;

   case ITEM_MANAPOTION:  //  Mana potion
      {
         float amounttoheal;
         if(g_Player->m_CurrentManaPoints < g_Player->m_MaxManaPoints)
         {
            if( g_Player->GetWillWithBonus() < 5 )
               amounttoheal = g_Player->m_MaxHitPoints * .15;
            else if( g_Player->GetWillWithBonus() < 9 )
               amounttoheal = g_Player->m_MaxHitPoints * .33;
            else
               amounttoheal = g_Player->m_MaxHitPoints * .50;
            if(amounttoheal < 1)
               amounttoheal = 1;
            g_Player->m_CurrentManaPoints += amounttoheal;
            char _tempString[64];
            snprintf(_tempString, 64, "The mana potion restores %d of your mana.", int(amounttoheal));
            AddConsoleString(_tempString);
            if(g_Player->m_CurrentManaPoints > g_Player->m_MaxManaPoints)
               g_Player->m_CurrentManaPoints = g_Player->m_MaxManaPoints;

            if( m_CurrentNumber > 1 )
               --m_CurrentNumber;
            else
               m_IsDead = true;
         }
         else
         {
            AddConsoleString("Already at full mana!");
         }
      }
      break;
   }
}

void Item::OnStand()
{
   if( m_ItemType == ITEM_STEPSWITCH )
   {
      list<Item*>::iterator node = g_Map->m_ItemList.begin();

      for( node; node != g_Map->m_ItemList.end(); )
      {
         if( this->m_TargetX == (*node)->m_PosX && this->m_TargetY == (*node)->m_PosY )
         {
            node = g_Map->m_ItemList.erase(node);
         }
         else
            ++node;
      }
   }

   if( m_ItemType == ITEM_POISONFIELD )
   {
      int adjustedwill = g_Player->GetWillWithBonus();
      if( g_SpecialAbilities[ABILITY_IRONWILL].m_Active )
         adjustedwill += 5;

      if(!DoRPGCheck(adjustedwill, 20 - adjustedwill) )
      {
         AddConsoleString("Poisoned!", 255, 128, 128);
         g_Player->m_PoisonCounter += 11;
      }
      else
      {
         AddConsoleString("You resist the poison field.");
      }

   }

   if( m_ItemType == ITEM_SLEEPFIELD )
   {
      int adjustedwill = g_Player->GetWillWithBonus();
      if( g_SpecialAbilities[ABILITY_IRONWILL].m_Active )
         adjustedwill += 5;

      if(!DoRPGCheck(adjustedwill, 20 - adjustedwill) )
      {
         AddConsoleString("You have fallen asleep!", 255, 128, 128);
         g_Player->m_SleepCounter += 4;
      }
      else
      {
         AddConsoleString("You resist the sleep field.");
      }
   }

   if( m_ItemType == ITEM_TRANSFORMATIONFIELD )
   {
      AddConsoleString("The Evil One: \"I love how that trick keeps working.  I have never gone wrong by tempting a human with power.\"");
      g_Player->m_PlayerSprite = 55;
      g_Player->m_IsDead = true;
      m_IsDead = true;
      AddConsoleString("Your game is over.");
   }
}

void Item::OnAttack()
{
   /*  ITEM_DOOR,
   ITEM_SHOOTABLESWITCH*/
   if( m_ItemType == ITEM_SHOOTSWITCH )
   {

      list<Item*>::iterator node = g_Map->m_ItemList.begin();

      for( node; node != g_Map->m_ItemList.end(); )
      {
         if( this->m_TargetX == (*node)->m_PosX && this->m_TargetY == (*node)->m_PosY )
         {
            node = g_Map->m_ItemList.erase(node);
         }
         else
            ++node;
      }


      g_SoundSystem->PlaySoundGroup("Sounds/Hit", 5, ".wav");
      m_IsDead = true;
   }
}

void Item::OnPickup()
{
   m_PickupHandled = false;
   //  Gold goes directly into your gold count rather than your inventory.
   if(m_ItemType == ITEM_COIN)
   {

      int value =  ( g_Map->m_MapLevel * 5 ) + ( rand() % (g_Map->m_MapLevel * 5) );
      g_Player->m_CurrentGold += value;
      char temp[64];
      sprintf(temp, "You find %d Viridian Coins.", value);
      AddConsoleString(temp);
      m_IsDead = true;
   }

   //  Chests cough up a random item when you open them.
   else if(m_ItemType == ITEM_CHEST)
   {
      Item *temp = new Item;
      int _randomItemType = rand() % 100;
      if(_randomItemType < 50)
      {
         //  Half the time it's just a normal item, the other half it's a special item.
         switch(rand() % 10)
         {
         case 0:
            temp->Init("Items/leatherarmor.cfg");
            break;

         case 1:
            temp->Init("Items/chainarmor.cfg");
            break;

         case 2:
            temp->Init("Items/dagger.cfg");
            break;

         case 3:
            temp->Init("Items/bow.cfg");
            break;

         case 4:
            temp->Init("Items/sword.cfg");
            break;

         case 5:
            temp->Init("Items/warhammer.cfg");
            break;

         case 6:
            temp->Init("Items/sword.cfg");
            break;

         case 7:
            temp->Init("Items/clotharmor.cfg");
            break;

         case 8:
            temp->Init("Items/ring.cfg");
            break;

         case 9:
            temp->Init("Items/necklace.cfg");
            break;
         }
      }
      else
      {
         temp->Init("Items/coin.cfg");
      }

      temp->m_PosX = this->m_PosX;
      temp->m_PosY = this->m_PosY;
      g_Map->m_ItemList.push_front(temp);
      m_IsDead = true;
   }
   else if(m_ItemType == ITEM_HEALTHPOTION)
   {
      bool stackfound = false;
      //  Find the appropriate stack in the player's inventory
      for(int i = 0; i < g_Player->m_PlayerInventory.size(); ++i)
      {
         if( g_Player->m_PlayerInventory[i]->m_ItemType == m_ItemType )
         {
            stackfound = true;
            if( g_Player->m_PlayerInventory[i]->m_CurrentNumber < 20 )
            {
               ++g_Player->m_PlayerInventory[i]->m_CurrentNumber;
               m_IsDead = true;
            }
            else
            {
               AddConsoleString("You cannot carry any more potions.");
               m_PickupHandled = true;
            }
         }
      }
   }
   else if(m_ItemType == ITEM_MANAPOTION)
   {
      bool stackfound = false;
      //  Find the appropriate stack in the player's inventory
      for(int i = 0; i < g_Player->m_PlayerInventory.size(); ++i)
      {
         if( g_Player->m_PlayerInventory[i]->m_ItemType == m_ItemType )
         {
            stackfound = true;
            if( g_Player->m_PlayerInventory[i]->m_CurrentNumber < 20 )
            {
               ++g_Player->m_PlayerInventory[i]->m_CurrentNumber;
               m_IsDead = true;
            }
            else
            {
               AddConsoleString("You cannot carry any more potions.");
               m_PickupHandled = true;
            }
         }
      }
   }
}

