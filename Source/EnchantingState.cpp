#include "EnchantingState.h"
#include "GameGlobals.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  EnchantingState
////////////////////////////////////////////////////////////////////////////////

EnchantingState::~EnchantingState()
{

}

void EnchantingState::Init(const std::string& configfile)
{

}

void EnchantingState::Shutdown(){}

void EnchantingState::Update()
{
   DoPlayerInput();
   if( g_Input->m_WasRightButtonClicked )
      g_StateMachine->PopState();
}

void EnchantingState::Draw()
{
   g_Display->BlitImageRect( g_Mask, 0, 0, 165, 165, 108, 108);
   g_Display->DrawBox(107, 107, 166, 166);

   g_SmallFont->DrawTextCentered( "Merchant's Inventory", 187, 110 );


   //  Draw the player's inventory
/*   for(unsigned int invcount = 0; invcount < 16; ++invcount)
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
   }*/

   //  Draw the Enchantings's inventory
   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if(invcount < m_EnchantingInventory.size())
      {
         g_Display->DrawSprite( g_Tiles[m_EnchantingInventory[invcount]->m_Tile], 124 + ((invcount % 4) * g_InventoryTileSize), 124 + ((invcount / 4) * g_InventoryTileSize) );
      }
      else
      {
         g_Display->DrawSprite( g_Tiles[70], 124 + ((invcount % 4) * g_InventoryTileSize), 124 + ((invcount / 4) * 32) );
      }
   }
   DrawConsoleStrings();

   if( DrawButton("Done", 220, 256 ) )
      g_StateMachine->PopState();

   if( g_Input->IsMouseInRegion( 124, 124, 124 + (4 * 32), 124 + ( 4 * 32 ) ) )
   {
      //  Turn the screen coordinates into inventory coordinates.
      int mapx = ((g_Input->m_MouseX - 124) / 32);
      int mapy = ((g_Input->m_MouseY - 124) / 32);
      int inventoryindex = mapy * 4 + mapx;

      if(inventoryindex < m_EnchantingInventory.size())
      {
         DrawToolTip( g_SmallFont, ConstructItemTooltip(m_EnchantingInventory[inventoryindex]), 123 + mapx * 32, 124 + mapy * 32, 2 );
      }
   }
   

   g_Display->BlitImage(g_Cursors[0], g_Input->m_MouseX, g_Input->m_MouseY);

}

void EnchantingState::OnEnter()
{
   AddConsoleString( "Left-click an item in the merchant's inventory to buy it.");
   AddConsoleString( "Left-click an item in your inventory to sell it." );
   AddConsoleString( "Right-click to cancel.", 255, 255, 255 );
   m_EnchantingInventory.clear();

   //  Add items to the inventory based on the player's stats.

   Item *temp = new Item;
   temp->Init("Items/healthpotion.cfg");
   m_EnchantingInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/manapotion.cfg");
   m_EnchantingInventory.push_back(temp);

   if(g_Player->m_Strength >= 2)
   {
      temp = new Item;
      temp->Init("Items/clotharmor.cfg");
      m_EnchantingInventory.push_back(temp);
   }

   temp = new Item;
   temp->Init("Items/dagger.cfg");
   m_EnchantingInventory.push_back(temp);

   if( g_Player->m_Strength >= 5 )
   {
      temp = new Item;
      temp->Init("Items/sword.cfg");
      m_EnchantingInventory.push_back(temp);
   }

   if( g_Player->m_Dexterity >= 5 )
   {
      temp = new Item;
      temp->Init("Items/bow.cfg");
      m_EnchantingInventory.push_back(temp);
   }

   if(g_Player->m_Strength >= 3)
   {
      temp = new Item;
      temp->Init("Items/leatherarmor.cfg");
      m_EnchantingInventory.push_back(temp);
   }

   if(g_Player->m_Strength >= 5 )
   {
      temp = new Item;
      temp->Init("Items/chainarmor.cfg");
      m_EnchantingInventory.push_back(temp);
   }


   if( g_Player->m_Strength >= 7 )
   {
      temp = new Item;
      temp->Init("Items/platearmor.cfg");
      m_EnchantingInventory.push_back(temp);
   }
}

void EnchantingState::OnExit()
{

}

