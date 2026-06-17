#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "BaseUnit.h"
#include "Geist/Object.h"
#include "Item.h"

class Player : public BaseUnit
{
public:
   Player(){};
   ~Player();

   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   void DoDamage(std::string _NPCName, int _damage);

   int GetStrengthBonus();
   int GetDexterityBonus();
   int GetIntelligenceBonus();
   int GetWillBonus();
   int GetEnduranceBonus();

   int GetStrengthWithBonus();
   int GetDexterityWithBonus();
   int GetIntelligenceWithBonus();
   int GetWillWithBonus();
   int GetEnduranceWithBonus();



   int GetDamage();
   int GetArmor();
   void RecalculateHitpointsAndMana();


   int m_PlayerPosX, m_PlayerPosY;

   int m_Strength;
   int m_Dexterity;
   int m_Intelligence;
   int m_Endurance;
   int m_Will;

   int m_CurrentHitPoints;
   int m_MaxHitPoints;
   int m_CurrentManaPoints;
   int m_MaxManaPoints;
   int m_Armor;
   int m_Level;
   int m_CurrentXP;
   int m_NextLevel;
   Item* m_CurrentWeaponType;
   Item* m_CurrentArmorType;
   Item* m_CurrentTrinketType;
   bool m_IsDead;
   int m_CurrentGold;
   std::string m_Name;

   std::vector<Item*> m_PlayerInventory;

   int m_PoisonCounter;
   int m_SleepCounter;

   bool m_IsPerceptive;

   bool m_KilledWhelp;
   bool m_KilledSlornKing;
   bool m_AcceptedStupidOffer;
   bool m_KilledEvilOne;
   bool m_KilledMendikar;
   bool m_GotSolsSword;
   bool m_GotViridianArmor;
   bool m_PaidByInarianKing;
   bool m_ShowTutorials;

   int m_PlayerSprite;

   int m_StatPoints;
};
#endif