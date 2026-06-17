#include "GameGlobals.h"
#include <sstream>
#include <assert.h>
#include <fstream>

using namespace std;

std::list<InariaEffect*> g_AnimationList;
Player* g_Player;
Map* g_Map;
Map* g_AllMaps[14];
int g_CurrentEditorMap;
Sprite* g_Tiles[NUMBER_OF_TILES];
Texture* g_Mask;
Sprite* g_Star;
Sprite* g_Bubble;
Texture* g_WallShadows[8];
Texture* g_FloorShadows[6];
deque<ColoredString> g_ConsoleStrings;
Texture* g_Cursors[NUMBER_OF_CURSORS];
bool g_IsPlayerTurn;
bool g_IsFullscreen;

bool g_DanceParty = false;

bool g_ShowBackstory = false;

string g_Version = "Version 1.2";

int g_TileSize;
int g_Offset;
int g_ViewRange;

int g_InventoryX;
int g_InventoryY;
int g_InventoryTileSize;

int g_CheckPathCount;
int g_LineLengthCount;

RNG* g_RNG;

int g_InfiniteYes;

SpecialAbilityData g_SpecialAbilities[16] = 
{
   { ABILITY_SMITE,        2, 2,  30, "Smite", "Hurl a ball of destructive energy at foes.", "Magic Ranged Attack", 0, 0, false },
   { ABILITY_MAGICARMOR,   3, 3,  29, "Enchant Armor", "Enchant your armor so that it protects better for 10 turns.", "Enhanced armor for 10 turns.", 0, 0, false },
   { ABILITY_DAMAGEBONUS,  4, 4,  28, "Enchant Weapon", "Enchant your weapon so that it does extra damage for 10 turns.","Enhanced damage for 10 turns.", 0, 0, false },
   { ABILITY_HEAL,         5, 5,  31, "Heal", "Heal yourself.", "Heal yourself.", 0, 0, false },
   { ABILITY_MAGICUNLOCK,  6, 6, 111, "Magic Unlock", "Magically unlock a door or disarm a trap.", "Remove a locked door or trap.", 0, 0, false },
   { ABILITY_MAP,          7, 7, 112, "Peer", "Look at the entire level and everything in it.", "Show level map.", 0, 0, false },
   { ABILITY_TELEPORT,     8, 8, 113, "Teleport", "Teleport to somewhere else in the level.", "Teleport in line-of-sight.", 0, 0, false },
   { ABILITY_TOWNPORTAL,   9, 9, 114, "Retreat", "Teleport back to the town of Emerald.", "Retreat to Emerald.", 0, 0, false },
   { ABILITY_CLEAVE,       3, 0, 115, "Cleave", "Passive: Every melee attack may now hit multiple enemies.", "Passive: Multi-foe melee hit.", 0, 0, false },
   { ABILITY_BASH,         5, 0, 116, "Bash", "Attempt to break down a locked door.", "Bash open a locked door.", 0, 0, false },
   { ABILITY_PICKLOCK,     3, 0, 117, "Lockpicking", "Attempt to unlock a door or disarm a trap.", "Remove a locked door or trap.", 0, 0, false },
   { ABILITY_SNIPER,       5, 0, 131, "Sniper", "Passive: Greatly increases your critical chance with a bow.", "+Crit chance with a bow.", 0, 0, false },
   { ABILITY_POISON,       3, 0, 134, "Poison", "Passive:  Increases the damage you do with all weapons.", "Permanent +2 damage.", 0, 0, false },
   { ABILITY_IRONWILL,     5, 0, 132, "Iron Will", "Passive:  Increses your chances of resisting bad status effects.", "Increased resistance.", 0, 0, false },
   { ABILITY_TANK,         3, 0, 133, "Tank", "Passive: You have two points of natural armor.  This stacks with all other armor.", "Permanent +2 armor.", 0, 0, false },
   { ABILITY_HARDY,        5, 0, 135, "Hardy", "Passive: You permanently gain ten hitpoints.", "Permanent +10 hitpoints.", 0, 0, false }
};

string g_HappyResponses[30] = 
{
   "You've done it!",
   "I can't believe it!",
   "At last, we can rebuild.",
   "The Slorn are gone?",
   "You're a hero!",
   "It's incredible!",
   "The fear...the fear is gone.",
   "Now we can get back to our lives.",
   "Wait, what happened?",
   "The Slorn King dead?  Can it be true?",
   "We won the war!",
   "Let us honor the dead.",
   "I'll buy you a drink any time!",
   "I can't believe it!  I'm so happy!",
   "Now maybe I can finally get a good night's sleep.",
   "Is it really over?",
   "The nightmare is over?",
   "I'd been having these horrible dreams...but they're gone now!",
   "It'll still take years to rebuild from the attacks...",
   "There was so much death...",
   "Can we rest now?  Can we finally rest?",
   "I don't believe he's gone.  We must prepare for the next attack!",
   "Did you ever know that...you're my hero?",
   "I'm so relieved.",
   "No more attacks!",
   "You killed them!  You killed them all!",
   "You did what the army could not!",
   "Our trade took a real beating...we'll still be poor for a while.",
   "At last we can recover.",
   "You wouldn't happen to be...married, would you?"
};

string g_MusicTracks[NUMBER_OF_MUSIC_TRACKS];
int g_CurrentMusic;

bool g_FirstTime;

