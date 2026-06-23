#include "StoreState.h"
#include "GameGlobals.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  StoreState
////////////////////////////////////////////////////////////////////////////////

StoreState::~StoreState()
{

}

void StoreState::Init(const std::string& configfile)
{

}

void StoreState::Shutdown(){}

void StoreState::Update()
{
   DoPlayerInput();
   if( g_InputSystem->WasRButtonClicked() )
      g_StateMachine->PopState();
}

void StoreState::Draw()
{
   DrawImageRectAt( g_Mask, 0, 0, 165, 165, 108, 108);
   DrawBoxAt(107, 107, 166, 166);

   DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "Merchant's Inventory", 187, 110 );


   //  Draw the player's inventory
/*   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if(invcount < g_Player->m_PlayerInventory.size())
      {
         DrawImageAt( g_Tiles[g_Player->m_PlayerInventory[invcount]->m_Tile], g_InventoryX + ((invcount % 4) * g_InventoryTileSize), g_InventoryY + ((invcount / 4) * g_InventoryTileSize) );
         if( g_Player->m_PlayerInventory[invcount]->m_Stackable )
         {
            stringstream tempstream;
            tempstream << g_Player->m_PlayerInventory[invcount]->m_CurrentNumber;
            DrawTextAt(g_smallFont.get(), g_smallFontSize,  tempstream.str(), g_InventoryX + ((invcount % 4) * g_InventoryTileSize) + 23, g_InventoryY + ((invcount / 4) * g_InventoryTileSize) + 23, 0, 0, 0 );
            DrawTextAt(g_smallFont.get(), g_smallFontSize,  tempstream.str(), g_InventoryX + ((invcount % 4) * g_InventoryTileSize) + 22, g_InventoryY + ((invcount / 4) * g_InventoryTileSize) + 22 );
         }
      }
      else
      {
         DrawImageAt( g_Tiles[70], g_InventoryX + ((invcount % 4) * g_InventoryTileSize), g_InventoryY + ((invcount / 4) * 32) );
      }
   }*/

   //  Draw the stores's inventory
   for(unsigned int invcount = 0; invcount < 16; ++invcount)
   {
      if(invcount < m_StoreInventory.size())
      {
         DrawSpriteAt( g_Tiles[m_StoreInventory[invcount]->m_Tile], 124 + ((invcount % 4) * g_InventoryTileSize), 124 + ((invcount / 4) * g_InventoryTileSize) );
      }
      else
      {
         DrawSpriteAt( g_Tiles[70], 124 + ((invcount % 4) * g_InventoryTileSize), 124 + ((invcount / 4) * 32) );
      }
   }
   DrawConsoleStrings();

   if( DrawButton("Done", 220, 256 ) )
      g_StateMachine->PopState();

   if( IsMouseInDesignRegion( 124, 124, 124 + (4 * 32), 124 + ( 4 * 32 ) ) )
   {
      //  Turn the screen coordinates into inventory coordinates.
      int mapx = ((GetDesignMouseX() - 124) / 32);
      int mapy = ((GetDesignMouseY() - 124) / 32);
      int inventoryindex = mapy * 4 + mapx;

      if(inventoryindex < m_StoreInventory.size())
      {
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize, ConstructItemTooltip(m_StoreInventory[inventoryindex]), 123 + mapx * 32, 124 + mapy * 32, 2 );
      }
   }

   //  Do inventory tooltips.
   if( IsMouseInDesignRegion( g_InventoryX, g_InventoryY, g_InventoryX + (4 * 32), g_InventoryY + ( 4 * 32 ) ) )
   {
      //  Turn the screen coordinates into inventory coordinates.
      int mapx = ((GetDesignMouseX() - g_InventoryX) / 32);
      int mapy = ((GetDesignMouseY() - g_InventoryY) / 32);
      int inventoryindex = mapy * 4 + mapx;

      if(inventoryindex < g_Player->m_PlayerInventory.size())
      {
         DrawDesignToolTip( g_smallFont.get(), g_smallFontSize, ConstructItemTooltip(g_Player->m_PlayerInventory[inventoryindex]), g_InventoryX + mapx * 32, g_InventoryY + mapy * 32, 2 );
      }
   }


   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());

}