void EnchantingState::DoPlayerInput()
{
   if( g_Input->m_WasLeftButtonClicked )
   {
      if( g_Input->m_MouseX > g_InventoryX && g_Input->m_MouseX < g_InventoryX + 4 * 32 && g_Input->m_MouseY > g_InventoryY && g_Input->m_MouseY < g_InventoryY + 4 * 32 )
      {

         //  Turn the screen coordinates into inventory coordinates.
         int mapx = ((g_Input->m_MouseX - g_InventoryX) / 32);
         int mapy = ((g_Input->m_MouseY - g_InventoryY) / 32);
         int inventoryindex = mapy * 4 + mapx;

         if(inventoryindex < g_Player->m_PlayerInventory.size())
         {
            vector<Item*>::iterator node = g_Player->m_PlayerInventory.begin();
            node += inventoryindex;
            stringstream tempstring;
            tempstring << "You sold a " << g_Player->m_PlayerInventory[inventoryindex]->m_Name << " for " << g_Player->m_PlayerInventory[inventoryindex]->m_Price << " coins.";
            AddConsoleString( tempstring.str() );
            g_Player->m_CurrentGold += g_Player->m_PlayerInventory[inventoryindex]->m_Price;

            if( (*node)->m_Stackable && (*node)->m_CurrentNumber > 1 )
               --(*node)->m_CurrentNumber;
            else
               g_Player->m_PlayerInventory.erase(node);
         }
      }
   }

   if( g_Input->WasLButtonClickedInRegion( 124, 124, 124 + (4 * 32), 124 + ( 4 * 32 ) ) )
   {
      //  Turn the screen coordinates into inventory coordinates.
      int mapx = ((g_Input->m_MouseX - 124) / 32);
      int mapy = ((g_Input->m_MouseY - 124) / 32);
      int inventoryindex = mapy * 4 + mapx;

      bool stackfound = false;
      if(inventoryindex < m_EnchantingInventory.size())
      {
         
         if( m_EnchantingInventory[inventoryindex]->m_Stackable )
         {
            //  Find the appropriate stack in the player's inventory
            for(int i = 0; i < g_Player->m_PlayerInventory.size(); ++i)
            {
               if( g_Player->m_PlayerInventory[i]->m_ItemType == m_EnchantingInventory[inventoryindex]->m_ItemType )
               {
                  stackfound = true;
                  if( g_Player->m_PlayerInventory[i]->m_CurrentNumber < 20 )
                  {
                     if( g_Player->m_CurrentGold >= m_EnchantingInventory[inventoryindex]->m_Price )
                     {
                        g_Player->m_CurrentGold -= m_EnchantingInventory[inventoryindex]->m_Price;
                        ++g_Player->m_PlayerInventory[i]->m_CurrentNumber;
                        stringstream tempstring;
                        tempstring << "You bought a " << m_EnchantingInventory[inventoryindex]->m_Name << " for " << m_EnchantingInventory[inventoryindex]->m_Price << " coins.";
                        AddConsoleString(tempstring.str());
                     }
                     else
                        AddConsoleString("Not enough money!");
                  }
                  else
                  {
                     stringstream temp;
                     temp.str("");
                     temp << "You cannot carry any more " << m_EnchantingInventory[inventoryindex]->m_Name << "s.";
                     AddConsoleString(temp.str());
                  }
               }
            }
         }

         if( ( m_EnchantingInventory[inventoryindex]->m_Stackable && !stackfound ) || !m_EnchantingInventory[inventoryindex]->m_Stackable ) 
         {
            if( g_Player->m_PlayerInventory.size() <= 15 )
            {
               if( g_Player->m_CurrentGold >= m_EnchantingInventory[inventoryindex]->m_Price )
               {
                  stringstream tempstring;
                  tempstring << "You bought a " << m_EnchantingInventory[inventoryindex]->m_Name << " for " << m_EnchantingInventory[inventoryindex]->m_Price << " coins.";
                  AddConsoleString(tempstring.str());
                  g_Player->m_CurrentGold -= m_EnchantingInventory[inventoryindex]->m_Price;
                  Item* temp = ItemFactory( m_EnchantingInventory[inventoryindex]->m_ItemType );
                  temp->m_STRBonus = 0;
                  temp->m_DEXBonus = 0;
                  temp->m_INTBonus = 0;
                  temp->m_WILBonus = 0;
                  temp->m_ENDBonus = 0;
                  temp->m_Price /= 2;
                  g_Player->m_PlayerInventory.push_back(temp);

               }
               else
               {
                  AddConsoleString("Not enough money!");
               }
            }
            else
               AddConsoleString("Not enough space in inventory!");
         }
      }
   }
}