void AddConsoleString( std::string str, int r, int g, int b )
{
   char string1[256];
   char string2[256];
   if( GetStringMetrics(g_font.get(), g_fontSize,  str ) > 610 )
   {
      int lastsplit;
      for(int i = 0; i < str.length(); ++i )
      {
         if( str[i] == ' ' )
         {
            int length = str.copy(string1, i, 0 );
            string1[length] = '\0';
            if(GetStringMetrics(g_font.get(), g_fontSize, string(string1)) < 610 )
               lastsplit = i;
         }
      }

      int length = str.copy(string1, lastsplit, 0 );
      string1[length] = '\0';

      length = str.copy(string2,  str.size() - lastsplit, lastsplit );
      string2[length] = '\0';

      ColoredString _coloredString( string1, r, g, b);
      g_ConsoleStrings.push_back(_coloredString);
      ColoredString _coloredString2( string2, r, g, b);
      g_ConsoleStrings.push_back(_coloredString2);

   }
   else
   {
      ColoredString _coloredString(str, r, g, b);
      g_ConsoleStrings.push_back(_coloredString);
   }

   while(g_ConsoleStrings.size() > 7)
   {
      g_ConsoleStrings.pop_front();
   }

}

void DrawConsoleStrings()
{
   deque<ColoredString>::iterator node = g_ConsoleStrings.begin();

   int posy = 380;
   for(node; node != g_ConsoleStrings.end(); ++node)
   {
      DrawTextAt(g_font.get(), g_fontSize, (*node).m_String, 18, posy, (*node).m_Color);
      posy += 12;
   }

}

bool DrawButton( std::string text, int x, int y, int r, int g, int b, int a )
{
   if( text.empty() )
      return false;

   int xwidth = GetStringMetrics(g_font.get(), g_fontSize,  text );

   int yheight = 12;

   DrawImageRectAt( g_Mask, 0, 0, xwidth - 1, yheight - 1, x + 1, y + 1 );

   if( g_InputSystem->IsLButtonDownInDesignRegion( x, y, x + xwidth, y + yheight ) )
   {
      DrawBoxAt(x, y, xwidth + 2, yheight + 2, r, g, b, a, true);
      DrawTextAt(g_font.get(), g_fontSize,  text, x + 2, y, 255 - r, 255 - g, 255 - b, a );
   }
   else
   {
      DrawBoxAt(x, y, xwidth + 2, yheight + 2, r, g, b, a);
      DrawTextAt(g_font.get(), g_fontSize,  text, x + 2, y, r, g, b, a );
   }

   if( g_InputSystem->WasLButtonClickedInDesignRegion( x, y, x + xwidth, y + yheight ) )
      return true;
   else
      return false;
}

bool DrawButtonCentered( std::string text, int x, int y, int r, int g, int b, int a )
{
   if( text.empty() )
      return false;

   int xwidth = GetStringMetrics(g_font.get(), g_fontSize,  text );

   x -= (xwidth / 2);

   int yheight = 12;

   DrawImageRectAt( g_Mask, 0, 0, xwidth - 1, yheight - 1, x + 1, y + 1 );

   if( g_InputSystem->IsLButtonDownInDesignRegion( x, y, x + xwidth, y + yheight ) )
   {
      DrawBoxAt(x, y, xwidth + 2, yheight + 2, r, g, b, a, true);
      DrawTextAt(g_font.get(), g_fontSize,  text, x + 2, y, 255 - r, 255 - g, 255 - b, a );
   }
   else
   {
      DrawBoxAt(x, y, xwidth + 2, yheight + 2, r, g, b, a);
      DrawTextAt(g_font.get(), g_fontSize,  text, x + 2, y, r, g, b, a );
   }

   if( g_InputSystem->WasLButtonClickedInDesignRegion( x, y, x + xwidth, y + yheight ) )
      return true;
   else
      return false;
}


bool DoRPGCheck( int doer, int stopper )
{
   int result = g_RNG->Random( doer + stopper );
   return ( result <= doer );
}

