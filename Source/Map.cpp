#include "GameGlobals.h"
#include "../Geist/Source/Engine.h"
#include "../Geist/Source/Globals.h"
#include "GameGlobals.h"
#include <fstream>
#include <assert.h>
#include <sstream>

using namespace std;

int monsterTypes[22] = { 9, 10, 11, 12, 13, 14, 16, 17, 18, 19, 20, 21, 22, 23, 57, 59, 60, 61, 62, 63, 64, 65 };
int itemTypes[9] = { 0, 1, 2, 8, 9, 10, 12, 13, 14 };

bool isGrassType(int i)
{
   return
      i == 0 || 
      i == 1 || 
      i == 3 || 
      i == 6 || 
      i == 8 ||
      i == 12 ||
      i == 13 || 
      i == 18 ||
      i == 19 ||
      i == 103;
}

bool isGrassOrWaterType(int i)
{
   return i == 2 || isGrassType(i); 
}

bool isPavement(int i )
{
   return ( i == 5 || i == 100 );
}

bool isPavedType(int i)
{
   return
      i == 102 ||
      i == 104;
}

bool isWallType(int i)
{
   return  i == 4 || i == 10 || i == 9;   
}

Map::Map()
{
   m_Width = 64;
   m_Height = 64;
}

Map::~Map()
{
   Shutdown();
}

void Map::Init(const std::string& configfile)
{
   g_Offset = (int)g_Engine->m_EngineConfig.GetNumber("offset");
   g_TileSize = (int)g_Engine->m_EngineConfig.GetNumber("tilesize");
   g_ViewRange = (int)g_Engine->m_EngineConfig.GetNumber("viewrange");
   m_DrawInvisibles = false;
   m_Peer = false;
}

void Map::Shutdown()
{

}


void Map::DrawPeer()
{
   //  Find out how big the space we have to display the map.
   int mapwidth = ( (g_ViewRange * 2) + 1 ) * g_TileSize;
   int mapheight = mapwidth;

   int cellwidth = mapwidth / m_Width;

   int newwidth = cellwidth * m_Width;
   newwidth = mapheight - newwidth;
   newwidth /= 2;

   for( int i = 0; i < m_Width; ++i)
   {
      for( int j = 0; j < m_Height; ++ j)
      {
         DrawSpriteResizedAt( g_Tiles[m_terrainTypes[m_map[i][j].m_TerrainType].m_tile], g_Offset + newwidth + i * cellwidth, g_Offset + newwidth + j * cellwidth, cellwidth, cellwidth );
      }
   }


   //  Draw the unit list.
   list<NPC*>::iterator node;
   for( node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
   {
      DrawSpriteResizedAt( g_Tiles[(*node)->m_Tile], g_Offset + newwidth + (*node)->m_PosX * cellwidth, g_Offset + newwidth + (*node)->m_PosY * cellwidth, cellwidth, cellwidth );
   }


   //  Draw the item list.
   list<Item*>::iterator itemnode;
   for(itemnode = g_Map->m_ItemList.begin(); itemnode != g_Map->m_ItemList.end(); ++itemnode)
   {
      DrawSpriteResizedAt( g_Tiles[(*itemnode)->m_Tile], g_Offset + newwidth + (*itemnode)->m_PosX * cellwidth, g_Offset + newwidth + (*itemnode)->m_PosY * cellwidth, cellwidth, cellwidth );
   }

   //  Draw the player
   DrawSpriteResizedAt( g_Tiles[46], g_Offset + newwidth + g_Player->m_PlayerPosX * cellwidth, g_Offset + newwidth + g_Player->m_PlayerPosY * cellwidth, cellwidth, cellwidth );
}

void Map::DrawEditor()
{
   int x, y, adjustedx, adjustedy;
   for(x = g_Player->m_PlayerPosX - ( g_ViewRange ); x <= g_Player->m_PlayerPosX + ( g_ViewRange ); ++x)
   {
      if ( x < 0 || x >= g_AllMaps[g_CurrentEditorMap]->m_Width )
         continue;

      for(y = g_Player->m_PlayerPosY - ( g_ViewRange ) ; y <= g_Player->m_PlayerPosY + ( g_ViewRange); ++y)
      {
         if ( y < 0 || y >= g_AllMaps[g_CurrentEditorMap]->m_Height )
            continue;

         if(x >= 0 && x < g_AllMaps[g_CurrentEditorMap]->m_Width && y >= 0 && y < g_AllMaps[g_CurrentEditorMap]->m_Height)
         {
            adjustedx = x - (g_Player->m_PlayerPosX - ( g_ViewRange * 2 ));
            adjustedy = y - (g_Player->m_PlayerPosY - ( g_ViewRange * 2 ));
         }

         DrawSpriteResizedAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], g_Offset + (adjustedx * ( g_TileSize / 2)) - (g_ViewRange * (g_TileSize / 2)), g_Offset + (adjustedy * ( g_TileSize / 2))  - (g_ViewRange * (g_TileSize / 2)), ( g_TileSize / 2), ( g_TileSize / 2) );
      }
   }


   /*
               //  If this is a map link, and "Draw Map Links" is on, draw a yellow square here.
            if( m_DrawInvisibles )
            {
               bool invisible = false;
               //  Check if this is on the map link list.
               //  First, check for the "edge of map" link
               if( x == 0 || x == m_Width - 1 ||
                  y == 0 || y == m_Height - 1 )
               {
                  //  Scan the list for a map link with coordinates 99, 99
                  vector<MapLink*>::iterator node = m_MapLinks.begin();
                  for(node; node != m_MapLinks.end(); ++node)
                  {
                     if( (*node)->m_XCoord == 99 && (*node)->m_YCoord == 99 )
                     {
                        invisible = true;
                        break;
                     }

                  }
               }

               //  No edge of map link?  Check for regular links.
               vector<MapLink*>::iterator node = m_MapLinks.begin();
               for(node; node != m_MapLinks.end(); ++node)
               {
                  if( (*node)->m_XCoord == x && (*node)->m_YCoord == y )
                  {
                     invisible = true;
                     break;
                  }
               }

               if( invisible )
                  DrawBoxAt( g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize), g_TileSize - 1, g_TileSize, 255, 255, 0 );

               bool linktarget = false;
               //  Check if this square is the target of another map link.
               for(int i = 0; i < 12; ++i)
               {
                  vector<MapLink*>::iterator node = g_AllMaps[i]->m_MapLinks.begin();
                  for(node; node != g_AllMaps[i]->m_MapLinks.end(); ++node)
                  {
                     if( (*node)->m_DestinationMap == g_AllMaps[g_CurrentEditorMap]->m_MapName && (*node)->m_XDest == x && (*node)->m_YDest == y )
                     {
                        linktarget = true;
                        break;
                     }
                  }
               }

               if( linktarget )
                  DrawBoxAt( g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize), g_TileSize - 1, g_TileSize, 255, 0, 0 );
                  */


}

