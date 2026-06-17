#include "TutorialState.h"
#include "GameGlobals.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  TutorialState
////////////////////////////////////////////////////////////////////////////////

TutorialState::~TutorialState()
{

}

void TutorialState::Init(const std::string& configfile)
{
   m_Quitting = false;
   m_Returning = false;

   g_ShowBackstory = true;
   m_SecondPage = false;
}

void TutorialState::Shutdown(){}
void TutorialState::Update()
{
   if( g_InputSystem->WasKeyPressed(KEY_ESCAPE) )
   {
      g_StateMachine->PopState();
   }

}
void TutorialState::Draw()
{
   if( !g_ShowBackstory )
   {

      if( !m_SecondPage )
      {

         DrawImageResizedAt(g_Mask, 40, 40, 560, 400 );
         DrawBoxAt(40, 40, 560, 400);

         DrawTextCenteredAt(g_font.get(), g_fontSize, "Welcome to Inaria!", 320, 45);

         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "This is a brief tutorial on how to play Inaria.", 320, 70);

         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "YOUR CHARACTER", 320, 92);

         DrawSpriteAt( g_Tiles[64], 60, 100);
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "Strength - Determines your damage and how often you hit in melee combat.", 80, 104);

         DrawSpriteAt( g_Tiles[107], 60, 118);
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "Dexterity - Determines critical hits and how often you hit combat.", 80, 122);

         DrawSpriteAt( g_Tiles[108], 60, 136);
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "Endurance - Determines how many extra hitpoints you gain when you level up.", 80, 140);

         DrawSpriteAt( g_Tiles[109], 60, 154);
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "Intelligence - Determines what spells you can cast and spell combat.", 80, 158);

         DrawSpriteAt( g_Tiles[110], 60, 172);
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "Will - Determines spell combat effectiveness and your mana points.", 80, 176);

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "Every time your character levels up, you will be given skill points you can use", 80, 198);

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "to improve your stats or buy special abilities (more on that below).", 80, 210 );

         int controlstart = 236;

         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "CONTROLLING THE GAME", 320, controlstart);

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "You can move around three ways: left-click on the map in the direction you want", 80, controlstart + 12);
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "to go, use the WASD or arrow keys, or use the following key layouts to move:", 80, controlstart + 24);;//WASD or arrow keys to move.  The four standard icons in the", 80, 272);
         DrawTextAt(g_smallFont.get(), g_smallFontSize, "               q  w  e              7  8  9", 80, controlstart + 40);
         DrawTextAt(g_smallFont.get(), g_smallFontSize, "                \\ | /                \\ | / ", 80, controlstart + 52);
         DrawTextAt(g_smallFont.get(), g_smallFontSize, "               a- . -d              4- . -6", 80, controlstart + 64);
         DrawTextAt(g_smallFont.get(), g_smallFontSize, "                / | \\                / | \\ ", 80, controlstart + 76);
         DrawTextAt(g_smallFont.get(), g_smallFontSize, "               z  x  c              1  2  3", 80, controlstart + 88);

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "You can pass your turn by pressing the space bar or 5 on the keypad. The four", 80, controlstart + 104);
            
         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "standard icons in the bottom-right allow you to pick things up, attack, talk to", 80, controlstart + 116);

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "and look at things.  If you don't want to use those icons, you can simply right-click", 80, controlstart + 128);

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "people something and the game will do the \"right\" thing according to what you just ", 80, controlstart + 140); 

         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "clicked on - talking to friendlies, melee attacking enemies and picking up objects.", 80, controlstart + 152);