vector<ColoredString> ConstructItemTooltip( Item* item )
{
   stringstream writer;

   ColoredString* temp = new ColoredString(item->m_Name);
   vector<ColoredString> tooltip;
   tooltip.push_back(*temp);

   writer.str("");
   if( item->m_ArmorBonus > 0 )
   {
      writer << "Armor: " << item->m_ArmorBonus;
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_DamageMultiplier > 0 )
   {
      writer << "Damage: " << item->m_DamageMultiplier;
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_STRBonus > 0 )
   {
      writer << "+" << item->m_STRBonus << " Strength";
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_DEXBonus > 0 )
   {
      writer << "+" << item->m_DEXBonus << " Dexterity";
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_INTBonus > 0 )
   {
      writer << "+" << item->m_INTBonus << " Intelligence";
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_ENDBonus > 0 )
   {
      writer << "+" << item->m_ENDBonus << " Endurance";
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_WILBonus > 0 )
   {
      writer << "+" << item->m_WILBonus << " Will";
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   if( item->m_Price > 0 )
   {
      writer << item->m_Price << " coins";
      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   //  Make sure we meet strength requirements
   if( item->m_Name == "Sword" )
   {
      if( g_Player->m_Strength < 4 )
      {
         temp = new ColoredString( "Requires Strength 4", 255, 128, 128 );
      }
      else
         temp = new ColoredString( "Requires Strength 4" );
      tooltip.push_back(*temp);
   }

   if( item->m_Name == "Bow" )
   {
      if( g_Player->m_Dexterity < 5 )
      {
         temp = new ColoredString( "Requires Dexterity 5", 255, 128, 128 );
      }
      else
         temp = new ColoredString( "Requires Dexterity 5" );
      tooltip.push_back(*temp);
   }

   if( item->m_Name == "Warhammer" )
   {
     if( g_Player->m_Strength < 8 )
      {
         temp = new ColoredString( "Requires Strength 8", 255, 128, 128 );
      }
      else
         temp = new ColoredString( "Requires Strength 8" );
      tooltip.push_back(*temp);
   }

   if( item->m_Name == "Leather Armor" )
   {
     if( g_Player->m_Strength < 3 )
      {
         temp = new ColoredString( "Requires Strength 3", 255, 128, 128 );
      }
      else
         temp = new ColoredString( "Requires Strength 3" );
      tooltip.push_back(*temp);
   }

   if( item->m_Name == "Chain Armor" )
   {
     if( g_Player->m_Strength < 5 )
      {
         temp = new ColoredString( "Requires Strength 5", 255, 128, 128 );
      }
      else
         temp = new ColoredString( "Requires Strength 5" );
      tooltip.push_back(*temp);
   }

   if( item->m_Name == "Plate Armor" )
   {
     if( g_Player->m_Strength < 7 )
      {
         temp = new ColoredString( "Requires Strength 7", 255, 128, 128 );
      }
      else
         temp = new ColoredString( "Requires Strength 7" );
      tooltip.push_back(*temp);
   }


   return tooltip;
}


vector<ColoredString> ConstructAbilityTooltip( int index )
{
   stringstream writer;

   ColoredString* temp = new ColoredString( g_SpecialAbilities[index].m_Name );
   vector<ColoredString> tooltip;
   tooltip.push_back(*temp);

   if( index < 8 )
   {
      writer.str("");
      writer <<  g_SpecialAbilities[index].m_ManaCost << " mana";

      temp = new ColoredString( writer.str() );
      tooltip.push_back(*temp);
   }

   writer.str("");
   temp = new ColoredString( g_SpecialAbilities[index].m_ShortDescription );
   tooltip.push_back(*temp);

   return tooltip;
}

Item* ItemFactory(int itemtype, bool savegame)
{
   Item* temp = new Item;
   switch(itemtype)
   {
   default:
      AddConsoleString("Undefined item type in loadmap.");
      assert(false && "Undefined item type in loadmap!");
      delete temp;
      temp = 0;
   case 0:
      temp->Init("Items/leatherarmor.cfg");
      break;

   case 1:
      temp->Init("Items/chainarmor.cfg");
      break;

   case 2:
      temp->Init("Items/platearmor.cfg");
      break;

   case 3:
      if( !g_Player || (savegame && g_Player->m_GotViridianArmor) || (!savegame && !g_Player->m_GotViridianArmor) )
         temp->Init("Items/viridianarmor.cfg");
      else
         temp =  NULL;
      break;

   case 4:
      temp->Init("Items/teleporter.cfg");
      break;

   case 5:
      temp->Init("Items/stepswitch.cfg");
      break;

   case 6:
      temp->Init("Items/shootswitch.cfg");
      break;

   case 7:
      temp->Init("Items/sleepfield.cfg");
      break;

   case 8:
      temp->Init("Items/bow.cfg");
      break;

   case 9:
      temp->Init("Items/dagger.cfg");
      break;

   case 10:
      temp->Init("Items/sword.cfg");
      break;

   case 11:
      if( !g_Player || (savegame && g_Player->m_GotSolsSword) || (!savegame && !g_Player->m_GotSolsSword) )
         temp->Init("Items/righteoussword.cfg");
      else
         temp =  NULL;
      break;

   case 12:
      temp->Init("Items/warhammer.cfg");
      break;

   case 13:
      temp->Init("Items/chest.cfg");
      break;

   case 14:
      temp->Init("Items/coin.cfg");
      break;

   case 15:
      temp->Init("Items/secretdoor.cfg");
      break;

   case 16:
      temp->Init("Items/poisonfield.cfg");
      break;

   case 17:
      temp->Init("Items/manapotion.cfg");
      break;

   case 18:
      temp->Init("Items/lockeddoor.cfg");
      break;

   case 19:
      temp->Init("Items/healthpotion.cfg");
      break;

   case 20:
      temp->Init("Items/firefield.cfg");
      break;

   case 21:
      temp->Init("Items/electricfield.cfg");
      break;

   case 22:
      temp->Init("Items/clotharmor.cfg");
      break;

   case 23:
      temp->Init("Items/ring.cfg");
      break;

   case 24:
      temp->Init("Items/necklace.cfg");
      break;

   case 25:
      temp->Init("Items/transformingfield.cfg");
      break;
   }

   return temp;
}

NPC* UnitFactory(int type, bool savegame)
{
   NPC* temp = new NPC;
   switch(type)
   {
   default:
      AddConsoleString("Undefined NPC type in loadmap.");
      assert(false && "Undefined NPC type in loadmap!");
      delete temp;
      temp = 0;

   case 0:
      temp->Init("NPCs/child.cfg");
      break;

   case 1:
      temp->Init("NPCs/farmer.cfg");
      break;

   case 2:
      temp->Init("NPCs/merchant.cfg");
      break;

   case 3:
      temp->Init("NPCs/man.cfg");
      break;

   case 4:
      temp->Init("NPCs/woman.cfg");
      break;

   case 5:
      temp->Init("NPCs/king.cfg");
      break;

   case 8:
      if( !g_Player || (savegame && g_Player->m_KilledWhelp) || (!savegame && !g_Player->m_KilledWhelp) )
         temp->Init("NPCs/babydragon.cfg");
      else
         temp =  NULL;
      break;

   case 9:
      temp->Init("NPCs/dragon.cfg");
      break;

   case 10:
      temp->Init("NPCs/firebird.cfg");
      break;

   case 11:
      temp->Init("NPCs/gazer.cfg");
      break;

   case 12:
      temp->Init("NPCs/cleric.cfg");
      break;

   case 13:
      temp->Init("NPCs/thief.cfg");
      break;

   case 14:
      temp->Init("NPCs/quetz.cfg");
      break;

   case 15:
      if( !g_Player || (savegame && g_Player->m_KilledSlornKing) || (!savegame && !g_Player->m_KilledSlornKing) )
         temp->Init("NPCs/slorniteking.cfg");
      else
         temp = NULL;
      break;

   case 16:
      temp->Init("NPCs/blob.cfg");
      break;

   case 17:
      temp->Init("NPCs/mushroom.cfg");
      break;

   case 18:
      temp->Init("NPCs/caster.cfg");
      break;

   case 19:
      temp->Init("NPCs/fighter.cfg");
      break;

   case 20:
      temp->Init("NPCs/lightninggolem.cfg");
      break;

   case 21:
      temp->Init("NPCs/crystalgolem.cfg");
      break;

   case 22:
      temp->Init("NPCs/handofdeath.cfg");
      break;

   case 23:
      temp->Init("NPCs/froad.cfg");
      break;

   case 24:
      temp->Init("NPCs/pawnshop.cfg");
      break;

      //  Emerald NPCs
   case 25:
      temp->Init("NPCs/emchild.cfg");
      break;

   case 26:
      temp->Init("NPCs/emfarmer.cfg");
      break;

   case 27:
      temp->Init("NPCs/emfarmerswife.cfg");
      break;

   case 28:
      temp->Init("NPCs/emguard.cfg");
      break;

   case 29:
      temp->Init("NPCs/emhealer.cfg");
      break;

   case 30:
      temp->Init("NPCs/emleader.cfg");
      break;

   case 31:
      temp->Init("NPCs/emmerchant.cfg");
      break;

   case 32:
      temp->Init("NPCs/emwanderingmerchant.cfg");
      break;


      //  The Last Front NPCs


   case 33:
      temp->Init("NPCs/tlfguard1.cfg");
      break;

   case 34:
      temp->Init("NPCs/tlfguard2.cfg");
      break;

   case 35:
      temp->Init("NPCs/tlfhealer.cfg");
      break;

   case 36:
      temp->Init("NPCs/tlfking.cfg");
      break;

   case 37:
      temp->Init("NPCs/tlfrearguard.cfg");
      break;

      //  Sapphire NPCs

   case 38:
      temp->Init("NPCs/sapchild.cfg");
      break;

   case 39:
      temp->Init("NPCs/sapfisherman.cfg");
      break;

   case 40:
      temp->Init("NPCs/sapfisheworman.cfg");
      break;

   case 41:
      temp->Init("NPCs/saphealer.cfg");
      break;

   case 42:
      temp->Init("NPCs/sapleader.cfg");
      break;

   case 43:
      temp->Init("NPCs/sapmerchant.cfg");
      break;

   case 44:
      temp->Init("NPCs/sapslornguard1.cfg");
      break;

   case 45:
      temp->Init("NPCs/sapslornguard2.cfg");
      break;

   case 46:
      temp->Init("NPCs/sapwanderingmerchant.cfg");
      break;


      //  Ruby NPCs

   case 47:
      temp->Init("NPCs/rubychild.cfg");
      break;

   case 48:
      temp->Init("NPCs/rubyfarmer.cfg");
      break;

   case 49:
      temp->Init("NPCs/rubyfarmerswife.cfg");
      break;

   case 50:
      temp->Init("NPCs/rubyhealer.cfg");
      break;

   case 51:
      temp->Init("NPCs/rubyinsanesoldier.cfg");
      break;

   case 52:
      temp->Init("NPCs/rubyleader.cfg");
      break;

   case 53:
      temp->Init("NPCs/rubymerchant.cfg");
      break;

   case 54:
      temp->Init("NPCs/rubyminer.cfg");
      break;

   case 55:
      temp->Init("NPCs/rubywanderingmerchant.cfg");
      break;


      //  Other NPCs
   case 56:
      if( !g_Player || (savegame && g_Player->m_KilledEvilOne) || (!savegame && !g_Player->m_KilledEvilOne) )
         temp->Init("NPCs/undeaddragon.cfg");
      else
         temp = NULL;
      break;

   case 57:
      temp->Init("NPCs/skitteringscorpion.cfg");
      break;

   case 58:
      if( !g_Player || (savegame && g_Player->m_KilledMendikar) || (!savegame && !g_Player->m_KilledMendikar) )
         temp->Init("NPCs/mendikarthemadmaker.cfg");
      else
         temp = NULL;
      break;

   case 59:
      temp->Init("NPCs/clatteringskeleton.cfg");
      break;

   case 60:
      temp->Init("NPCs/fleshborer.cfg");
      break;

   case 61:
      temp->Init("NPCs/spittingasp.cfg");
      break;

   case 62:
      temp->Init("NPCs/ambushingbat.cfg");
      break;

   case 63:
      temp->Init("NPCs/spewingslime.cfg");
      break;

   case 64:
      temp->Init("NPCs/mindthresher.cfg");
      break;

   case 65:
      temp->Init("NPCs/rodentofunusualsize.cfg");
      break;

   case 66:
      temp->Init("NPCs/oublietteinsaneman.cfg");
      break;

   case 67:
      if( !g_Player || (savegame && g_Player->m_KilledMendikar) || (!savegame && !g_Player->m_KilledMendikar) )
         temp->Init("NPCs/oubliettepalsiedwoman.cfg");
      else
         temp = NULL;
      break;

   case 68:
      temp->Init("NPCs/redrighthand.cfg");
      break;

   case 69:
      temp->Init("NPCs/triumphantking.cfg");
      break;

   case 70:
      temp->Init("NPCs/emdancepartygirl.cfg");
      break;

   case 71:
      temp->Init("NPCs/mommydragon.cfg");
      break;
   }

   return temp;
}



bool CheckPath( int x1, int y1, int x2, int y2, bool careaboutnpcs, bool careaboutitems )
{
   int ticksthiscall = static_cast<uint32_t>(g_Engine->GameTimeInMS());

   signed char ix;
   signed char iy;

   // if x1 == x2 or y1 == y2, then it does not matter what we set here
   int delta_x((x2 > x1?(ix = 1, x2 - x1):(ix = -1, x1 - x2)) << 1);
   int delta_y((y2 > y1?(iy = 1, y2 - y1):(iy = -1, y1 - y2)) << 1);

   if (delta_x >= delta_y)
   {
      // error may go below zero
      int error(delta_y - (delta_x >> 1));

      while (x1 != x2)
      {
         if (error >= 0)
         {
            if (error || (ix > 0))
            {
               y1 += iy;
               error -= delta_x;
            }
            // else do nothing
         }
         // else do nothing

         x1 += ix;
         error += delta_y;

         if( !g_Map->m_terrainTypes[g_Map->m_map[x1][y1].m_TerrainType].m_passable &&
            ( g_Map->m_map[x1][y1].m_TerrainType != 2 &&
            g_Map->m_map[x1][y1].m_TerrainType != 7 &&
            g_Map->m_map[x1][y1].m_TerrainType != 9 ) &&
            !( x1 == x2 && y1 == y2 )


            )
         {
            return false;
         }
      }
   }
   else
   {
      // error may go below zero
      int error(delta_x - (delta_y >> 1));

      while (y1 != y2)
      {
         if (error >= 0)
         {
            if (error || (iy > 0))
            {
               x1 += ix;
               error -= delta_y;
            }
            // else do nothing
         }
         // else do nothing

         y1 += iy;
         error += delta_x;

         if( !g_Map->m_terrainTypes[g_Map->m_map[x1][y1].m_TerrainType].m_passable &&
            ( g_Map->m_map[x1][y1].m_TerrainType != 2 && g_Map->m_map[x1][y1].m_TerrainType != 7 && g_Map->m_map[x1][y1].m_TerrainType != 9 ) &&
            !( x1 == x2 && y1 == y2 )
            )
         {
            return false;
         }
      }
   }


   return true;
}







/*

   
   if( x < 0 || x >= g_Map->m_Width ||
      y < 0 || y >= g_Map->m_Height ||
      endx < 0 || endx >= g_Map->m_Width ||
      endy < 0 || endy >= g_Map->m_Height )
      return false;

   float sx, ex, sy, ey;
   bool visible = false;

   for( int i = 0; i < 4; ++ i )
   {
      sx = x + .5f;
      sy = y + .5f;

      if( i == 0 )
      {
         ex = endx;
         ey = endy;
      }

      if( i == 1 )
      {
         ex = endx + 1;
         ey = endy;
      }

      if( i == 2 )
      {
         ex = endx + 1;
         ey = endy + 1;
      }

      if( i == 3 )
      {
         ex = endx;
         ey - endy + 1;
      }


      float normalx = ex - sx;
      float normaly = ey - sy;

      float length = LineLength( sx, sy, ex, ey);

      normalx /= length;
      normaly /= length;

      normalx /= 2;
      normaly /= 2;

      bool thisvisible = true;

      float previousx;
      float previousy;

      while( ( ( sx - ex) > .5f && (sx + ex) < -.5f ) || ( ( sy - ey) > .5f && ( sy + ey ) < -.5f )  )
      {
         sx += normalx;
         sy += normaly;

         if( int( sx ) == int( previousx ) && int( sy == previousy ) )
            continue; //  Don't do anything unless something has changed
       
         if( !g_Map->m_terrainTypes[g_Map->m_map[int(sx)][int(sy)].m_TerrainType].m_passable &&
            ( g_Map->m_map[int(sx)][int(sy)].m_TerrainType != 2 && g_Map->m_map[int(sx)][int(sy)].m_TerrainType != 7 && g_Map->m_map[int(sx)][int(sy)].m_TerrainType != 9 )
            )
         {
            thisvisible = false;
         }

         if( careaboutitems )
         {
            list<Item*>::iterator node = g_Map->m_ItemList.begin();
            for(node; node != g_Map->m_ItemList.end(); ++node)
            {
//               if( ( ( (*node)->m_PosX - x) > g_ViewRange ||  ( (*node)->m_PosX - x ) < -g_ViewRange ) && ( ( (*node)->m_PosY - y) > g_ViewRange || ( (*node)->m_PosY - y ) < -g_ViewRange ) )
//                  continue;

               if( (*node)->m_PosX == int(sx) && (*node)->m_PosY == int(sy) )
               {
                  if(! (*node)->m_Passable )
                     return false;
               }
            }
         }


         if( careaboutnpcs)
         {
            list<NPC*>::iterator node2 = g_Map->m_NPCList.begin();
            for(node2; node2 != g_Map->m_NPCList.end(); ++node2)
            {
//               if( ( ( (*node2)->m_PosX - x) > g_ViewRange ||  ( (*node2)->m_PosX - x ) < -g_ViewRange ) && ( ( (*node2)->m_PosY - y) > g_ViewRange || ( (*node2)->m_PosY - y ) < -g_ViewRange ) )
//                  continue;

               if( (*node2)->m_PosX == int(sx) && (*node2)->m_PosY == int(sy) )
               {
                  return false;
               }
            }
         }
      }

      visible = visible || thisvisible;
   }

   g_CheckPathCount += static_cast<uint32_t>(g_Engine->GameTimeInMS()) - ticksthiscall;

   return visible;
}*/

/*bool CheckPath( NPC* start, NPC* end )
{
   g_CheckPathCount++;
   float x = start->m_PosX + .5f;
   float y = start->m_PosY + .5f;
   float endx = end->m_PosX + .5f;
   float endy = end->m_PosY + .5f;

   float normalx = endx - x;
   float normaly = endy - y;

   float length = LineLength(x, y, endx, endy);

   normalx /= length;
   normaly /= length;

   while( abs( x - endx) > .5 || abs( y - endy) > .5  )
   {
      if( !g_Map->m_terrainTypes[g_Map->m_map[int(x)][int(y)].m_TerrainType].m_passable &&
         ( g_Map->m_map[int(x)][int(y)].m_TerrainType != 2 && g_Map->m_map[int(x)][int(y)].m_TerrainType != 7 )
         )
      {
         return false;
      }

      list<Item*>::iterator node = g_Map->m_ItemList.begin();
      for(node; node != g_Map->m_ItemList.end(); ++node)
      {
         if( (*node)->m_PosX == int(x) && (*node)->m_PosY == int(y) )
         {
            if(! (*node)->m_Passable )
               return false;
         }
      }

      /*      list<NPC*>::iterator node2 = g_Map->m_NPCList.begin();
      for(node2; node2 != g_Map->m_NPCList.end(); ++node2)
      {
      if( (*node2)->m_PosX == int(x) && (*node2)->m_PosY == int(y) && (*node2) != end )
      {
      return false;
      }
      }*/

/*      x += normalx;
      y += normaly;
   }

   return true;
}*/

float LineLength( float x, float y, float endx, float endy )
{
	int ticksthisupdate = static_cast<uint32_t>(g_Engine->GameTimeInMS());
   return sqrt( abs(float(endx - x) * float(endx - x)) + abs(float(endy - y) * float(endy - y )) );
   ticksthisupdate = static_cast<uint32_t>(g_Engine->GameTimeInMS()) - ticksthisupdate;
   g_LineLengthCount += ticksthisupdate;
}


bool IsPlayerInRegion(int x, int y, int endx, int endy )
{
   if( g_Player->m_PlayerPosX >= x && g_Player->m_PlayerPosX <= endx &&
      g_Player->m_PlayerPosY >= y && g_Player->m_PlayerPosY <= endy )
      return true;
   else
      return false;
}


//  Plays the appropriate music for the current situation.
void PlayMusic()
{

   if( g_StateMachine->GetCurrentState() == STATE_MAINSTATE )
   {
      if( g_Player->m_IsDead )
         return;

      if( g_Map->m_MapTitle == "Inaria Special" )
      {
         if( g_DanceParty )
         {
            if( g_CurrentMusic != MUSIC_DANCEPARTY )
            {
               g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_DANCEPARTY]);
               g_CurrentMusic = MUSIC_DANCEPARTY;
            }
         }
         else
         {
            if( g_CurrentMusic != MUSIC_WIN )
            {
               g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_WIN]);
               g_CurrentMusic = MUSIC_WIN;
            }
         }
      }


      if( g_Map->m_MapTitle == "Inaria" )
      {
         if( IsPlayerInRegion( 40, 24, 58, 41 ) )
         {
            if( g_CurrentMusic != MUSIC_EMERALD )
            {
               g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_EMERALD]);
               g_CurrentMusic = MUSIC_EMERALD;
            }
         }

         else if( IsPlayerInRegion( 43, 7, 58, 23 ) )
         {
            if( g_CurrentMusic != MUSIC_LASTFRONT )
            {
               g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_LASTFRONT]);
               g_CurrentMusic = MUSIC_LASTFRONT;
            }
         }

         else if( IsPlayerInRegion( 5, 6, 25, 37 ) ) 
         {
            if( g_CurrentMusic != MUSIC_SAPPHIRE )
            {
               g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_SAPPHIRE]);
               g_CurrentMusic = MUSIC_SAPPHIRE;
            }
         }

         else if( IsPlayerInRegion( 5, 39, 19, 52 ) )
         {
            if( g_CurrentMusic != MUSIC_RUBY )
            {
               g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_RUBY]);
               g_CurrentMusic = MUSIC_RUBY;
            }
         }

         else if( g_CurrentMusic != MUSIC_OVERWORLD )
         {
            g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_OVERWORLD]);
            g_CurrentMusic = MUSIC_OVERWORLD;
         }
      }


      if( g_Map->m_MapTitle == "The Mountain Trial"  && g_CurrentMusic != MUSIC_OVERWORLD )
      {
         g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_OVERWORLD]);
         g_CurrentMusic = MUSIC_OVERWORLD;
      }


      if( (g_Map->m_MapTitle == "The Collapsing Cave" || 
         g_Map->m_MapTitle == "The Deadly Maze of Death" || 
         g_Map->m_MapTitle == "Death's Coil" || 
         g_Map->m_MapTitle == "The Passage to Apocalypse" || 
         g_Map->m_MapTitle == "The Passage to Doom" || 
         g_Map->m_MapTitle == "The Infinite" || 
         g_Map->m_MapTitle == "The Oubliette" || 
         g_Map->m_MapTitle == "The Underwater Tunnel" ) && g_CurrentMusic != MUSIC_DUNGEON )
      {
         g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_DUNGEON]);
         g_CurrentMusic = MUSIC_DUNGEON;
      }

      if( g_Map->m_MapTitle == "The Castle of the Slornite King" && g_CurrentMusic != MUSIC_CASTLE )
      {
         g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_CASTLE]);
         g_CurrentMusic = MUSIC_CASTLE;
      }

      if( g_Map->m_MapTitle == "Volcano's Heart" && g_CurrentMusic != MUSIC_VOLCANOHEART )
      {
         g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_VOLCANOHEART]);
         g_CurrentMusic = MUSIC_VOLCANOHEART;
      }
   }

   else if( g_StateMachine->GetCurrentState() == STATE_STORESTATE )
   {
      if( g_CurrentMusic != MUSIC_SHOPPING )
      {
         g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_SHOPPING]);
         g_CurrentMusic = MUSIC_SHOPPING;
      }
   }

   else if( g_StateMachine->GetCurrentState() == STATE_TITLESTATE )
   {
      if( g_CurrentMusic != MUSIC_THEME )
      {
         g_SoundSystem->PlayMusic(g_MusicTracks[MUSIC_THEME]);
         g_CurrentMusic = MUSIC_THEME;
      }
   }

   else if( g_StateMachine->GetCurrentState() == STATE_EDITORSTATE )
   {
      g_SoundSystem->StopCurrentMusic();
   }
}

