#ifndef _GAMEGLOBALS_H_
#define _GAMEGLOBALS_H_

#include <deque>
#include <list>
#include <string>
#include <vector>

#include "Geist/Globals.h"
#include "Geist/Primitives.h"
#include "Geist/RNG.h"
#include "Geist/StateMachine.h"
#include "InariaCompat.h"
#include "InariaEffect.h"
#include "NPC.h"
#include "Map.h"
#include "Player.h"
#include "Item.h"
#include "Trigger.h"

#include "EditorState.h"
#include "EnchantingState.h"
#include "InfiniteWarningState.h"
#include "LevelUpState.h"
#include "MainState.h"
#include "OptionsState.h"
#include "StoreState.h"
#include "TitleState.h"
#include "TutorialState.h"

#define NUMBER_OF_TILES 256
#define NUMBER_OF_CURSORS 10
#define NUMBER_OF_MUSIC_TRACKS 12

extern std::string g_HappyResponses[30];

extern std::list<InariaEffect*> g_AnimationList;
extern Player* g_Player;
extern Map* g_Map;
extern Map* g_AllMaps[14];
extern int g_CurrentEditorMap;
extern LegacySprite* g_Tiles[NUMBER_OF_TILES];
extern Texture* g_Mask;
extern LegacySprite* g_Star;
extern LegacySprite* g_Bubble;
extern Texture* g_WallShadows[8];
extern Texture* g_FloorShadows[6];
extern std::deque<ColoredString> g_ConsoleStrings;
extern Texture* g_Cursors[NUMBER_OF_CURSORS];

extern Texture* g_TextBackground;
extern Texture* g_TBUL;
extern Texture* g_TBUR;
extern Texture* g_TBLR;
extern Texture* g_TBLL;
extern Texture* g_TalkSpurLeft;
extern Texture* g_TalkSpurRight;
extern Texture* g_TalkSpurRightUp;

extern bool g_IsPlayerTurn;

extern int g_TileSize;
extern int g_Offset;
extern int g_ViewRange;

extern int g_InventoryX;
extern int g_InventoryY;
extern int g_InventoryTileSize;

extern bool g_IsFullscreen;

extern std::string g_Version;

extern RNG* g_RNG;

void AddConsoleString(std::string str, int r = 255, int g = 255, int b = 255);
void DrawConsoleStrings();

std::vector<ColoredString> ConstructItemTooltip(Item* item);
std::vector<ColoredString> ConstructAbilityTooltip(int index);

bool DrawButton(std::string text, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);
bool DrawButtonCentered(std::string text, int x, int y, int r = 255, int g = 255, int b = 255, int a = 255);

bool DoRPGCheck(int doer, int stopper);

Item* ItemFactory(int itemtype, bool savegame = false);
NPC* UnitFactory(int type, bool savegame = false);

bool SaveGame();
bool LoadGame();

extern bool g_ShowBackstory;

enum States
{
    STATE_MAINSTATE = 0,
    STATE_LEVELUPSTATE,
    STATE_STORESTATE,
    STATE_OPTIONSSTATE,
    STATE_TITLESTATE,
    STATE_EDITORSTATE,
    STATE_TUTORIALSTATE,
    STATE_ENCHANTERSTATE,
    STATE_INFINITEWARNINGSTATE,
    STATE_LASTSTATE
};

enum SpecialAbilities
{
    ABILITY_SMITE = 0,
    ABILITY_MAGICARMOR,
    ABILITY_DAMAGEBONUS,
    ABILITY_HEAL,
    ABILITY_MAGICUNLOCK,
    ABILITY_MAP,
    ABILITY_TELEPORT,
    ABILITY_TOWNPORTAL,
    ABILITY_CLEAVE,
    ABILITY_BASH,
    ABILITY_PICKLOCK,
    ABILITY_SNIPER,
    ABILITY_POISON,
    ABILITY_IRONWILL,
    ABILITY_TANK,
    ABILITY_HARDY
};

