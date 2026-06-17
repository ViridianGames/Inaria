
#ifndef _ITEM_H_
#define _ITEM_H_

#include "BaseUnit.h"

class Item: public BaseUnit
{
public:
   Item();
   virtual ~Item();
   void OnTalk();
   void OnLook();
   void SetPos(int x, int y);
   int GetItemType() { return m_ItemType; }
   virtual void OnUse();
   virtual void OnPickup();
   virtual void OnStand();
   virtual void OnAttack();



   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();

   std::string m_Name;
   std::string m_LookResponse;
   int m_ItemType;
   int m_PosX;
   int m_PosY;
   int m_Tile;

   //  Attributes for the various things items can do
   int m_STRBonus;
   int m_INTBonus;
   int m_WILBonus;
   int m_ENDBonus;
   int m_DEXBonus;
   
   float m_DamageMultiplier;
   int m_ArmorBonus;

   int m_TargetX; //  Teleport Target X
   int m_TargetY; //  Teleport Target Y

   bool m_Bashable; //  For bashable doors/walls
   bool m_Pickable;
   bool m_Dispellable;
   bool m_HasRandomStats; //  For items you can pick up that have random stats

   bool m_Passable;

   int m_Equippable;
   int m_Pickupable;

   int m_Price;
   int m_Stackable;
   int m_CurrentNumber;

   bool m_PickupHandled; //  Did the custom pickup handler handle this pickup?

   bool m_DidPlayerSee;


   std::string m_ConfigFile;
   
};


#endif