bool SaveGame()
{
   if( g_Player->m_IsDead )
   {
      AddConsoleString("You cannot save the game when you are dead!");
      return false;
   }

   if( g_Map->m_MapTitle == "The Infinite" )
   {
      AddConsoleString("You cannot save the game while inside The Infinite.");
      return false;
   }

   g_Map->SaveMap("savemap.map");

   ofstream ostream;
   ostream.open("savegame.txt");

   if( ostream.fail() || ostream.eof() )
      return false;

   //  Player stats.
   ostream << g_Player->m_Strength << endl;
   ostream << g_Player->m_Dexterity << endl;
   ostream << g_Player->m_Intelligence << endl;
   ostream << g_Player->m_Will << endl;
   ostream << g_Player->m_Endurance << endl;
   ostream << g_Player->m_CurrentHitPoints << endl;
   ostream << g_Player->m_CurrentManaPoints << endl;
   ostream << g_Player->m_CurrentGold << endl;
   ostream << g_Player->m_Level << endl;
   ostream << g_Player->m_CurrentXP << endl;
   ostream << g_Player->m_NextLevel << endl;
   ostream << g_Player->m_PlayerSprite << endl;
   ostream << g_Player->m_PoisonCounter << endl;
   ostream << g_Player->m_StatPoints << endl;

   ostream << g_Player->m_KilledWhelp << endl;
   ostream << g_Player->m_KilledSlornKing << endl;
   ostream << g_Player->m_AcceptedStupidOffer << endl;
   ostream << g_Player->m_KilledEvilOne << endl;
   ostream << g_Player->m_KilledMendikar << endl;
   ostream << g_Player->m_GotSolsSword << endl;;
   ostream << g_Player->m_GotViridianArmor << endl;
   ostream << g_Player->m_PaidByInarianKing << endl;
   ostream << g_Player->m_ShowTutorials << endl;

   //  Player inventory.
   ostream << g_Player->m_PlayerInventory.size() << endl;

   for( uint32_t i = 0; i < g_Player->m_PlayerInventory.size(); ++i)
   {
      ostream << g_Player->m_PlayerInventory[i]->m_ItemType << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_STRBonus << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_DEXBonus << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_INTBonus << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_WILBonus << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_ENDBonus << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_ArmorBonus << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_DamageMultiplier << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_Stackable << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_CurrentNumber << endl;
      ostream << g_Player->m_PlayerInventory[i]->m_Price << endl;
   }

   //  Player equipped items.
   if( g_Player->m_CurrentWeaponType )
   {
      ostream << g_Player->m_CurrentWeaponType->m_ItemType << endl;
      ostream << g_Player->m_CurrentWeaponType->m_STRBonus << endl;
      ostream << g_Player->m_CurrentWeaponType->m_DEXBonus << endl;
      ostream << g_Player->m_CurrentWeaponType->m_INTBonus << endl;
      ostream << g_Player->m_CurrentWeaponType->m_WILBonus << endl;
      ostream << g_Player->m_CurrentWeaponType->m_ENDBonus << endl;
      ostream << g_Player->m_CurrentWeaponType->m_ArmorBonus << endl;
      ostream << g_Player->m_CurrentWeaponType->m_DamageMultiplier << endl;
      ostream << g_Player->m_CurrentWeaponType->m_Price << endl;
   }
   else
   {
      ostream << -1 << endl;
   }

   if( g_Player->m_CurrentArmorType )
   {
      ostream << g_Player->m_CurrentArmorType->m_ItemType << endl;
      ostream << g_Player->m_CurrentArmorType->m_STRBonus << endl;
      ostream << g_Player->m_CurrentArmorType->m_DEXBonus << endl;
      ostream << g_Player->m_CurrentArmorType->m_INTBonus << endl;
      ostream << g_Player->m_CurrentArmorType->m_WILBonus << endl;
      ostream << g_Player->m_CurrentArmorType->m_ENDBonus << endl;
      ostream << g_Player->m_CurrentArmorType->m_ArmorBonus << endl;
      ostream << g_Player->m_CurrentArmorType->m_DamageMultiplier << endl;
      ostream << g_Player->m_CurrentArmorType->m_Price << endl;
   }
   else
   {
      ostream << -1 << endl;
   }

   if( g_Player->m_CurrentTrinketType )
   {
      ostream << g_Player->m_CurrentTrinketType->m_ItemType << endl;
      ostream << g_Player->m_CurrentTrinketType->m_STRBonus << endl;
      ostream << g_Player->m_CurrentTrinketType->m_DEXBonus << endl;
      ostream << g_Player->m_CurrentTrinketType->m_INTBonus << endl;
      ostream << g_Player->m_CurrentTrinketType->m_WILBonus << endl;
      ostream << g_Player->m_CurrentTrinketType->m_ENDBonus << endl;
      ostream << g_Player->m_CurrentTrinketType->m_ArmorBonus << endl;
      ostream << g_Player->m_CurrentTrinketType->m_DamageMultiplier << endl;
      ostream << g_Player->m_CurrentTrinketType->m_Price << endl;
   }
   else
   {
      ostream << -1 << endl;
   }

   //  Player abilities
   for( int i = 0; i < 16; ++ i )
   {
      ostream << g_SpecialAbilities[i].m_Active << endl;
      ostream << g_SpecialAbilities[i].m_Level << endl;
      ostream << g_SpecialAbilities[i].m_Duration << endl;
   }

   //  Player status effects

   //  Save the name of the map and the map location of the player.
   ostream << g_Map->m_MapName << endl;

   ostream << g_Player->m_PlayerPosX << endl;
   ostream << g_Player->m_PlayerPosY << endl;

   ostream.close();

   return true;
}

