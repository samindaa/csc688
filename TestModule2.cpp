/*
 * TestModule2.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: sam
 */

#include "TestModule2.h"

TestModule2::TestModule2() :
    ledState(0x01), prevTime(0)
{
}

void TestModule2::execute()
{
#if defined(ENERGIA)
  // blink green led
  if (millis() - prevTime > 250)
  {
    prevTime = millis();
    ledState ^= HIGH;
  }

  if (theTestRepresentation1->blinkslow)
    blinkslow();
  else if (theTestRepresentation1->blinkfast)
    blinkfast();
  else if (theISL29023Representation->fAmbient < 10) // dark
  {
    digitalWrite(RED_LED, ledState);
    digitalWrite(GREEN_LED, ledState);
    digitalWrite(BLUE_LED, ledState);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, ledState);
    digitalWrite(BLUE_LED, LOW);
  }
#endif

}

void TestModule2::blinkfast()
{
#if defined(ENERGIA)
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(RED_LED, HIGH);
    delay(250);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(250);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    delay(250);
    digitalWrite(BLUE_LED, LOW);
  }
#endif
}
void TestModule2::blinkslow()
{
#if defined(ENERGIA)
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(RED_LED, HIGH);
    delay(1000);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(1000);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
    delay(1000);
    digitalWrite(BLUE_LED, LOW);
  }
#endif
}

MAKE_MODULE(TestModule2)

