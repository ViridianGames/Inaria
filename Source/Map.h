#ifndef _MAP_H_
#define _MAP_H_
#include "../Geist/Source/Object.h"
#include "raylib.h"
#include "Item.h"
#include "NPC.h"
#include <vector>
#include <list>
#include <string>

enum TerrainTypes
{
   TT_GRASS = 0,
   TT_TREE,
   TT_WATER,
   TT_MOUNTAIN,
   TT_STONEWALL,
   TT_BRICKFLOOR,
   TT_BUSHES,
   TT_LAVA,
   TT_DUNGEONENTRANCE,
   TT_COUNTER,
   TT_WOODWALL,
   TT_WOODFLOOR,
   TT_TOWN,
   TT_CASTLE,
   TT_LADDERUP,
   TT_LADDERDOWN,
   TT_DUNGEONFLOOR = 47,
   TT_SAND = 81
};

struct TerrainType
{
	int m_tile;       //  Tile that represents this terrain type
   void* m_Mesh;
	bool m_passable;  //  We're not using travel types, so this is either yes or no
   std::string m_Name;
   Color m_MinimapColor;
};

struct MapCell
{
   int  m_TerrainType;
   bool m_passable;
   int  m_link;      //  The number of the map to load when this tile is stepped on
};

struct MapLink
{
   std::string m_DestinationMap;
   int m_XCoord;
   int m_YCoord;
   int m_XDest;
   int m_YDest;
};


class Map : public Object
{
public:

   TerrainType m_terrainTypes[64];
   MapCell m_map[256][256];  //  Ah, the classic.
   int m_Width;
   int m_Height;

   std::string m_MapName;
   std::string m_MapTitle;
   std::vector<MapLink*> m_MapLinks;
   std::list<Item*> m_ItemList;
   std::list<NPC*> m_NPCList;

   Map();
   ~Map();

   void Init(const std::string& configfile);
   void Shutdown();
   void Update();
   void Draw();
   void DrawPeer();
   void DrawEditor();

   void InitializeTerrainTypes();
   void ClearMap();
   bool LoadMap(const char *mapname);
   void SaveMap(const char *mapname);
   void CheckMapLinks();
   void SetCellTerrainType( int x, int y, int terraintype );

   void CreateRoguelikeMap();

   int m_MapLevel;

   bool m_DrawInvisibles;

   bool m_Peer;

};

#endif