bool LoadGame()
{

   ifstream instream;
   instream.open("savegame.txt");

   if( instream.fail() || instream.eof() )
      return false;

   g_Player->m_IsDead = false;  //  There.  I'm alive again.

   //  Load the player data.
   instream >> g_Player->m_Strength;
   instream >> g_Player->m_Dexterity;
   instream >> g_Player->m_Intelligence;
   instream >> g_Player->m_Will;
   instream >> g_Player->m_Endurance;
   instream >> g_Player->m_CurrentHitPoints;
   instream >> g_Player->m_CurrentManaPoints;
   instream >> g_Player->m_CurrentGold;
   instream >> g_Player->m_Level;
   instream >> g_Player->m_CurrentXP;
   instream >> g_Player->m_NextLevel;
   instream >> g_Player->m_PlayerSprite;
   instream >> g_Player->m_PoisonCounter;
   instream >> g_Player->m_StatPoints;

   instream >> g_Player->m_KilledWhelp;
   instream >> g_Player->m_KilledSlornKing;
   instream >> g_Player->m_AcceptedStupidOffer;
   instream >> g_Player->m_KilledEvilOne;
   instream >> g_Player->m_KilledMendikar;
   instream >> g_Player->m_GotSolsSword;
   instream >> g_Player->m_GotViridianArmor;
   instream >> g_Player->m_PaidByInarianKing;
   instream >> g_Player->m_ShowTutorials;

   g_Player->m_PlayerInventory.clear();
   g_Player->m_CurrentArmorType = NULL;
   g_Player->m_CurrentWeaponType = NULL;
   g_Player->m_CurrentTrinketType = NULL;
   int inventorysize;
   instream >> inventorysize;

   for(int i = 0; i < inventorysize; ++i )
   {
      int itemtype;
      instream >> itemtype;
      Item* temp = ItemFactory(itemtype, true);
      instream >> temp->m_STRBonus;
      instream >> temp->m_DEXBonus;
      instream >> temp->m_INTBonus;
      instream >> temp->m_WILBonus;
      instream >> temp->m_ENDBonus;
      instream >> temp->m_ArmorBonus;
      instream >> temp->m_DamageMultiplier;
      instream >> temp->m_Stackable;
      instream >> temp->m_CurrentNumber;
      instream >> temp->m_Price;

      g_Player->m_PlayerInventory.push_back(temp);
   }

   //  Load equipped items
   int itemtype;
   instream >> itemtype;

   if( itemtype != -1 )
   {
      Item* temp = ItemFactory(itemtype, true);
      instream >> temp->m_STRBonus;
      instream >> temp->m_DEXBonus;
      instream >> temp->m_INTBonus;
      instream >> temp->m_WILBonus;
      instream >> temp->m_ENDBonus;
      instream >> temp->m_ArmorBonus;
      instream >> temp->m_DamageMultiplier;
      instream >> temp->m_Price;

      g_Player->m_CurrentWeaponType = temp;
   }

   instream >> itemtype;

   if( itemtype != -1 )
   {
      Item* temp = ItemFactory(itemtype, true);
      instream >> temp->m_STRBonus;
      instream >> temp->m_DEXBonus;
      instream >> temp->m_INTBonus;
      instream >> temp->m_WILBonus;
      instream >> temp->m_ENDBonus;
      instream >> temp->m_ArmorBonus;
      instream >> temp->m_DamageMultiplier;
      instream >> temp->m_Price;

      g_Player->m_CurrentArmorType = temp;
   }

   instream >> itemtype;

   if( itemtype != -1 )
   {
      Item* temp = ItemFactory(itemtype, true);
      instream >> temp->m_STRBonus;
      instream >> temp->m_DEXBonus;
      instream >> temp->m_INTBonus;
      instream >> temp->m_WILBonus;
      instream >> temp->m_ENDBonus;
      instream >> temp->m_ArmorBonus;
      instream >> temp->m_DamageMultiplier;
      instream >> temp->m_Price;

      g_Player->m_CurrentTrinketType = temp;
   }


   //  Player abilities
   for( int i = 0; i < 16; ++ i )
   {
      instream >> g_SpecialAbilities[i].m_Active;
      instream >> g_SpecialAbilities[i].m_Level;
      instream >> g_SpecialAbilities[i].m_Duration;
   }

   //  Load the map and the location of the player.
   string mapname;
   getline(instream, mapname);
   getline(instream, mapname);
   if( !g_Map->LoadMap("savemap.map") )
      g_Map->LoadMap(mapname.c_str());

   instream >> g_Player->m_PlayerPosX;
   instream >> g_Player->m_PlayerPosY;

   g_Player->RecalculateHitpointsAndMana();

   return true;

}

