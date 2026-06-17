#include "EditorState.h"
#include "GameGlobals.h"
#include "Geist/Engine.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>
#include <filesystem>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  EditorState
////////////////////////////////////////////////////////////////////////////////

EditorState::~EditorState()
{
   Shutdown();
}

void EditorState::Init(const std::string& configfile)
{

   string names[13] = 
   {
      "Maps/inaria.map",
      "Maps/castleoftheslornking.map",
      "Maps/collapsingcave.map",
      "Maps/deadlymazeofdeath.map",
      "Maps/mountaintrial.map",
      "Maps/passagetoapocalypse.map",
      "Maps/passagetodoom.map",
      "Maps/thecube.map",
      "Maps/theoubliette.map",
      "Maps/underwatertunnel.map",
      "Maps/volcanoheart.map",
      "Maps/intro.map",
      "Maps/inariatest.map"
   };

   for(int i = 0; i < 13; ++i )
   {
      g_AllMaps[i] = new Map;
      g_AllMaps[i]->Init("");
      g_AllMaps[i]->LoadMap(names[i].c_str());
      g_AllMaps[i]->m_DrawInvisibles = true;
   }

   g_CurrentEditorMap = 0;

   for (const auto& entry : std::filesystem::directory_iterator("Items"))
   {
      if (entry.path().extension() == ".cfg")
      {
         Item* tempitem = new Item;
         tempitem->Init(entry.path().string());
         m_AllItems.push_back(tempitem);
      }
   }

   for (const auto& entry : std::filesystem::directory_iterator("NPCs"))
   {
      if (entry.path().extension() == ".cfg")
      {
         NPC* tempnpc = new NPC;
         tempnpc->Init(entry.path().string());
         m_AllNPCs.push_back(tempnpc);
      }
   }

   m_MapMode = MM_DrawTerrain; // 0 = terrain, 1 = items, 2 = NPCs

   m_CurrentTerrainType = -1;
   m_CurrentNPC = NULL;
   m_CurrentItem = NULL;
   m_CurrentMapLink = NULL;

   m_ItemListX = 400;
   m_ItemListY = 280;
}

void EditorState::OnEnter()
{
   g_ViewRange = 11;
   g_Offset = 0;
}

void EditorState::OnExit()
{
   g_Offset = (int)g_Engine->m_EngineConfig.GetNumber("offset");
   g_ViewRange = (int)g_Engine->m_EngineConfig.GetNumber("viewrange");

   g_Map->LoadMap(g_AllMaps[g_CurrentEditorMap]->m_MapName.c_str());

}

void EditorState::Shutdown()
{

}

void EditorState::Update()
{
   uint32_t startTime = static_cast<uint32_t>(g_Engine->GameTimeInMS());

   if(g_InputSystem->WasKeyPressed(KEY_E) && g_InputSystem->IsKeyDown(KEY_LEFT_CONTROL) )
   {
      g_StateMachine->MakeStateTransition(STATE_MAINSTATE);
   }

   if( (g_InputSystem->IsKeyDown(KEY_A) || g_InputSystem->IsKeyDown(KEY_LEFT)) && g_Player->m_PlayerPosX > 0 )
   {
      g_Player->m_PlayerPosX -= 1;
   }
   else if( (g_InputSystem->IsKeyDown(KEY_D) || g_InputSystem->IsKeyDown(KEY_RIGHT)) && g_Player->m_PlayerPosX < g_AllMaps[g_CurrentEditorMap]->m_Width )
   {
      g_Player->m_PlayerPosX += 1;
   }
   else if( (g_InputSystem->IsKeyDown(KEY_W) || g_InputSystem->IsKeyDown(KEY_UP)) && g_Player->m_PlayerPosY > 0 )
   {
      g_Player->m_PlayerPosY -= 1;
   }
   else if( (g_InputSystem->IsKeyDown(KEY_S) || g_InputSystem->IsKeyDown(KEY_DOWN)) && g_Player->m_PlayerPosY < g_AllMaps[g_CurrentEditorMap]->m_Width )
   {
      g_Player->m_PlayerPosY += 1;
   }

   DoPlayerInput();

   g_AllMaps[g_CurrentEditorMap]->Update();
}


