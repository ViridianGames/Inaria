
#include "GameGlobals.h"
#include <list>
#include <algorithm>

typedef BaseUnit parent;

using namespace std;

Trigger::Trigger()
{

}

Trigger::~Trigger()
{
   parent::Shutdown();
   Shutdown();
}

void Trigger::Init(const std::string& configfile)
{

}

void Trigger::Shutdown()
{
   parent::Shutdown();

}

void Trigger::Update()
{
   parent::Update();
}

void Trigger::Draw()
{

}

void Trigger::OnTalk()
{
   AddConsoleString("You cannot talk to that.");
}

void Trigger::OnLook()
{

}

void Trigger::SetPos(int x, int y)
{
}

void Trigger::OnUse()
{
}

void Trigger::OnStand()
{
}

void Trigger::OnAttack()
{
}

void Trigger::OnPickup()
{
}

