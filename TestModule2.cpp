/*
 * TestModule2.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: sam
 */

#include "TestModule2.h"

void TestModule2::init()
{
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  //pinMode(BLUE_LED, OUTPUT);
}

void TestModule2::execute()
{
  //if (theTestRepresentation1->blinkslow)
  //  blinkslow();
  if (theTestRepresentation1->blinkfast)
    blinkfast();
  

}

void TestModule2::blinkfast()
{
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(RED_LED, HIGH);
    delay(250);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(250);
    digitalWrite(GREEN_LED, LOW);
    //digitalWrite(BLUE_LED, HIGH);
    delay(250);
    //digitalWrite(BLUE_LED, LOW);
  }
}
void TestModule2::blinkslow()
{
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(RED_LED, HIGH);
    delay(1000);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    delay(1000);
    digitalWrite(GREEN_LED, LOW);
    //digitalWrite(BLUE_LED, HIGH);
    delay(1000);
    //digitalWrite(BLUE_LED, LOW);
  }
}

MAKE_MODULE(TestModule2)

