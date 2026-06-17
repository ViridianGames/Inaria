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
   if( g_Input->WasKeyPressed(SDLK_ESCAPE) )
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

         g_Display->BlitImageResized(g_Mask, 40, 40, 560, 400 );
         g_Display->DrawBox(40, 40, 560, 400);

         g_Font->DrawTextCentered("Welcome to Inaria!", 320, 45);

         g_SmallFont->DrawTextCentered( "This is a brief tutorial on how to play Inaria.", 320, 70);

         g_SmallFont->DrawTextCentered( "YOUR CHARACTER", 320, 92);

         g_Display->DrawSprite( g_Tiles[64], 60, 100);
         g_SmallFont->DrawTextA( "Strength - Determines your damage and how often you hit in melee combat.", 80, 104);

         g_Display->DrawSprite( g_Tiles[107], 60, 118);
         g_SmallFont->DrawTextA( "Dexterity - Determines critical hits and how often you hit combat.", 80, 122);

         g_Display->DrawSprite( g_Tiles[108], 60, 136);
         g_SmallFont->DrawTextA( "Endurance - Determines how many extra hitpoints you gain when you level up.", 80, 140);

         g_Display->DrawSprite( g_Tiles[109], 60, 154);
         g_SmallFont->DrawTextA( "Intelligence - Determines what spells you can cast and spell combat.", 80, 158);

         g_Display->DrawSprite( g_Tiles[110], 60, 172);
         g_SmallFont->DrawTextA( "Will - Determines spell combat effectiveness and your mana points.", 80, 176);

         g_SmallFont->DrawText( "Every time your character levels up, you will be given skill points you can use", 80, 198);

         g_SmallFont->DrawTextA( "to improve your stats or buy special abilities (more on that below).", 80, 210 );

         int controlstart = 236;

         g_SmallFont->DrawTextCentered( "CONTROLLING THE GAME", 320, controlstart);

         g_SmallFont->DrawTextA( "You can move around three ways: left-click on the map in the direction you want", 80, controlstart + 12);
         g_SmallFont->DrawTextA( "to go, use the WASD or arrow keys, or use the following key layouts to move:", 80, controlstart + 24);;//WASD or arrow keys to move.  The four standard icons in the", 80, 272);
         g_SmallFixedFont->DrawTextA( "               q  w  e              7  8  9", 80, controlstart + 40);
         g_SmallFixedFont->DrawTextA( "                \\ | /                \\ | / ", 80, controlstart + 52);         
         g_SmallFixedFont->DrawTextA( "               a- . -d              4- . -6", 80, controlstart + 64);             
         g_SmallFixedFont->DrawTextA( "                / | \\                / | \\ ", 80, controlstart + 76);                
         g_SmallFixedFont->DrawTextA( "               z  x  c              1  2  3", 80, controlstart + 88);

         g_SmallFont->DrawTextA( "You can pass your turn by pressing the space bar or 5 on the keypad. The four", 80, controlstart + 104);
            
         g_SmallFont->DrawTextA( "standard icons in the bottom-right allow you to pick things up, attack, talk to", 80, controlstart + 116);

         g_SmallFont->DrawTextA( "and look at things.  If you don't want to use those icons, you can simply right-click", 80, controlstart + 128);

         g_SmallFont->DrawTextA( "people something and the game will do the \"right\" thing according to what you just ", 80, controlstart + 140); 

         g_SmallFont->DrawTextA( "clicked on - talking to friendlies, melee attacking enemies and picking up objects.", 80, controlstart + 152);