void StoreState::OnEnter()
{
   AddConsoleString( "Left-click an item in the merchant's inventory to buy it.");
   AddConsoleString( "Left-click an item in your inventory to sell it." );
   AddConsoleString( "Right-click to cancel.", 255, 255, 255 );
   m_StoreInventory.clear();

   //  Add items to the inventory based on the player's stats.

   Item *temp = new Item;
   temp->Init("Items/healthpotion.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/manapotion.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/clotharmor.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/dagger.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/sword.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/bow.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/leatherarmor.cfg");
   m_StoreInventory.push_back(temp);

   temp = new Item;
   temp->Init("Items/chainarmor.cfg");
   m_StoreInventory.push_back(temp);


   temp = new Item;
   temp->Init("Items/platearmor.cfg");
   m_StoreInventory.push_back(temp);
}

void StoreState::OnExit()
{

}

void StoreState::DoPlayerInput()
{
   if( g_InputSystem->WasLButtonClicked() )
   {
      if( GetDesignMouseX() > g_InventoryX && GetDesignMouseX() < g_InventoryX + 4 * 32 && GetDesignMouseY() > g_InventoryY && GetDesignMouseY() < g_InventoryY + 4 * 32 )
      {

         //  Turn the screen coordinates into inventory coordinates.
         int mapx = ((GetDesignMouseX() - g_InventoryX) / 32);
         int mapy = ((GetDesignMouseY() - g_InventoryY) / 32);
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

   if( WasLButtonClickedInDesignRegion( 124, 124, 124 + (4 * 32), 124 + ( 4 * 32 ) ) )
   {
      //  Turn the screen coordinates into inventory coordinates.
      int mapx = ((GetDesignMouseX() - 124) / 32);
      int mapy = ((GetDesignMouseY() - 124) / 32);
      int inventoryindex = mapy * 4 + mapx;

      bool stackfound = false;
      if(inventoryindex < m_StoreInventory.size())
      {
         
         if( m_StoreInventory[inventoryindex]->m_Stackable )
         {
            //  Find the appropriate stack in the player's inventory
            for(int i = 0; i < g_Player->m_PlayerInventory.size(); ++i)
            {
               if( g_Player->m_PlayerInventory[i]->m_ItemType == m_StoreInventory[inventoryindex]->m_ItemType )
               {
                  stackfound = true;
                  if( g_Player->m_PlayerInventory[i]->m_CurrentNumber < 20 )
                  {
                     if( g_Player->m_CurrentGold >= m_StoreInventory[inventoryindex]->m_Price )
                     {
                        g_Player->m_CurrentGold -= m_StoreInventory[inventoryindex]->m_Price;
                        ++g_Player->m_PlayerInventory[i]->m_CurrentNumber;
                        stringstream tempstring;
                        tempstring << "You bought a " << m_StoreInventory[inventoryindex]->m_Name << " for " << m_StoreInventory[inventoryindex]->m_Price << " coins.";
                        AddConsoleString(tempstring.str());
                     }
                     else
                        AddConsoleString("Not enough money!");
                  }
                  else
                  {
                     stringstream temp;
                     temp.str("");
                     temp << "You cannot carry any more " << m_StoreInventory[inventoryindex]->m_Name << "s.";
                     AddConsoleString(temp.str());
                  }
               }
            }
         }

         if( ( m_StoreInventory[inventoryindex]->m_Stackable && !stackfound ) || !m_StoreInventory[inventoryindex]->m_Stackable ) 
         {
            if( g_Player->m_PlayerInventory.size() <= 15 )
            {
               if( g_Player->m_CurrentGold >= m_StoreInventory[inventoryindex]->m_Price )
               {
                  stringstream tempstring;
                  tempstring << "You bought a " << m_StoreInventory[inventoryindex]->m_Name << " for " << m_StoreInventory[inventoryindex]->m_Price << " coins.";
                  AddConsoleString(tempstring.str());
                  g_Player->m_CurrentGold -= m_StoreInventory[inventoryindex]->m_Price;
                  Item* temp = ItemFactory( m_StoreInventory[inventoryindex]->m_ItemType );
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
