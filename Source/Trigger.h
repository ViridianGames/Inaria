
#ifndef _Trigger_H_
#define _Trigger_H_

#include "BaseUnit.h"

class Trigger: public BaseUnit
{
public:
   Trigger();
   virtual ~Trigger();
   void OnTalk();
   void OnLook();
   void SetPos(int x, int y);
   
   virtual void OnUse();
   virtual void OnPickup();
   virtual void OnStand();
   virtual void OnAttack();



   virtual void Init(const std::string& configfile);
   virtual void Shutdown();
   virtual void Update();
   virtual void Draw();


};


#endif