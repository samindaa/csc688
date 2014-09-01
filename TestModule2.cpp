/*
 * TestModule2.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: sam
 */

#include "TestModule2.h"

MAKE_MODULE(TestModule2)

TestModule2::TestModule2() :
    ledState(0x1), prevTime(0), collectData(false)
{
}

void TestModule2::execute()
{
#if defined(ENERGIA)
  // Toggle LED
  if (millis() - prevTime > 250)
  {
    prevTime = millis();
    ledState ^= HIGH;
  }

  if (theTestRepresentation1->rightButton)
    collectData = true;
  else if (theTestRepresentation1->leftButton)
    collectData = false;

  if (collectData)
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, ledState);
  }

  /*if (collectData)
   {
   digitalWrite(RED_LED, ledState);
   digitalWrite(GREEN_LED, ledState);
   digitalWrite(BLUE_LED, ledState);
   }
   else if (theISL29023Representation->fAmbient < 10) // dark
   {
   digitalWrite(RED_LED, ledState);
   digitalWrite(GREEN_LED, ledState);
   digitalWrite(BLUE_LED, ledState);
   }
   else
   {
   digitalWrite(RED_LED, LOW);
   digitalWrite(GREEN_LED, LOW);
   digitalWrite(BLUE_LED, ledState);
   }*/
#endif

}

void TestModule2::update(TestRepresentation2& theTestRepresentation2)
{
  theTestRepresentation2.collectData = collectData;
}

void TestModule2::update(TestRepresentation3& theTestRepresentation3)
{
}

