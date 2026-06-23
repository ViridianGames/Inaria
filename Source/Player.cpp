#include "GameGlobals.h"
#include <fstream>
#include <sstream>

using namespace std;

Player::~Player()
{
   Shutdown();
}

void Player::Init(const std::string& configfile)
{
   short frumple;


   m_PlayerPosX = 50;
   m_PlayerPosY = 34;
   
   m_CurrentXP = 0;
   m_NextLevel = 50;

/*#ifdef _DEBUG
   m_Level = 20;

   m_Strength     = 12;
   m_Dexterity    = 12;
   m_Intelligence = 12;
   m_Will         = 12;
   m_Endurance    = 12;
#else*/

   m_Level = 1;

   m_StatPoints = 0;

   m_Strength     = 2;
   m_Dexterity    = 2;
   m_Intelligence = 2;
   m_Will         = 2;
   m_Endurance    = 2;
//#endif

   m_Armor = 0;
   m_Name = "Viridian";
   m_CurrentWeaponType = NULL;
   m_CurrentArmorType = NULL;
   m_CurrentTrinketType = NULL;
   m_CurrentGold = 40;
   m_IsDead = false;
   
   RecalculateHitpointsAndMana();
   m_CurrentHitPoints = m_MaxHitPoints;
   m_CurrentManaPoints = m_MaxManaPoints;

   m_PoisonCounter = 0;
   m_SleepCounter = 0;

   m_PlayerInventory.clear();

   m_IsPerceptive = false;

   m_KilledWhelp = false;
   m_KilledSlornKing = false;
   m_AcceptedStupidOffer = false;
   m_KilledEvilOne = false;
   m_KilledMendikar = false;
   m_GotSolsSword = false;
   m_GotViridianArmor = false;
   m_PaidByInarianKing = false;
   m_ShowTutorials = true;

   m_PlayerSprite = 46;

   for(int i = 0; i < 16; ++ i)
   {
      g_SpecialAbilities[i].m_Active = false;
      g_SpecialAbilities[i].m_Duration = 0;
      g_SpecialAbilities[i].m_Level = 0;
   }

   g_SpecialAbilities[0].m_Active = true;
}

void Player::Shutdown()
{

}

void Player::Update()
{
   //  Check if the player is currently on a map link.
   g_Map->CheckMapLinks();

   // Level up!
   if(m_CurrentXP >= m_NextLevel )
   {
      m_Level++;
      m_NextLevel += 75 * (m_Level - 1);
      m_StatPoints += m_Level;

      AddConsoleString( "Level up!", 64, 255, 64 );
      g_SoundSystem->PlaySound("Sounds/level_up.wav");
      RecalculateHitpointsAndMana();
      stringstream temp;
      temp.str("");
      temp << "You have " << m_StatPoints << " skill points!  Press the Level Up button to improve your character.";
      AddConsoleString(temp.str(), 64, 255, 64);
   }
}

void Player::Draw()
{
   int playerLocation = g_TileSize * g_ViewRange + g_Offset;

   if( !g_Map->m_Peer )
      DrawSpriteAt(g_Tiles[m_PlayerSprite], playerLocation, playerLocation);

}

void Player::DoDamage(string _NPCName, int _damage)
{
   if( m_IsDead )  //  No sense doing this if we're ALREADY DEAD!  SOB!
      return;

   int playerlocation = g_TileSize * g_ViewRange + g_Offset;
   InariaEffect* tempanimation = new InariaEffect;
   tempanimation->Init( g_Tiles[37], playerlocation - 16, playerlocation - 16, playerlocation + 16, playerlocation + 16, 100, true, 0, "Sounds/Hit1.wav" );
   g_AnimationList.push_back(tempanimation);

   char _tempString[64];
   float _startingpercent = (float)m_CurrentHitPoints / (float)m_MaxHitPoints;
   m_CurrentHitPoints -= _damage;
   sprintf(_tempString, "The %s hits you for %d points!", _NPCName.c_str(), _damage);
   AddConsoleString(_tempString);
   float _endingpercent = (float)m_CurrentHitPoints / (float)m_MaxHitPoints;

   if( _startingpercent > .33f && _endingpercent <= .33f )
      AddConsoleString("LOW HEALTH WARNING!", 255, 64, 64);

   if(m_CurrentHitPoints <= 0)
   {
      m_CurrentHitPoints = 0;
      AddConsoleString("You have been killed!");
      m_IsDead = true;
      StopCurrentMusic();
      g_CurrentMusic = -1;
      g_SoundSystem->PlaySound("Sounds/inaria-death_withfade.wav");
   }
}

int Player::GetStrengthBonus()
{
   int bonusSTR = 0;
   if( m_CurrentArmorType)
      bonusSTR += m_CurrentArmorType->m_STRBonus;
   if( m_CurrentWeaponType)
      bonusSTR += m_CurrentWeaponType->m_STRBonus;
   if( m_CurrentTrinketType)
      bonusSTR += m_CurrentTrinketType->m_STRBonus;

   return bonusSTR;
}

int Player::GetDexterityBonus()
{
   int bonusDEX = 0;
   if( m_CurrentArmorType)
      bonusDEX += m_CurrentArmorType->m_DEXBonus;
   if( m_CurrentWeaponType)
      bonusDEX += m_CurrentWeaponType->m_DEXBonus;
   if( m_CurrentTrinketType)
      bonusDEX += m_CurrentTrinketType->m_DEXBonus;

   return bonusDEX;

}