int Factorial(int start)
{
   int finalvalue = 0;
   for( int i = 0; i <= start; ++i )
      finalvalue += i;
   return finalvalue;
}


int HalfNHalf( int range )
{
   int split = 0;

   if( range % 2 ) // Odd
   {
      split = 1 + ( (range - 1) / 2) + 1 + g_RNG->Random( (range - 1) / 2);
   }
   else
   {
      split = (range / 2) + 1 + g_RNG->Random(range / 2 );
   }
   return split;
}

Texture* g_TextBackground;
Texture* g_TBUL;
Texture* g_TBUR;
Texture* g_TBLR;
Texture* g_TBLL;
Texture* g_TalkSpurLeft;
Texture* g_TalkSpurRight;
Texture* g_TalkSpurRightUp;

void DrawToolTip(Font* font, float fontSize, std::vector<ColoredString> strings, int x, int y, int anchorcorner)
{
    DrawToolTipRender(font, fontSize, strings,
        static_cast<int>(DesignToRenderX(static_cast<float>(x))),
        static_cast<int>(DesignToRenderY(static_cast<float>(y))), 1.0f, anchorcorner);
}

void DrawToolTip(Font* font, float fontSize, std::string strings, int x, int y, int anchorcorner, int r, int g, int b)
{
    std::vector<ColoredString> temp;
    temp.emplace_back(strings, MakeColor(r, g, b));
    DrawToolTip(font, fontSize, temp, x, y, anchorcorner);
}

void DrawToolTip(Font* font, float fontSize, std::vector<ColoredString> strings, int x, int y, float linewidth, int anchorcorner)
{
    DrawToolTipRender(font, fontSize, strings,
        static_cast<int>(DesignToRenderX(static_cast<float>(x))),
        static_cast<int>(DesignToRenderY(static_cast<float>(y))), linewidth, anchorcorner);
}

void DrawToolTip(Font* font, float fontSize, std::string strings, int x, int y, float linewidth, int anchorcorner, int r, int g, int b)
{
    std::vector<ColoredString> temp;
    temp.emplace_back(strings, MakeColor(r, g, b));
    DrawToolTip(font, fontSize, temp, x, y, linewidth, anchorcorner);
}