/*
 * TestModule2.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: sam
 */

#include "TestModule2.h"

void TestModule2::init()
{
#ifndef OFFLINE
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
#endif
}

void TestModule2::execute()
{
#ifndef OFFLINE
  if (theTestRepresentation1->blinkslow)
    blinkslow();
  else if (theTestRepresentation1->blinkfast)
    blinkfast();
  else if (theTestRepresentation2->ulTempValueC > 0)
  {
    // blink green led
    static int toggle = HIGH;
    digitalWrite(GREEN_LED, toggle);
    toggle ^= HIGH;
    delay(250);
  }
#endif

}

void TestModule2::blinkfast()
{
#ifndef OFFLINE
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
#ifndef OFFLINE
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