//         g_SmallFont->DrawTextA( "", 80, controlstart + 164);


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
         g_Display->BlitImageResized(g_Mask, 40, 40, 560, 400 );
         g_Display->DrawBox(40, 40, 560, 400);


         g_SmallFont->DrawTextCentered( "INVENTORY", 320, 50);

         g_SmallFont->DrawParagraph("As you travel through the game, you may purchase or find items.  \
These will go into your inventory.   You can left-click on inventory items to use or equip them.  \
You can equip up to three different kinds of items - weapons, armor and trinkets.  All three can be \
enchanted and add bonuses to your base stats.  Click an equipped item to unequip it and put it back \
in your inventory if you have room for it.", 80, 66, 480, 12);

         g_SmallFont->DrawTextCentered( "LEVELING UP", 320, 140 );

         g_SmallFont->DrawParagraph("When you level up, you will be given a certain number of skill \
points that you can use to improve your stats or buy special abilities.  As you improve stats, the \
cost to improve them again goes up.  Abilities have a base cost to buy, and some abilities can be \
improved at a higher cost.  You don't have to spend all your skill points right away.  Any unspent \
points are saved and you can spend them anytime later.", 80, 156, 480, 12 );

         g_SmallFont->DrawTextCentered( "ABILITIES", 320, 230 );
            
         g_SmallFont->DrawParagraph("You can switch between your inventory and your ability list by \
clicking the tabs above them.  You start out with a single ability - Smite, a magical ranged attack.  \
As you level up, you can unlock new abilities.  Some abilities are passive and give you permanent \
bonuses.  Some can be used by left-clicking.  Some (like Smite) require you to target something in \
the world.  You can mouse over each ability's name to get a short description.", 80, 246, 480, 12);

         g_SmallFont->DrawTextCentered( "TIPS FOR STARTING OUT", 320, 325 );

         g_SmallFont->DrawParagraph("It's dangerous to go alone!  You should immediately head to the \
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
      g_Display->BlitImageResized(g_Mask, 40, 40, 560, 400 );
      g_Display->DrawBox(40, 40, 560, 400);

      g_Font->DrawTextCentered("Welcome to Inaria, the land of Two Peoples.", 320, 45);

      g_Display->DrawSprite(g_Tiles[98], 70, 80 );

      g_SmallFont->DrawText("The Humans of Inaria had always lived on its surface.  They were a simple", 120, 75);
      g_SmallFont->DrawText("people - farmers, fishermen, and miners.  They were ruled by a line of kings", 120, 87);
      g_SmallFont->DrawText("to the best of their abilities...and their abilities were sufficient to see", 120, 99);
      g_SmallFont->DrawText("Inaria grow into a thriving and prosperous (if small) kingdom.", 120, 111);

      g_Display->DrawSprite(g_Tiles[52], 500, 141 );

      g_SmallFont->DrawText("The Slorn dwelt in the sunless depths below Inaria's surface, near the", 60, 136);
      g_SmallFont->DrawText("magma of an underground volcano, which gave them heat and light.", 60, 148);
      g_SmallFont->DrawText("But their greatest gift was magic. The Slorn used magic to build", 60, 160 );
      g_SmallFont->DrawText("a great underground civilization, whose people wanted for nothing.", 60, 172);

      g_SmallFont->DrawTextCentered("The two peoples lived in two different worlds that rarely met or", 320, 197);
      g_SmallFont->DrawTextCentered("overlapped.  Of course, there had been animosity in the past but", 320, 209);
      g_SmallFont->DrawTextCentered("the two lived as peacefully as they could given their differences.", 320, 221);

      g_SmallFont->DrawTextCentered("Until the day the Slorn crowned a new king, who pushed them to one", 320, 246);
      g_SmallFont->DrawTextCentered("purpose: WAR.  No one knows why the new Slorn king chose this path,", 320, 258);
      g_SmallFont->DrawTextCentered("but at his command a huge army boiled out of the volcano and swept", 320, 270);
      g_SmallFont->DrawTextCentered("across Inaria.  Both Ruby and Sapphire fell, but the Slorn were", 320, 282);
      g_SmallFont->DrawTextCentered("surprised at how ferociously the Inarians defended themselves.", 320, 294);

      g_SmallFont->DrawTextCentered("And then they arrived at the Castle of the Inarian King.  After a desperate", 320, 319);
      g_SmallFont->DrawTextCentered("battle, the Slorn king and his army were defeated...but the Slorn king was", 320, 331);
      g_SmallFont->DrawTextCentered("not killed.  He teleported back to his castle and began rebuilding his army.", 320, 343);

      g_SmallFont->DrawTextCentered("You are a citizen of Emerald, the last free city of Inaria.  Throwing a coin", 320, 368);
      g_SmallFont->DrawTextCentered("into the well for luck, you decide you must do something to help. The guards", 320, 380);
      g_SmallFont->DrawTextCentered("are preparing their defense in the north, but the Collapsing Cave is to the", 320, 392);
      g_SmallFont->DrawTextCentered("south...and the Slorn could use it to mount a surprise attack.", 320, 404);

      if( DrawButton("Let It Begin!", 440, 420 ) )
      {
         g_StateMachine->PopState();
      }
   }

   g_Display->BlitImage(g_Cursors[0], g_Input->m_MouseX, g_Input->m_MouseY);
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