void EditorState::Draw()
{

   g_AllMaps[g_CurrentEditorMap]->DrawEditor();


   
   list<NPC*>::iterator node;
   for( node = g_AllMaps[g_CurrentEditorMap]->m_NPCList.begin(); node != g_AllMaps[g_CurrentEditorMap]->m_NPCList.end(); ++node )
      (*node)->Draw();

   list<Item*>::iterator itemnode;
   for(itemnode = g_AllMaps[g_CurrentEditorMap]->m_ItemList.begin(); itemnode != g_AllMaps[g_CurrentEditorMap]->m_ItemList.end(); ++itemnode)
      (*itemnode)->Draw();

   //  Draw the minimap

   int xstepper = 256 / g_AllMaps[g_CurrentEditorMap]->m_Width;
   int ystepper = 256 / g_AllMaps[g_CurrentEditorMap]->m_Height;
   for(int i = 0; i < g_AllMaps[g_CurrentEditorMap]->m_Width; i = i + 1)
   {
      for( int j = 0; j < g_AllMaps[g_CurrentEditorMap]->m_Height; j = j + 1 )     
      {
         DrawBoxAt(384 + (i * xstepper), (j * ystepper), xstepper, ystepper, 
                              g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[g_AllMaps[g_CurrentEditorMap]->m_map[i][j].m_TerrainType].m_MinimapColor.r,
                  g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[g_AllMaps[g_CurrentEditorMap]->m_map[i][j].m_TerrainType].m_MinimapColor.g,
                  g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[g_AllMaps[g_CurrentEditorMap]->m_map[i][j].m_TerrainType].m_MinimapColor.b,
                  g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[g_AllMaps[g_CurrentEditorMap]->m_map[i][j].m_TerrainType].m_MinimapColor.a,
                  true);
      }
   }

   //  Draw all items on this map
/*   for( list<Item*>::iterator node = g_AllMaps[g_CurrentEditorMap]->m_ItemList.begin(); node != g_AllMaps[g_CurrentEditorMap]->m_ItemList.end(); ++node )
   {
      DrawSpriteResizedAt( g_Tiles[(*node)->m_Tile], 384 + (xstepper * (*node)->m_PosX), (ystepper * (*node)->m_PosY), xstepper, ystepper );
   }


   //  Draw all NPCs on this map
   for( list<NPC*>::iterator node = g_AllMaps[g_CurrentEditorMap]->m_NPCList.begin(); node != g_AllMaps[g_CurrentEditorMap]->m_NPCList.end(); ++node )
   {
      DrawSpriteResizedAt( g_Tiles[(*node)->m_Tile], 384 + (xstepper * (*node)->m_PosX), (ystepper * (*node)->m_PosY), xstepper, ystepper );
   }*/

   //  Draw a box representing what the player is looking at.
   xstepper = 256 / g_AllMaps[g_CurrentEditorMap]->m_Width;
   ystepper = 256 / g_AllMaps[g_CurrentEditorMap]->m_Height;

   DrawBoxAt( 384 + ((g_Player->m_PlayerPosX - g_ViewRange) * xstepper), (g_Player->m_PlayerPosY - g_ViewRange ) * ystepper, 23 * xstepper, 23 * ystepper );


   DrawTextAt(g_smallFont.get(), g_smallFontSize, g_AllMaps[g_CurrentEditorMap]->m_MapName.c_str(), 0, 370 );

   if( g_InputSystem->IsMouseInDesignRegion(g_Offset, g_Offset, 368, 368) )
   {
      int x = ( GetDesignMouseX() - g_Offset ) / 16;
      int y = ( GetDesignMouseY() - g_Offset ) / 16;

      x += g_Player->m_PlayerPosX;
      y += g_Player->m_PlayerPosY;

      x -= g_ViewRange;
      y -= g_ViewRange;

      char temp[64];
      sprintf( temp, "X: %d Y: %d", x, y);
      DrawTextAt(g_smallFont.get(), g_smallFontSize,  temp, 368 - GetStringMetrics(g_smallFont.get(), g_smallFontSize, temp), 370 );
   }
   else
   {
      char temp[64];
      sprintf( temp, "X: %d Y: %d", g_Player->m_PlayerPosX, g_Player->m_PlayerPosY);
      DrawTextAt(g_smallFont.get(), g_smallFontSize,  temp, 368 - GetStringMetrics(g_smallFont.get(), g_smallFontSize, temp), 370 );
   }


   

   int counter = 0;
   switch( m_MapMode )
   {
   case 0:

      //  Draw the terrain types.
      DrawTextAt(g_font.get(), g_fontSize, "Terrain", 438, 260);
      DrawTextAt(g_font.get(), g_fontSize, "Items", 518, 260, 128, 128, 128 );
      DrawTextAt(g_font.get(), g_fontSize, "NPCs", 578, 260, 128, 128, 128 );
      counter = 0;      
      for(int i = 0; i <= 21; ++ i)
      {
         DrawSpriteResizedAt( g_Tiles[g_AllMaps[0]->m_terrainTypes[i].m_tile], m_ItemListX + ( ( counter % 10 ) * 24 ), m_ItemListY + ((counter / 10 ) * 24 ), 24, 24 );
         ++counter;
      }
      break;


   case 1:
      //  Draw the item types.
      DrawTextAt(g_font.get(), g_fontSize, "Terrain", 438, 260, 128, 128, 128);
      DrawTextAt(g_font.get(), g_fontSize, "Items", 518, 260 );
      DrawTextAt(g_font.get(), g_fontSize, "NPCs", 578, 260, 128, 128, 128 );
      counter = 0;
      for ( list<Item*>::iterator node = m_AllItems.begin(); node != m_AllItems.end(); ++node )
      {
         DrawSpriteResizedAt( g_Tiles[(*node)->m_Tile], m_ItemListX + ( ( counter % 10 ) * 24 ), m_ItemListY + ((counter / 10 ) * 24 ), 24, 24 );
         ++counter;
      }
      break;

      //  Draw the NPCs.
   case 2:
      DrawTextAt(g_font.get(), g_fontSize, "Terrain", 438, 260, 128, 128, 128 );
      DrawTextAt(g_font.get(), g_fontSize, "Items", 518, 260, 128, 128, 128 );
      DrawTextAt(g_font.get(), g_fontSize, "NPCs", 578, 260 );
      counter = 0;
      for ( list<NPC*>::iterator node = m_AllNPCs.begin(); node != m_AllNPCs.end(); ++node )
      {
         DrawSpriteResizedAt( g_Tiles[(*node)->m_Tile], m_ItemListX + ( ( counter % 10 ) * 24 ), m_ItemListY + ((counter / 10 ) * 24 ), 24, 24 );
         ++counter;
      }
      break;
   }

   //  Draw tooltips
   if( m_MapMode == 0 )
   {
      if( g_InputSystem->IsMouseInDesignRegion( m_ItemListX, m_ItemListY, 640, 480 ) )
      {
         int index = ((GetDesignMouseY() - m_ItemListY) / 24 ) * 10 ;
         index += ((GetDesignMouseX() - m_ItemListX) / 24);

         if( index < 22 )
         {
            ColoredString* temp = new ColoredString(g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[index].m_Name);
            vector<ColoredString> coloredstrings;
            coloredstrings.push_back(*temp);
            if( g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[index].m_passable )
            {
               temp = new ColoredString( "Passable: True" );
               coloredstrings.push_back(*temp);
            }
            else
            {
               temp = new ColoredString( "Passable: False" );
               coloredstrings.push_back(*temp);
            }

            DrawToolTip( g_smallFont.get(), g_smallFontSize,  coloredstrings, GetDesignMouseX() - (GetDesignMouseX() % 24), GetDesignMouseY() - (GetDesignMouseY() % 24), 2 );
         }
      }
   }


   //  Draw tooltips
   if( m_MapMode == 1 )
   {
      if( g_InputSystem->IsMouseInDesignRegion( m_ItemListX, m_ItemListY, 640, 480 ) )
      {
         int index = ((GetDesignMouseY() - m_ItemListY) / 24 ) * 10 ;
         index += ((GetDesignMouseX() - m_ItemListX) / 24);

         if( index < m_AllItems.size() )
         {
            list<Item*>::iterator node = m_AllItems.begin();
            for(int i = 0; i < index; ++i)
               ++node;
            ColoredString* temp = new ColoredString((*node)->m_Name);
            vector<ColoredString> coloredstrings;
            coloredstrings.push_back(*temp);
            DrawToolTip( g_smallFont.get(), g_smallFontSize, coloredstrings, GetDesignMouseX() - (GetDesignMouseX() % 24), GetDesignMouseY() - (GetDesignMouseY() % 24), 2 );
         }
      }
   }

   //  Draw tooltips
   if( m_MapMode == 2 )
   {
      if( g_InputSystem->IsMouseInDesignRegion( m_ItemListX, m_ItemListY, 640, 480 ) )
      {
         int index = ((GetDesignMouseY() - m_ItemListY) / 24 ) * 10 ;
         index += ((GetDesignMouseX() - m_ItemListX) / 24);

         if( index < m_AllNPCs.size() )
         {
            list<NPC*>::iterator node = m_AllNPCs.begin();
            for(int i = 0; i < index; ++i)
               ++node;
            ColoredString* temp = new ColoredString((*node)->m_Name);
            vector<ColoredString> coloredstrings;
            coloredstrings.push_back(*temp);
            DrawToolTip( g_smallFont.get(), g_smallFontSize,  coloredstrings, GetDesignMouseX() - (GetDesignMouseX() % 24), GetDesignMouseY() - (GetDesignMouseY() % 24), 2 );
         }
      }
   }

   if( m_MapMode == 0 )
   {
      if( m_CurrentTerrainType >= 0 )
         DrawSpriteAt(g_Tiles[g_AllMaps[g_CurrentEditorMap]->m_terrainTypes[m_CurrentTerrainType].m_tile], GetDesignMouseX(), GetDesignMouseY() );
   }

   if( m_MapMode == 1 )
   {
      if( m_CurrentItem )
      {
         DrawSpriteAt(g_Tiles[m_CurrentItem->m_Tile], GetDesignMouseX(), GetDesignMouseY() );
      }
   }

   if( m_MapMode == 2 )
   {
      if( m_CurrentNPC )
      {
         DrawSpriteAt(g_Tiles[m_CurrentNPC->m_Tile], GetDesignMouseX(), GetDesignMouseY() );
      }
   }

   //  Draw Miscellanious Buttons
   DrawTextAt(g_font.get(), g_fontSize, "Delete Mode", 428, 428 );

   //  Draw status info
/*   if( m_Deleting )
   {
      DrawTextAt(g_font.get(), g_fontSize,  "In Delete mode.  Right-click to cancel.", 0, 432, 255, 255, 0);
   }

   if( m_PlacingMapLinkSource )
   {
      DrawTextAt(g_font.get(), g_fontSize,  "Please click the source of the map link.", 0, 432, 255, 255, 0);
   }

   if( m_PlacingMapLinkDestination )
   {
      DrawTextAt(g_font.get(), g_fontSize,  "Please click the destination of the map link.", 0, 432, 255, 255, 0);
   }

   if( m_LinkingEgg )
   {
      DrawTextAt(g_font.get(), g_fontSize,  "Please click the item this item is linked to.", 0, 432, 255, 255, 0);
   }*/

   if( g_InputSystem->IsMouseInDesignRegion( g_Offset, g_Offset, 364, 364 ) )
   {
      int x = (GetDesignMouseX() - g_Offset) / 32;
      int y = (GetDesignMouseY() - g_Offset) / 32;

      x -= g_ViewRange;
      y -= g_ViewRange;

      x += g_Player->m_PlayerPosX;
      y += g_Player->m_PlayerPosY;

      //  No edge of map link?  Check for regular links.
      vector<MapLink*>::iterator node = g_AllMaps[g_CurrentEditorMap]->m_MapLinks.begin();
      for(node; node != g_AllMaps[g_CurrentEditorMap]->m_MapLinks.end(); ++node)
      {
         if( (*node)->m_XCoord == x && (*node)->m_YCoord == y )
         {
            stringstream sstream;
            sstream << "Link to " << (*node)->m_XDest << ", " << (*node)->m_YDest << ", " << (*node)->m_DestinationMap;
            ColoredString* temp = new ColoredString(sstream.str());
            vector<ColoredString> coloredstrings;
            coloredstrings.push_back(*temp);
            DrawToolTip( g_smallFont.get(), g_smallFontSize, coloredstrings, GetDesignMouseX(), GetDesignMouseY(), 3);
            break;
         }
      }

      //  Check if this square is the target of another map link.
      for(int i = 0; i < 12; ++i)
      {
         vector<MapLink*>::iterator node = g_AllMaps[i]->m_MapLinks.begin();
         for(node; node != g_AllMaps[i]->m_MapLinks.end(); ++node)
         {
            if( (*node)->m_DestinationMap == g_AllMaps[g_CurrentEditorMap]->m_MapName && (*node)->m_XDest == x && (*node)->m_YDest == y )
            {
               stringstream sstream;
               sstream << "Link from " << (*node)->m_XCoord << ", " << (*node)->m_YCoord << ", " << g_AllMaps[i]->m_MapName;
               ColoredString* temp = new ColoredString(sstream.str());
               vector<ColoredString> coloredstrings;
               coloredstrings.push_back(*temp);
               DrawToolTip( g_smallFont.get(), g_smallFontSize, coloredstrings, GetDesignMouseX(), GetDesignMouseY(), 3);
               break;
            }
         }
      }

      //  Check for an item link
      list<Item*>::iterator itemnode = g_AllMaps[g_CurrentEditorMap]->m_ItemList.begin();
      for(itemnode; itemnode != g_AllMaps[g_CurrentEditorMap]->m_ItemList.end(); ++itemnode)
      {
         if( (*itemnode)->m_PosX == x && (*itemnode)->m_PosY == y )
         {
            //  Draw a line to the linked item
            if( (*itemnode)->m_TargetX != 0 || (*itemnode)->m_TargetY != 0 )
            {
               int sourcex = (GetDesignMouseX() - g_Offset) / 32;
               int sourcey = (GetDesignMouseY() - g_Offset) / 32;

               int targetx = (*itemnode)->m_TargetX;
               int targety = (*itemnode)->m_TargetY;

               targetx -= g_Player->m_PlayerPosX;
               targety -= g_Player->m_PlayerPosY;

               targetx += g_ViewRange;
               targety += g_ViewRange;

               DrawLineEx(
                  Vector2{ float(g_Offset + sourcex * g_TileSize + 16), float(g_Offset + sourcey * g_TileSize + 16) },
                  Vector2{ float(g_Offset + targetx * g_TileSize + 16), float(g_Offset + targety * g_TileSize + 16) },
                  1.0f, WHITE);

               break;
            }
         }
      }
   }

   if ( DrawButton( "Save Map", 16, 420 ) )
   {
      g_AllMaps[g_CurrentEditorMap]->SaveMap(g_AllMaps[g_CurrentEditorMap]->m_MapName.c_str());
   }

      if( DrawButton( "Previous Map", 16, 440 ) )
   {
      --g_CurrentEditorMap;
      if(g_CurrentEditorMap < 0 )
         g_CurrentEditorMap = 12;
   }

   if( DrawButton( "Next Map", 16, 460 ) )
   {
      ++g_CurrentEditorMap;
      if(g_CurrentEditorMap > 12 )
         g_CurrentEditorMap = 0;
   }

   if( g_InputSystem->WasLButtonClickedInDesignRegion( 428, 428, 428 + GetStringMetrics(g_font.get(), g_fontSize, "Delete Mode"), 428 + 16 ) )
   {
   }


   if( g_InputSystem->WasLButtonClickedInDesignRegion( 428, 444, 428 + GetStringMetrics(g_font.get(), g_fontSize, "Add Map Link"), 444 + 16 ) )
   {
   }


   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());
}