int Player::GetIntelligenceBonus()
{

   int bonusINT = 0;
   if( m_CurrentArmorType)
      bonusINT += m_CurrentArmorType->m_INTBonus;
   if( m_CurrentWeaponType)
      bonusINT += m_CurrentWeaponType->m_INTBonus;
   if( m_CurrentTrinketType)
      bonusINT += m_CurrentTrinketType->m_INTBonus;

   return bonusINT;

}

int Player::GetWillBonus()
{
      int bonusWIL = 0;
   if( m_CurrentArmorType)
      bonusWIL += m_CurrentArmorType->m_WILBonus;
   if( m_CurrentWeaponType)
      bonusWIL += m_CurrentWeaponType->m_WILBonus;
   if( m_CurrentTrinketType)
      bonusWIL += m_CurrentTrinketType->m_WILBonus;

   return bonusWIL;


}

int Player::GetEnduranceBonus()
{

      int bonusEND = 0;
   if( m_CurrentArmorType)
      bonusEND += m_CurrentArmorType->m_ENDBonus;
   if( m_CurrentWeaponType)
      bonusEND += m_CurrentWeaponType->m_ENDBonus;
   if( m_CurrentTrinketType)
      bonusEND += m_CurrentTrinketType->m_ENDBonus;

   return bonusEND;

}

int Player::GetStrengthWithBonus()
{
   int bonusSTR = m_Strength;
   if( m_CurrentArmorType)
      bonusSTR += m_CurrentArmorType->m_STRBonus;
   if( m_CurrentWeaponType)
      bonusSTR += m_CurrentWeaponType->m_STRBonus;
   if( m_CurrentTrinketType)
      bonusSTR += m_CurrentTrinketType->m_STRBonus;

   return bonusSTR;
}

int Player::GetDexterityWithBonus()
{
   int bonusDEX = m_Dexterity;
   if( m_CurrentArmorType)
      bonusDEX += m_CurrentArmorType->m_DEXBonus;
   if( m_CurrentWeaponType)
      bonusDEX += m_CurrentWeaponType->m_DEXBonus;
   if( m_CurrentTrinketType)
      bonusDEX += m_CurrentTrinketType->m_DEXBonus;

   return bonusDEX;

}

int Player::GetIntelligenceWithBonus()
{

   int bonusINT = m_Intelligence;
   if( m_CurrentArmorType)
      bonusINT += m_CurrentArmorType->m_INTBonus;
   if( m_CurrentWeaponType)
      bonusINT += m_CurrentWeaponType->m_INTBonus;
   if( m_CurrentTrinketType)
      bonusINT += m_CurrentTrinketType->m_INTBonus;

   return bonusINT;

}

int Player::GetWillWithBonus()
{
      int bonusWIL = m_Will;
   if( m_CurrentArmorType)
      bonusWIL += m_CurrentArmorType->m_WILBonus;
   if( m_CurrentWeaponType)
      bonusWIL += m_CurrentWeaponType->m_WILBonus;
   if( m_CurrentTrinketType)
      bonusWIL += m_CurrentTrinketType->m_WILBonus;

   return bonusWIL;


}

int Player::GetEnduranceWithBonus()
{

      int bonusEND = m_Endurance;
   if( m_CurrentArmorType)
      bonusEND += m_CurrentArmorType->m_ENDBonus;
   if( m_CurrentWeaponType)
      bonusEND += m_CurrentWeaponType->m_ENDBonus;
   if( m_CurrentTrinketType)
      bonusEND += m_CurrentTrinketType->m_ENDBonus;

   return bonusEND;

}

int Player::GetDamage()
{
   int damage = GetStrengthWithBonus();

   //  Damage from "Poison"
   if( g_SpecialAbilities[ABILITY_POISON].m_Active )
      damage += 2;

   if( m_CurrentWeaponType && m_CurrentWeaponType->m_DamageMultiplier  > 0 )
      damage += m_CurrentWeaponType->m_DamageMultiplier;
   if( m_CurrentArmorType && m_CurrentArmorType->m_DamageMultiplier > 0 )
      damage += m_CurrentArmorType->m_DamageMultiplier;
   if( m_CurrentTrinketType && m_CurrentTrinketType->m_DamageMultiplier > 0 )
      damage += m_CurrentTrinketType->m_DamageMultiplier;

   //  Damage from "Enchant Weapon"
   if( g_SpecialAbilities[ABILITY_DAMAGEBONUS].m_Duration > 0)
      damage += (damage / 2);

   return damage;
}

int Player::GetArmor()
{
   int armor = 0;
   
   //  Armor from "Tank"
   if( g_SpecialAbilities[ABILITY_TANK].m_Active )
      armor += 2;

   if( m_CurrentWeaponType )
      armor += m_CurrentWeaponType->m_ArmorBonus;
   if( m_CurrentArmorType  )
      armor += m_CurrentArmorType->m_ArmorBonus;
   if( m_CurrentTrinketType  )
      armor += m_CurrentTrinketType->m_ArmorBonus;

   //  Armor from "Magic Armor"
   if( g_SpecialAbilities[ABILITY_MAGICARMOR].m_Duration > 0 )
      armor += (armor / 2);

   return armor;
}

void Player::RecalculateHitpointsAndMana()
{
   //  Recalculate the player's hitpoints and mana points based on the new stats
   m_MaxHitPoints = 10 + ( g_SpecialAbilities[ABILITY_HARDY].m_Level * 10 ) + Factorial( GetEnduranceWithBonus() ) + Factorial( m_Level );
   m_MaxManaPoints = 10 + Factorial( GetWillWithBonus() )  + Factorial( m_Level );
   g_Player->m_CurrentHitPoints = g_Player->m_MaxHitPoints;
   g_Player->m_CurrentManaPoints = g_Player->m_MaxManaPoints;
}