//         DrawTextAt(g_smallFont.get(), g_smallFontSize,  "", 80, controlstart + 164);


         if( DrawButton("Next Page", 440, 420 ) )
         {
            m_SecondPage = true;
         }

         if( DrawButton("Done", 540, 420 ) )
         {
            g_StateMachine->PopState();
         }
      }
      else
      {
         DrawImageResizedAt(g_Mask, 40, 40, 560, 400 );
         DrawBoxAt(40, 40, 560, 400);


         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "INVENTORY", 320, 50);

         DrawParagraphAt(g_smallFont.get(), g_smallFontSize, "As you travel through the game, you may purchase or find items.  \
These will go into your inventory.   You can left-click on inventory items to use or equip them.  \
You can equip up to three different kinds of items - weapons, armor and trinkets.  All three can be \
enchanted and add bonuses to your base stats.  Click an equipped item to unequip it and put it back \
in your inventory if you have room for it.", 80, 66, 480, 12);

         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "LEVELING UP", 320, 140 );

         DrawParagraphAt(g_smallFont.get(), g_smallFontSize, "When you level up, you will be given a certain number of skill \
points that you can use to improve your stats or buy special abilities.  As you improve stats, the \
cost to improve them again goes up.  Abilities have a base cost to buy, and some abilities can be \
improved at a higher cost.  You don't have to spend all your skill points right away.  Any unspent \
points are saved and you can spend them anytime later.", 80, 156, 480, 12 );

         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "ABILITIES", 320, 230 );
            
         DrawParagraphAt(g_smallFont.get(), g_smallFontSize, "You can switch between your inventory and your ability list by \
clicking the tabs above them.  You start out with a single ability - Smite, a magical ranged attack.  \
As you level up, you can unlock new abilities.  Some abilities are passive and give you permanent \
bonuses.  Some can be used by left-clicking.  Some (like Smite) require you to target something in \
the world.  You can mouse over each ability's name to get a short description.", 80, 246, 480, 12);

         DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize,  "TIPS FOR STARTING OUT", 320, 325 );

         DrawParagraphAt(g_smallFont.get(), g_smallFontSize, "It's dangerous to go alone!  You should immediately head to the \
merchant and buy a weapon, some armor and some health potions.  The easiest dungeon is the Collapsing \
Cave, just south of Emerald.  Even here you may be challenged; don't be afraid to run back to the \
surface to heal up.  Also, the Enchant Armor and Enchant Weapon spells are cheap and will help you a lot in the beginning.", 80, 341, 480, 12 );

 
         if( DrawButton("Previous Page", 405, 420 ) )
         {
            m_SecondPage = false;
         }

         if( DrawButton("Done", 540, 420 ) )
         {
            g_StateMachine->PopState();
         }
      }
   }
   else
   {
      DrawImageResizedAt(g_Mask, 40, 40, 560, 400 );
      DrawBoxAt(40, 40, 560, 400);

      DrawTextCenteredAt(g_font.get(), g_fontSize, "Welcome to Inaria, the land of Two Peoples.", 320, 45);

      DrawSpriteAt(g_Tiles[98], 70, 80 );

      DrawTextAt(g_smallFont.get(), g_smallFontSize, "The Humans of Inaria had always lived on its surface.  They were a simple", 120, 75);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, "people - farmers, fishermen, and miners.  They were ruled by a line of kings", 120, 87);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, "to the best of their abilities...and their abilities were sufficient to see", 120, 99);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, "Inaria grow into a thriving and prosperous (if small) kingdom.", 120, 111);

      DrawSpriteAt(g_Tiles[52], 500, 141 );

      DrawTextAt(g_smallFont.get(), g_smallFontSize, "The Slorn dwelt in the sunless depths below Inaria's surface, near the", 60, 136);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, "magma of an underground volcano, which gave them heat and light.", 60, 148);
      DrawTextAt(g_smallFont.get(), g_smallFontSize, "But their greatest gift was magic. The Slorn used magic to build", 60, 160 );
      DrawTextAt(g_smallFont.get(), g_smallFontSize, "a great underground civilization, whose people wanted for nothing.", 60, 172);

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "The two peoples lived in two different worlds that rarely met or", 320, 197);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "overlapped.  Of course, there had been animosity in the past but", 320, 209);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "the two lived as peacefully as they could given their differences.", 320, 221);

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "Until the day the Slorn crowned a new king, who pushed them to one", 320, 246);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "purpose: WAR.  No one knows why the new Slorn king chose this path,", 320, 258);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "but at his command a huge army boiled out of the volcano and swept", 320, 270);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "across Inaria.  Both Ruby and Sapphire fell, but the Slorn were", 320, 282);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "surprised at how ferociously the Inarians defended themselves.", 320, 294);

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "And then they arrived at the Castle of the Inarian King.  After a desperate", 320, 319);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "battle, the Slorn king and his army were defeated...but the Slorn king was", 320, 331);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "not killed.  He teleported back to his castle and began rebuilding his army.", 320, 343);

      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "You are a citizen of Emerald, the last free city of Inaria.  Throwing a coin", 320, 368);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "into the well for luck, you decide you must do something to help. The guards", 320, 380);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "are preparing their defense in the north, but the Collapsing Cave is to the", 320, 392);
      DrawTextCenteredAt(g_smallFont.get(), g_smallFontSize, "south...and the Slorn could use it to mount a surprise attack.", 320, 404);

      if( DrawButton("Let It Begin!", 440, 420 ) )
      {
         g_StateMachine->PopState();
      }
   }

   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());
}

void TutorialState::OnEnter()
{
   m_Quitting = false;
   m_Returning = false;
}

void TutorialState::OnExit()
{
   
}

void TutorialState::DoPlayerInput(){}