void EditorState::DoPlayerInput()
{
   if(  g_InputSystem->IsMouseInDesignRegion( 0, 0, 416, 416 ) )
   {

   }

   if( g_InputSystem->IsLButtonDown() )
   {
      //  Scroll around the minimap
      if( GetDesignMouseX() >= 372 && GetDesignMouseX() < 564 &&
         GetDesignMouseY() >= 0 && GetDesignMouseY() < 192 )
      {
         g_Player->m_PlayerPosX = ( GetDesignMouseX() - 372 ) / 3;
         g_Player->m_PlayerPosY = ( GetDesignMouseY() ) / 3;
      }
   }

   if( g_InputSystem->WasRButtonClicked() )
   {
      m_CurrentTerrainType = -1;
      m_CurrentNPC = NULL;
      m_CurrentItem = NULL;
   }


   if( g_InputSystem->WasLButtonClickedInDesignRegion( 438, 196, 517, 212 ) )
   {
      m_MapMode = 0;
   }

   if( g_InputSystem->WasLButtonClickedInDesignRegion( 518, 196, 577, 212 ) )
   {
      m_MapMode = 1;
   }

   if( g_InputSystem->WasLButtonClickedInDesignRegion( 578, 196, 639, 212 ) )
   {
      m_MapMode = 2;
   }


   if( m_MapMode == 0 )
   {
      if( g_InputSystem->WasLButtonClickedInDesignRegion( m_ItemListX, m_ItemListY, 640, 480 ) )
      {
         int index = ((GetDesignMouseY() - m_ItemListY) / 24 ) * 10 ;
         index += ((GetDesignMouseX() - m_ItemListX) / 24);

         if( index < 22 )
         {
            m_CurrentTerrainType = index;
         }
      }

      if( g_InputSystem->IsLButtonDownInDesignRegion( g_Offset, g_Offset, 364, 364 ) && m_CurrentTerrainType >= 0 )
      {
         int x = (GetDesignMouseX() - g_Offset) / 16;
         int y = (GetDesignMouseY() - g_Offset) / 16;

         x -= g_ViewRange;
         y -= g_ViewRange;

         x += g_Player->m_PlayerPosX;
         y += g_Player->m_PlayerPosY;

         g_AllMaps[g_CurrentEditorMap]->m_map[x][y].m_TerrainType = m_CurrentTerrainType;
      }
   }



   if( m_MapMode == 2 )
   {
      if( g_InputSystem->WasLButtonClickedInDesignRegion( m_ItemListX, m_ItemListY, 640, 480 ) )
      {
         int index = ((GetDesignMouseY() - m_ItemListY) / 24 ) * 10 ;
         index += ((GetDesignMouseX() - m_ItemListX) / 24);

         if( index < m_AllNPCs.size() )
         {
            list<NPC*>::iterator node = m_AllNPCs.begin();
            for(int i = 0; i < index; ++i)
               ++node;
            m_CurrentNPC = (*node);
         }
      }

      if( g_InputSystem->WasLButtonClickedInDesignRegion( g_Offset, g_Offset, 364, 364 ) )
      {
         int x = (GetDesignMouseX() - g_Offset) / 16;
         int y = (GetDesignMouseY() - g_Offset) / 16;

         x -= g_ViewRange;
         y -= g_ViewRange;

         x += g_Player->m_PlayerPosX;
         y += g_Player->m_PlayerPosY;

         if( m_CurrentNPC != NULL )
         {
            NPC* npc = new NPC( *m_CurrentNPC );

            npc->m_PosX = x;
            npc->m_PosY = y;

            g_AllMaps[g_CurrentEditorMap]->m_NPCList.push_back(npc);
         }
      }
   }

   if( m_MapMode == 1 )
   {
      if( g_InputSystem->WasLButtonClickedInDesignRegion( m_ItemListX, m_ItemListY, 640, 480 ) )
      {
         int index = ((GetDesignMouseY() - m_ItemListY) / 24 ) * 10 ;
         index += ((GetDesignMouseX() - m_ItemListX) / 24);

         if( index < m_AllItems.size() )
         {
            list<Item*>::iterator node = m_AllItems.begin();
            for(int i = 0; i < index; ++i)
               ++node;
            m_CurrentItem = (*node);
         }
      }

      if( g_InputSystem->WasLButtonClickedInDesignRegion( g_Offset, g_Offset, 364, 364 ) )
      {
         int x = (GetDesignMouseX() - g_Offset) / 16;
         int y = (GetDesignMouseY() - g_Offset) / 16;

         x -= g_ViewRange;
         y -= g_ViewRange;

         x += g_Player->m_PlayerPosX;
         y += g_Player->m_PlayerPosY;

         if( m_CurrentItem != NULL )
         {
            Item* item = new Item( *m_CurrentItem );

            item->m_PosX = x;
            item->m_PosY = y;

            g_AllMaps[g_CurrentEditorMap]->m_ItemList.push_back(item);
         }
      }
   }

}

