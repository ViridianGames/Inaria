
#ifndef _NPC_H_
#define _NPC_H_

#include "BaseUnit.h"
#include "Geist/Config.h"
#include "Item.h"
#include <vector>

class NPC: public BaseUnit
{
public:
   NPC(){};
   virtual ~NPC();
   void OnTalk();
   void OnAttack();
   bool Attack(); //  Attack the Player
   bool Attack( NPC* target ); //  Attack another NPC

   void OnLook();
   void OnSmite();
   void SetPos(int x, int y);
   void DoDamage(int damage, bool critical = false, NPC* hitter = NULL );
   std::string GetName();
   int  GetDamage();

   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   void DrawBark( std::string text);

   bool IsTurnDone();

   std::string m_LookResponse;
   std::string m_Name;

   int m_Strength;
   int m_Dexterity;
   int m_Intelligence;
   int m_Will;
   int m_Endurance;

   int m_HitPoints;
   int m_MaxHitPoints;
   int m_ManaPoints;
   int m_MaxManaPoints;
   int m_Armor;

   bool m_Attitude;
   int m_AI;  //  0 static  1 wander
   bool m_MovedLastTurn;
   int m_NumberOfTalks;
   int m_CurrentTalk;
   std::vector<std::string> m_Talks;
   std::vector<Item*> m_Inventory;
   int m_Tile;

   int m_PosX;
   int m_PosY;

   int m_Type;

   float m_AttackRange;

   bool m_HasUsedSpecialAbility;

   bool m_Talking;
   DWORD  m_TalkCounter;
   
};

#endif