void Map::Update()
{

}


void Map::Draw()
{
   static uint32_t scrolltimer = static_cast<uint32_t>(g_Engine->GameTimeInMS());
   static int scroller = g_TileSize;

   if( static_cast<uint32_t>(g_Engine->GameTimeInMS()) - scrolltimer > 100 )
   {
      --scroller;
      scrolltimer = static_cast<uint32_t>(g_Engine->GameTimeInMS());
   }

   if( scroller == 0 )
      scroller = g_TileSize;

   if( m_Peer )
   {
      DrawPeer();
      return;
   }

   if( g_StateMachine->GetCurrentState() == STATE_EDITORSTATE )
   {
      DrawEditor();
      return;
   }

   //  The map is drawn in several "passes" in order to shade things so that they look more 3D-ish.
   int x, y, adjustedx, adjustedy;


   //  MAIN RENDERING
   for(x = g_Player->m_PlayerPosX - g_ViewRange; x <= g_Player->m_PlayerPosX + g_ViewRange; ++x)
   {
      if ( x < 0 || x >= g_Map->m_Width )
         continue;

      for(y = g_Player->m_PlayerPosY - g_ViewRange; y <= g_Player->m_PlayerPosY + g_ViewRange; ++y)
      {
         if ( y < 0 || y >= g_Map->m_Height )
            continue;

         if( g_Map->m_MapTitle != "Inaria" &&
            g_Map->m_MapTitle != "Inaria Special" &&
            g_Map->m_MapTitle != "The Mountain Trial" &&
            !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, x, y, false) )
         {
            continue;
         }


         if(x >= 0 && x < m_Width && y >= 0 && y < m_Height)
         {
            adjustedx = x - (g_Player->m_PlayerPosX - g_ViewRange);
            adjustedy = y - (g_Player->m_PlayerPosY - g_ViewRange);

            //  Draw water and lava with scrolling animation.
            if( m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 2 || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 7 )
            {
               DrawSpriteRectAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], 0,              scroller, g_TileSize, g_TileSize - scroller, g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
               DrawSpriteRectAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], 0,                     0, g_TileSize,              scroller, g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) + (g_TileSize - scroller) );

            }
            else if( isGrassType( m_map[x][y].m_TerrainType ) )
            {
               int beachCounter = 0;
               //  Draw a beach tile if necessary

               if( y - 1 >= 0  && m_map[x][y - 1].m_TerrainType != 2 )
                  beachCounter += 1;
               if( x + 1 < m_Width && m_map[x + 1][y].m_TerrainType != 2 )
                  beachCounter += 2;
               if( y + 1 < m_Height && m_map[x][y + 1].m_TerrainType != 2 )
                  beachCounter += 4;
               if( x - 1 >= 0  && m_map[x - 1][y].m_TerrainType != 2 )
                  beachCounter += 8;

               if( beachCounter == 15 ) // no beach
               {
                  DrawSpriteAt( g_Tiles[0], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
                  DrawSpriteAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
               }
               else
               {
                  DrawSpriteRectAt( g_Tiles[2], 0,              scroller, g_TileSize, g_TileSize - scroller, g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
                  DrawSpriteRectAt( g_Tiles[2], 0,                     0, g_TileSize,              scroller, g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) + (g_TileSize - scroller) );
                  DrawSpriteAt( g_Tiles[82 + beachCounter], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );

                  if( m_map[x][y].m_TerrainType != 0 )
                  {
                     DrawSpriteAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
                  }
               }
            }
            else if( isPavedType( m_terrainTypes[m_map[x][y].m_TerrainType].m_tile) )
            {
               DrawSpriteAt( g_Tiles[5], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
               DrawSpriteAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize) );
            }
            else
            {
/*               glPushMatrix();
               glMatrixMode(GL_MODELVIEW_MATRIX);
               glColor3f(1, 1, 1);
               glTranslatef(g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize), -100);
               glBindTexture(GL_TEXTURE_2D, g_Tiles[0]->m_Texture->m_Texture);
               glBegin(GL_TRIANGLES);
               void* currentmesh = m_terrainTypes[m_map[x][y].m_TerrainType].m_Mesh;
               for( int i = 0; i < currentmesh->m_VertexList.size(); ++i )
               {
                  
                  glVertex3f( currentmesh->m_VertexList[i].pos.x, currentmesh->m_VertexList[i].pos.y, currentmesh->m_VertexList[i].pos.z);
               }
               glEnd();
               glPopMatrix();*/
               DrawSpriteAt( g_Tiles[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
            }
         }
      }
   }

   //  WALL RENDERING
   for(x = g_Player->m_PlayerPosX - g_ViewRange; x <= g_Player->m_PlayerPosX + g_ViewRange; ++x)
   {
      if ( x < 0 || x >= g_Map->m_Width )
         continue;

      for(y = g_Player->m_PlayerPosY - g_ViewRange; y <= g_Player->m_PlayerPosY + g_ViewRange; ++y)
      {
         if ( y < 0 || y >= g_Map->m_Height )
            continue;

         if( g_Map->m_MapTitle != "Inaria" && g_Map->m_MapTitle != "Inaria Special" && g_Map->m_MapTitle != "The Mountain Trial" && !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, x, y, false) )
         {
            continue;
         }

         adjustedx = x - (g_Player->m_PlayerPosX - g_ViewRange);
         adjustedy = y - (g_Player->m_PlayerPosY - g_ViewRange);

         int xdec = max(x - 1,0);
         int ydec = max(y - 1,0);
         int xinc = min(x + 1,m_Width - 1);
         int yinc = min(y + 1,m_Height - 1);

         if (m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 14 || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 15 || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 47)
            continue;

         bool bIsPavement = isPavement(m_terrainTypes[m_map[x][y].m_TerrainType].m_tile);

         bool bIsGrassTile = isGrassType(m_terrainTypes[m_map[x][y].m_TerrainType].m_tile);
         bool bIsBeachTile =  bIsGrassTile && (m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[xinc][y].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[x][yinc].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[xinc][ydec].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[xinc][yinc].m_TerrainType].m_tile == 2 ||
            m_terrainTypes[m_map[xdec][yinc].m_TerrainType].m_tile == 2                                                  
            );         

         bool bWallType = isWallType(m_terrainTypes[m_map[x][y].m_TerrainType].m_tile);            
         bool bWallOrLava = bWallType || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 7;// || m_terrainTypes[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 5;// || m_terrainTypes[m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 11;// || bIsGrassTile;

         bool NEdge = y == ydec || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile    && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile == 2));
         bool SEdge = y == yinc || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[x][yinc].m_TerrainType].m_tile    && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[x][yinc].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[x][yinc].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[x][yinc].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[x][yinc].m_TerrainType].m_tile == 2));
         bool WEdge = x == xdec || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile    && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile == 2));
         bool EEdge = x == xinc || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xinc][y].m_TerrainType].m_tile    && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[xinc][y].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[xinc][y].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[xinc][y].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[xinc][y].m_TerrainType].m_tile == 2));

         bool NWEdge =             m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile == 2));
         bool SWEdge =             m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xdec][yinc].m_TerrainType].m_tile && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[xdec][yinc].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[xdec][yinc].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[xdec][yinc].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[xdec][yinc].m_TerrainType].m_tile == 2));
         bool NEEdge =             m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xinc][ydec].m_TerrainType].m_tile && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[xinc][ydec].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[xinc][ydec].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[xinc][ydec].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[xinc][ydec].m_TerrainType].m_tile == 2));
         bool SEEdge =             m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xinc][yinc].m_TerrainType].m_tile && ((bIsGrassTile && isPavement( m_terrainTypes[m_map[xinc][yinc].m_TerrainType].m_tile)) || bWallOrLava || m_terrainTypes[m_map[xinc][yinc].m_TerrainType].m_tile == 7 || m_terrainTypes[m_map[xinc][yinc].m_TerrainType].m_tile == 47 || (!bIsBeachTile && m_terrainTypes[m_map[xinc][yinc].m_TerrainType].m_tile == 2));

         if (NEEdge && !NEdge && !EEdge)
         {
            DrawImageAt( g_WallShadows[2], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         if (NWEdge && !NEdge && !WEdge)
         {
            DrawImageAt( g_WallShadows[3], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         if (SEEdge && !SEdge && !EEdge)
         {
            DrawImageAt( g_WallShadows[5], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         if (SWEdge && !SEdge && !WEdge)
         {
            DrawImageAt( g_WallShadows[6], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }            
         if (NEdge)
         {
            DrawImageAt( g_WallShadows[1], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         if (SEdge)
         {
            DrawImageAt( g_WallShadows[4], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         if (EEdge)
         {
            DrawImageAt( g_WallShadows[0], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         if (WEdge)
         {
            DrawImageAt( g_WallShadows[7], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
      }
   }

   // FLOOR RENDERING

   for(x = g_Player->m_PlayerPosX - g_ViewRange; x <= g_Player->m_PlayerPosX + g_ViewRange; ++x)
   {
      if ( x < 0 || x >= g_Map->m_Width )
         continue;

      for(y = g_Player->m_PlayerPosY - g_ViewRange; y <= g_Player->m_PlayerPosY + g_ViewRange; ++y)
      {
         if ( y < 0 || y >= g_Map->m_Height )
            continue;

         if( g_Map->m_MapTitle != "Inaria" && g_Map->m_MapTitle != "Inaria Special" && g_Map->m_MapTitle != "The Mountain Trial" && !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, x, y, false) )
         {
            continue;
         }

         adjustedx = x - (g_Player->m_PlayerPosX - g_ViewRange);
         adjustedy = y - (g_Player->m_PlayerPosY - g_ViewRange);

         int xdec = max(x - 1,0);
         int ydec = max(y - 1,0);
         bool bWallType = isWallType(m_terrainTypes[m_map[x][y].m_TerrainType].m_tile);
         if (bWallType) continue;

         bool NWallType = isWallType(m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile);
         bool WWallType = isWallType(m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile);
         bool NWWallType = isWallType(m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile);

         bool SShadow = m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[x][ydec].m_TerrainType].m_tile && (NWallType || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 7);                
         bool EShadow = m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xdec][y].m_TerrainType].m_tile && (WWallType || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 7);
         bool CornerShadow1 = m_terrainTypes[m_map[x][y].m_TerrainType].m_tile != m_terrainTypes[m_map[xdec][ydec].m_TerrainType].m_tile && (NWWallType|| m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 7);
         bool CornerShadow2 = SShadow && EShadow;
         bool CornerShadow3 = SShadow && !NWWallType;
         bool CornerShadow4 = EShadow && !NWWallType;

         int alpha = (m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 14 || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 15 || m_terrainTypes[m_map[x][y].m_TerrainType].m_tile == 47) ? 255 : 200;

         if (CornerShadow2)
         {

            DrawImageAt( g_FloorShadows[1], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         else if (CornerShadow3)
         {
            DrawImageAt( g_FloorShadows[2], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         else if (CornerShadow4)
         {
            DrawImageAt( g_FloorShadows[3], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         else if (SShadow)
         {
            DrawImageAt( g_FloorShadows[5], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         else if (EShadow)
         {
            DrawImageAt( g_FloorShadows[4], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
         else if (CornerShadow1)
         {
            DrawImageAt( g_FloorShadows[0], g_Offset + (adjustedx * g_TileSize), g_Offset + (adjustedy * g_TileSize));
         }
      }
   }

   list<Item*>::iterator itemnode;
   for(itemnode = g_Map->m_ItemList.begin(); itemnode != g_Map->m_ItemList.end(); ++itemnode)
   {
      if( g_Map->m_MapTitle != "Inaria" && g_Map->m_MapTitle != "Inaria Special" && g_Map->m_MapTitle != "The Mountain Trial" && !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY,(*itemnode)->m_PosX, (*itemnode)->m_PosY, false) )
      {
         continue;
      }
      (*itemnode)->Draw();
   }

   //  Draw the unit list.
   list<NPC*>::iterator node;
   for( node = g_Map->m_NPCList.begin(); node != g_Map->m_NPCList.end(); ++node )
   {
      if( g_Map->m_MapTitle != "Inaria" && g_Map->m_MapTitle != "Inaria Special" && g_Map->m_MapTitle != "The Mountain Trial" && !CheckPath(g_Player->m_PlayerPosX, g_Player->m_PlayerPosY,(*node)->m_PosX, (*node)->m_PosY, false) )
      {
         continue;
      }

      (*node)->Draw();
   }
}


void Map::InitializeTerrainTypes()
{
   int i;
   for(i = 0; i < 64; ++i)
   {
      m_terrainTypes[i].m_tile = i;
      m_terrainTypes[i].m_passable = true;
      m_terrainTypes[i].m_Mesh = nullptr; // mesh rendering disabled
   }

   m_terrainTypes[1].m_passable = false;
   m_terrainTypes[2].m_passable = false;
   m_terrainTypes[3].m_passable = false;
   m_terrainTypes[4].m_passable = false;
   m_terrainTypes[7].m_passable = false;
   m_terrainTypes[9].m_passable = false;
   m_terrainTypes[10].m_passable = false;
   m_terrainTypes[18].m_passable = false;
   m_terrainTypes[19].m_passable = false;
   m_terrainTypes[20].m_passable = false;



   m_terrainTypes[16].m_tile = 100;
   m_terrainTypes[17].m_tile = 101;
   m_terrainTypes[18].m_tile = 102;
   m_terrainTypes[19].m_tile = 103;
   m_terrainTypes[20].m_tile = 104;
   m_terrainTypes[21].m_tile = 47;

   int x, y;
   for(x = 0; x < m_Width; ++x)
   {
      for(y = 0; y < m_Height; ++y)
      {
         // This code was reading random memory before, for tile type 47.  Passibility was random.
         m_map[x][y].m_passable = m_terrainTypes[m_map[x][y].m_TerrainType].m_passable;
      }
   }

   m_terrainTypes[0].m_Name = "Grass";
   m_terrainTypes[1].m_Name = "Large Tree";
   m_terrainTypes[2].m_Name = "Water";
   m_terrainTypes[3].m_Name = "Mountain";
   m_terrainTypes[4].m_Name = "Stone Wall";
   m_terrainTypes[5].m_Name = "Brick Floor";
   m_terrainTypes[6].m_Name = "Small Trees";
   m_terrainTypes[7].m_Name = "Lava";
   m_terrainTypes[8].m_Name = "Dungeon Entrance";
   m_terrainTypes[9].m_Name = "Counter";
   m_terrainTypes[10].m_Name = "Wood Wall";
   m_terrainTypes[11].m_Name = "Wood Floor";
   m_terrainTypes[12].m_Name = "Town";
   m_terrainTypes[13].m_Name = "Castle";
   m_terrainTypes[14].m_Name = "Ladder Up";
   m_terrainTypes[15].m_Name = "Ladder Down";
   m_terrainTypes[16].m_Name = "Farmland";
   m_terrainTypes[17].m_Name = "Mushrooms";
   m_terrainTypes[18].m_Name = "Well";
   m_terrainTypes[19].m_Name = "Rocks";
   m_terrainTypes[20].m_Name = "Throne";
   m_terrainTypes[21].m_Name = "Black Floor";

   m_terrainTypes[0].m_MinimapColor =  Color{   0, 128,   0, 255 };
   m_terrainTypes[1].m_MinimapColor =  Color{   0,  96,   0, 255 };
   m_terrainTypes[2].m_MinimapColor =  Color{ 128, 128, 255, 255 };
   m_terrainTypes[3].m_MinimapColor =  Color{ 128, 128, 128, 255 };
   m_terrainTypes[4].m_MinimapColor =  Color{ 128, 128, 128, 255 };
   m_terrainTypes[5].m_MinimapColor =  Color{ 255, 128, 128, 255 };
   m_terrainTypes[6].m_MinimapColor =  Color{   0, 192,   0, 255 };
   m_terrainTypes[7].m_MinimapColor =  Color{ 255, 128,   0, 255 };
   m_terrainTypes[8].m_MinimapColor =  Color{ 192, 192, 192, 255 };
   m_terrainTypes[9].m_MinimapColor =  Color{  97,  21,   0, 255 };
   m_terrainTypes[10].m_MinimapColor = Color{ 101,  64,   0, 255 };
   m_terrainTypes[11].m_MinimapColor = Color{ 166, 113,  61, 255 };
   m_terrainTypes[12].m_MinimapColor = Color{ 255, 255, 255, 255 };
   m_terrainTypes[13].m_MinimapColor = Color{ 255, 255, 255, 255 };
   m_terrainTypes[14].m_MinimapColor = Color{ 255, 255, 255, 255 };
   m_terrainTypes[15].m_MinimapColor = Color{ 255, 255, 255, 255 };
   m_terrainTypes[16].m_MinimapColor = Color{ 255, 128, 128, 255 };
   m_terrainTypes[17].m_MinimapColor = Color{   0, 128,  32, 255 };
   m_terrainTypes[18].m_MinimapColor = Color{ 0, 0, 0, 255 };
   m_terrainTypes[19].m_MinimapColor = Color{ 128, 128, 128, 255 };
   m_terrainTypes[20].m_MinimapColor = Color{ 255, 128, 255, 255 };
   m_terrainTypes[21].m_MinimapColor = Color{ 0, 0, 0, 255 };

}

void Map::ClearMap()
{
   memset(m_map, 0, 65536 * sizeof(MapCell));

   int i, j;
   for(i = 0; i < m_Width; ++i)
   {
      for(j = 0; j < m_Height; ++j)
      {
         m_map[i][j].m_TerrainType = 16;
      }
   }
}

//  Bog standard:  Open a file, read in 4096 map cells, close file.
bool Map::LoadMap(const char *mapname)
{
   //    g_SmoothScrollX = 0;
   //    g_SmoothScrollY = 0;
   m_MapName = mapname;
   ifstream instream;
   instream.open(mapname);
   if( instream.fail() )
      return false;

   getline(instream, m_MapTitle);

   if( m_MapTitle == "The Infinite" )
   {
      if( g_InfiniteYes == 1 )
         return true;
   }

   instream >> m_Width;
   instream >> m_Height;
   instream >> m_MapLevel;

   char buffer[64];

   int i, j, currentTerrainType;
   for(i = 0; i < m_Width; ++i)
   {
      for(j = 0; j < m_Height; ++j)
      {
         instream >> m_map[i][j].m_TerrainType;

/*         instream >> currentTerrainType;
         for( int k = 0; k < 4; ++k )
         {
            for( int l = 0; l < 4; ++l )
            {
               m_map[i * 4 + k][j * 4 + l].m_TerrainType = currentTerrainType;
            }
         }*/
      }
   }

   /*   for(i = 0; i < 64; ++i)
   {
   for(j = 0; j < 64; ++j)
   {
   m_MapCells[i][j].m_ShapeSpring.reset();
   }
   }*/

   //  Load in the itemlist
   m_ItemList.clear();
   bool donewithitems = false;

   int itemtype;
   while(!donewithitems)
   {
      instream >> itemtype;

      if(itemtype == 9999) //  9999 is a flag that means "no more items"
      {
         donewithitems = true;
      }
      else
      {
         Item *temp = ItemFactory(itemtype);

         int xPos, yPos, Targetx, Targety;
         instream >> xPos;
         instream >> yPos;
         instream >> Targetx;
         instream >> Targety;

         if (temp)
         {
            temp->m_PosX = xPos;
            temp->m_PosY = yPos;
            temp->m_TargetX = Targetx;
            temp->m_TargetY = Targety;
            //temp->m_renderPosX = temp->m_PosX;              
            //temp->m_renderPosY = temp->m_PosY = yPos;
            m_ItemList.push_back(temp);
         }
      }
   }

   //  Load in the NPC list

   m_NPCList.clear();

   bool donewithNPCs = false;

   int type;
   while(!donewithNPCs)
   {
      instream >> type;

      if(type == 5555) //  5555 is a flag that means "no more NPCs"
      {
         donewithNPCs = true;
      }
      else
      {
         NPC *temp = UnitFactory(type);

         int xPos, yPos;
         instream >> xPos;
         instream >> yPos;          

         if (temp)
         {
            int effectivelevel = m_MapLevel;
            //            if (effectivelevel < 1) effectivelevel = 1;
            //            if (effectivelevel > 8) effectivelevel = 8;

            temp->m_PosX = xPos;
            temp->m_PosY = yPos;

            //  First, find out how many points we have to work with.
            int maxpoints = effectivelevel * 5;
            int points = HalfNHalf(maxpoints);

            temp->m_Strength = effectivelevel;
            temp->m_Dexterity = effectivelevel;
            temp->m_Endurance = effectivelevel;
            temp->m_Intelligence = effectivelevel;
            temp->m_Will = effectivelevel;

            while ( points > 0 )
            {
               //  Pick a random stat to improve
               switch( g_RNG->Random(5) )
               {
               case 0:
                  temp->m_Strength += 1;
                  break;

               case 1:
                  temp->m_Dexterity += 1;
                  break;

               case 2:
                  temp->m_Intelligence += 1;
                  break;

               case 3:
                  temp->m_Endurance += 1;
                  break;

               case 4:
                  temp->m_Will += 1;
                  break;
               }
               points -= 1;
            }

            temp->m_MaxHitPoints = ( temp->m_Endurance + Factorial( m_MapLevel ) );
            temp->m_HitPoints = temp->m_MaxHitPoints;

            m_NPCList.push_back(temp);
         }
      }
   }

   //  Load map links
   m_MapLinks.clear();

   bool donewithmaplinks = false;
   while(!donewithmaplinks)
   {
      instream >> type;

      if(type == 4444) //  5555 is a flag that means "no more NPCs"
      {
         donewithmaplinks = true;
      }
      else
      {
         MapLink *tempmaplink = new MapLink;
         tempmaplink->m_XCoord = type;
         instream >> tempmaplink->m_YCoord;
         instream >> tempmaplink->m_XDest;
         instream >> tempmaplink->m_YDest;
         instream >> tempmaplink->m_DestinationMap;

         m_MapLinks.push_back(tempmaplink);
      }
   }


   instream.close();

   InitializeTerrainTypes();

   if( m_MapTitle == "The Infinite" )
      CreateRoguelikeMap();

   return true;
}

void Map::SaveMap(const char *mapname)
{
   int stopper = 0;

   ofstream ostream;
   ostream.open(mapname);

   ostream << m_MapTitle << endl;

   ostream << m_Width << endl;
   ostream << m_Height << endl;
   ostream << m_MapLevel << endl;


   int i, j;
   for(i = 0; i < m_Width; ++i)
   {
      for(j = 0; j < m_Height; ++j)
      {
         ostream.width(3);
         ostream << m_map[i][j].m_TerrainType;
      }
      ostream << endl;
   }

   ostream << endl;

   for(list<Item*>::iterator node = m_ItemList.begin(); node != m_ItemList.end(); ++node)
   {
      ostream << (*node)->m_ItemType << " " << (*node)->m_PosX << " " << (*node)->m_PosY << " " << (*node)->m_TargetX << " " << (*node)->m_TargetY << endl;
   }
   ostream << "9999" << endl;


   for(list<NPC*>::iterator node = m_NPCList.begin(); node != m_NPCList.end(); ++node)
   {
      ostream << (*node)->m_Type << " " << (*node)->m_PosX << " " << (*node)->m_PosY << endl;
   }
   ostream << "5555" << endl;

   for(vector<MapLink*>::iterator node = m_MapLinks.begin(); node != m_MapLinks.end(); ++node)
   {
      ostream << (*node)->m_XCoord << " " << (*node)->m_YCoord << " " << (*node)->m_XDest << " " << (*node)->m_YDest << " " << (*node)->m_DestinationMap << endl;
   }

   ostream << "4444" << endl;

   ostream.close();
}

void Map::CheckMapLinks()
{
   //  First, check for the "edge of map" link
   if(g_Player->m_PlayerPosX == 0 || g_Player->m_PlayerPosX == 63 ||
      g_Player->m_PlayerPosY == 0 || g_Player->m_PlayerPosY == 63)
   {
      //  Scan the list for a map link with coordinates 99, 99
      vector<MapLink*>::iterator node = m_MapLinks.begin();
      for(node; node != m_MapLinks.end(); ++node)
      {
         if( (*node)->m_XCoord == 99 && (*node)->m_YCoord == 99 )
            break;
      }

      if(node != m_MapLinks.end())
      {

         if( (*node)->m_DestinationMap == "maps/inaria.map" && g_Player->m_KilledEvilOne )
            LoadMap("maps/intro.map");
         else if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE && (*node)->m_DestinationMap == "maps/thecube.map" && g_InfiniteYes == 0 )
         {
            g_StateMachine->PushState(STATE_INFINITEWARNINGSTATE);
            return;
         }
         else
         {
            g_InfiniteYes = 0;
            LoadMap((*node)->m_DestinationMap.c_str());
         }

         g_Player->m_PlayerPosX = (*node)->m_XDest;
         g_Player->m_PlayerPosY = (*node)->m_YDest;
         string name = "Entering " + g_Map->m_MapTitle + "...";
         AddConsoleString(name);
         if( (*node)->m_DestinationMap == "maps/thecube.map" )
         {
            AddConsoleString("Enjoy your stay.");
         }
         else
         {
            stringstream maplevel;
            maplevel.str("");
            maplevel << "Map Level: " << g_Map->m_MapLevel;
            AddConsoleString(maplevel.str());
         }
      }
   }

   //  No edge of map link?  Check for regular links.
   vector<MapLink*>::iterator node = m_MapLinks.begin();
   for(node; node != m_MapLinks.end(); ++node)
   {
      if( (*node)->m_XCoord == g_Player->m_PlayerPosX && (*node)->m_YCoord == g_Player->m_PlayerPosY )
         break;
   }

   if(node != m_MapLinks.end())
   {
      if( (*node)->m_DestinationMap != g_Map->m_MapName )
      {
         if( (*node)->m_DestinationMap == "maps/inaria.map" && g_Player->m_KilledEvilOne )
            LoadMap("maps/intro.map");
         else if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE && (*node)->m_DestinationMap == "maps/thecube.map" && g_InfiniteYes == 0 )
         {
            g_StateMachine->PushState(STATE_INFINITEWARNINGSTATE);
            return;
         }
         else if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE && (*node)->m_DestinationMap == "maps/thecube.map" && g_InfiniteYes == 1 )
         {
            return;
         }
         else if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE && (*node)->m_DestinationMap == "maps/thecube.map" && g_InfiniteYes == 2 )
         {
            g_Player->m_PlayerPosX = (*node)->m_XDest;
            g_Player->m_PlayerPosY = (*node)->m_YDest;
            LoadMap((*node)->m_DestinationMap.c_str());

            AddConsoleString("Entering The Infinite...");
            AddConsoleString("Enjoy your stay.");
            return;
         }
         else
         {
            g_Player->m_PlayerPosX = (*node)->m_XDest;
            g_Player->m_PlayerPosY = (*node)->m_YDest;
            LoadMap((*node)->m_DestinationMap.c_str());

            string name = "Entering " + g_Map->m_MapTitle + "...";
            AddConsoleString(name);
            stringstream maplevel;
            maplevel.str("");
            maplevel << "Map Level: " << g_Map->m_MapLevel;
            AddConsoleString(maplevel.str());
         }
      }
   }
   else
   {
      g_InfiniteYes = 0;
   }
}

int grid[64][64];
int grid2[64][64];
//  This function dumps whatever is on the map and creates a roguelike map instead.
void Map::CreateRoguelikeMap()
{
   m_Width = 64;
   m_Height = 64;

   m_NPCList.clear();
   m_ItemList.clear();
   m_MapLinks.clear();

   for( int i = 0; i < m_Width; ++i )
   {
      for( int j = 0; j < m_Height; ++j )
      {
         if( i == 0 || j == 0 || i == m_Width - 1 || j == m_Height - 1 )
         {
            grid[i][j] = 4;  //  wall
         }
         else
         {
            if( g_RNG->Random(100) < 40 )
               grid[i][j] = 4;  //  wall
            else
               grid[i][j] = 21;  //  Empty floor
         }
      }
   }

   int r1_cutoff = 5;
   int r2_cutoff = 2;
   //  Now do some smoothing to get a better map feel

   for( int smoothcounter = 0; smoothcounter < 3; ++smoothcounter )
   {
      int xi, yi, ii, jj;

      for(yi=1; yi<m_Height-1; yi++)
         for(xi=1; xi<m_Width-1; xi++)
         {
            int adjcount_r1 = 0,
               adjcount_r2 = 0;

            for(ii=-1; ii<=1; ii++)
               for(jj=-1; jj<=1; jj++)
               {
                  if(grid[yi+ii][xi+jj] != 21)
                     adjcount_r1++;
               }
               for(ii=yi-2; ii<=yi+2; ii++)
                  for(jj=xi-2; jj<=xi+2; jj++)
                  {
                     if(abs(ii-yi)==2 && abs(jj-xi)==2)
                        continue;
                     if(ii<0 || jj<0 || ii>=m_Height || jj>=m_Width)
                        continue;
                     if(grid[ii][jj] != 21)
                        adjcount_r2++;
                  }
                  if(adjcount_r1 >= r1_cutoff || adjcount_r2 <= r2_cutoff)
                     grid2[yi][xi] = 4;
                  else
                     grid2[yi][xi] = 21;
         }
         for(yi=1; yi<m_Height-1; yi++)
            for(xi=1; xi<m_Width-1; xi++)
               grid[yi][xi] = grid2[yi][xi];
   }

   //  Write the final grid into the map.
   for( int i = 0; i < m_Width; ++i )
   {
      for( int j = 0; j < m_Height; ++j )
      {
         m_map[i][j].m_TerrainType = grid[i][j];
         if( grid[i][j] == 4 )
            m_map[i][j].m_passable = false;
         else
            m_map[i][j].m_passable = true;
      }
   }

   //  Find a starting place for the player.
   int xPos = 0;
   int yPos = 0;
   do
   {
      xPos = g_RNG->Random(m_Width / 8);
      yPos = g_RNG->Random(m_Height / 8);
   } while( m_map[xPos][yPos].m_passable == false );

   g_Player->m_PlayerPosX = xPos;
   g_Player->m_PlayerPosY = yPos;

   //  Add monsters to the map.

   for( int i = 0; i < m_Width; ++i )
   {
      NPC *temp = UnitFactory(monsterTypes[g_RNG->Random(22)]);

      if (temp)
      {
         int effectivelevel = g_Player->m_Level + 2;
         //            if (effectivelevel < 1) effectivelevel = 1;
         //            if (effectivelevel > 8) effectivelevel = 8;

         int xPos = 0;
         int yPos = 0;

         xPos = g_RNG->Random(m_Width);
         yPos = g_RNG->Random(m_Height);

         while( m_map[xPos][yPos].m_passable == false || LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, xPos, yPos ) < 4)
         {
            xPos = g_RNG->Random(m_Width);
            yPos = g_RNG->Random(m_Height);
         }


         temp->m_PosX = xPos;
         temp->m_PosY = yPos;

         //  First, find out how many points we have to work with.
         int maxpoints = effectivelevel * 5;
         int points = HalfNHalf(maxpoints);

         temp->m_Strength = effectivelevel;
         temp->m_Dexterity = effectivelevel;
         temp->m_Endurance = effectivelevel;
         temp->m_Intelligence = effectivelevel;
         temp->m_Will = effectivelevel;

         while ( points > 0 )
         {
            //  Pick a random stat to improve
            switch( g_RNG->Random(5) )
            {
            case 0:
               temp->m_Strength += 1;
               break;

            case 1:
               temp->m_Dexterity += 1;
               break;

            case 2:
               temp->m_Intelligence += 1;
               break;

            case 3:
               temp->m_Endurance += 1;
               break;

            case 4:
               temp->m_Will += 1;
               break;
            }
            points -= 1;
         }

         temp->m_MaxHitPoints = ( temp->m_Endurance + Factorial( m_MapLevel ) );
         temp->m_HitPoints = temp->m_MaxHitPoints;

         m_NPCList.push_back(temp);

      }
   }


   //  Add items to the map.
   for( int i = 0; i < m_Width / 4; ++i )
   {

      Item *temp = ItemFactory(itemTypes[13]);

      int xPos = 0;
      int yPos = 0;
      int Targetx = 0;
      int Targety = 0;

      xPos = g_RNG->Random(m_Width);
      yPos = g_RNG->Random(m_Height);
      while( m_map[xPos][yPos].m_passable == false || LineLength( g_Player->m_PlayerPosX, g_Player->m_PlayerPosY, xPos, yPos ) < 4)
      {
         xPos = g_RNG->Random(m_Width);
         yPos = g_RNG->Random(m_Height);
      }

      temp->m_PosX = xPos;
      temp->m_PosY = yPos;

      if (temp)
      {
         temp->m_PosX = xPos;
         temp->m_PosY = yPos;
         temp->m_TargetX = Targetx;
         temp->m_TargetY = Targety;
         m_ItemList.push_back(temp);
      }

   }

   //  Add a map link back to the surface
   xPos = 0;
   yPos = 0;
   do
   {
      xPos = g_RNG->Random(m_Width / 8) + 56 ;
      yPos = g_RNG->Random(m_Height / 8) + 56;
   } while( m_map[xPos][yPos].m_passable == false );

   MapLink *tempmaplink = new MapLink;
   tempmaplink->m_XCoord = xPos;
   tempmaplink->m_YCoord = yPos;
   tempmaplink->m_XDest = 22;
   tempmaplink->m_YDest = 21;
   tempmaplink->m_DestinationMap = "maps/inaria.map" ;
   m_map[xPos][yPos].m_TerrainType = 14;

   m_MapLinks.push_back(tempmaplink);
}