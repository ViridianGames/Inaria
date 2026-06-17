#include "InfiniteWarningState.h"
#include "GameGlobals.h"

#include <list>
#include <string>
#include <sstream>
#include <math.h>
#include <fstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//  InfiniteWarningState
////////////////////////////////////////////////////////////////////////////////

InfiniteWarningState::~InfiniteWarningState()
{

}

void InfiniteWarningState::Init(const std::string& configfile)
{

}

void InfiniteWarningState::Shutdown(){}
void InfiniteWarningState::Update()
{

}

void InfiniteWarningState::Draw()
{

   DrawImageResizedAt(g_Mask, 40, 40, 560, 400 );
   DrawBoxAt(40, 40, 560, 400);

   DrawTextCenteredAt(g_font.get(), g_fontSize, "WARNING! You are about to enter THE INFINITE.", 320, 45);

   DrawParagraphAt(g_font.get(), g_fontSize, "The Infinite is a randomly-generated dungeon, with strong enemies \
and extra opportunities to gain items and money.  Once you enter The Infinite, you will not be able to \
save your game until you find the exit stairs and leave.  The random nature of the dungeon means that \
it can be brutally unfair, and thus we recommend that you save before entering." , 80, 66, 480, 16);

   DrawTextCenteredAt(g_font.get(), g_fontSize, "Do you still wish to enter The Infinite?", 320, 200);


   if( DrawButton("Yes!", 405, 420 ) )
   {
      g_InfiniteYes = 2;
     g_StateMachine->PopState();
   }

   if( DrawButton("No...", 540, 420 ) )
   {
      g_InfiniteYes = 1;
      g_StateMachine->PopState();
   }

   DrawImageAt(g_Cursors[0], GetDesignMouseX(), GetDesignMouseY());
}

void InfiniteWarningState::OnEnter()
{

}

void InfiniteWarningState::OnExit()
{
   
}

void InfiniteWarningState::DoPlayerInput(){}