enum ItemTypes
{
    ITEM_LEATHERARMOR = 0,
    ITEM_CHAINARMOR,
    ITEM_PLATEARMOR,
    ITEM_VIRIDIANARMOR,
    ITEM_TELEPORTER,
    ITEM_STEPSWITCH,
    ITEM_SHOOTSWITCH,
    ITEM_SLEEPFIELD,
    ITEM_BOW,
    ITEM_DAGGER,
    ITEM_SWORD,
    ITEM_RIGHTEOUSSWORD,
    ITEM_WARHAMMER,
    ITEM_CHEST,
    ITEM_COIN,
    ITEM_SECRETDOOR,
    ITEM_POISONFIELD,
    ITEM_MANAPOTION,
    ITEM_LOCKEDDOOR,
    ITEM_HEALTHPOTION,
    ITEM_FIREFIELD,
    ITEM_ELECTRICFIELD,
    ITEM_CLOTHARMOR,
    ITEM_RING,
    ITEM_NECKLACE,
    ITEM_TRANSFORMATIONFIELD
};

enum Equippables
{
    EQUIPPABLE_WEAPON = 1,
    EQUIPPABLE_ARMOR,
    EQUIPPABLE_TRINKET
};

enum GuiStates
{
    GUI_NOSTATE = 0,
    GUI_GET,
    GUI_FIGHT,
    GUI_TALK,
    GUI_LOOK,
    GUI_LOAD,
    GUI_SAVE,
    GUI_QUIT,
    GUI_DAMAGESPELL,
    GUI_ARMORSPELL,
    GUI_SMITESPELL,
    GUI_HEALSPELL,
    GUI_MAGICUNLOCK,
    GUI_MAGICMAP,
    GUI_TELEPORT,
    GUI_TOWNPORTAL,
    GUI_BASH,
    GUI_PICKLOCK,
    GUI_NOMORESTATES
};

enum MusicTracks
{
    MUSIC_CASTLE = 0,
    MUSIC_DANCEPARTY,
    MUSIC_DUNGEON,
    MUSIC_EMERALD,
    MUSIC_LASTFRONT,
    MUSIC_OVERWORLD,
    MUSIC_RUBY,
    MUSIC_SAPPHIRE,
    MUSIC_SHOPPING,
    MUSIC_THEME,
    MUSIC_VOLCANOHEART,
    MUSIC_WIN
};

struct SpecialAbilityData
{
    int m_ID;
    int m_SkillPointCost;
    int m_ManaCost;
    int m_Tile;
    std::string m_Name;
    std::string m_Description;
    std::string m_ShortDescription;
    int m_Duration;
    int m_Level;
    bool m_Active;
};

extern SpecialAbilityData g_SpecialAbilities[16];

float LineLength(float x, float y, float endx, float endy);
bool CheckPath(int x1, int y1, int x2, int y2, bool careaboutnpcs = true, bool careaboutitems = true);

void PlayMusic();

int Factorial(int start);

extern std::string g_MusicTracks[NUMBER_OF_MUSIC_TRACKS];
extern int g_CurrentMusic;

int HalfNHalf(int range);

extern bool g_DanceParty;
extern bool g_FirstTime;
extern int g_InfiniteYes;
extern int g_CheckPathCount;
extern int g_LineLengthCount;

void DrawToolTip(InariaFont* font, std::vector<ColoredString> strings, int x, int y, int anchorcorner = 0);
void DrawToolTip(InariaFont* font, std::string strings, int x, int y, int anchorcorner = 0, int r = 255, int g = 255, int b = 255);
void DrawToolTip(InariaFont* font, std::vector<ColoredString> strings, int x, int y, int linewidth, int anchorcorner);
void DrawToolTip(InariaFont* font, std::string strings, int x, int y, int linewidth, int anchorcorner, int r, int g, int b);

#endif