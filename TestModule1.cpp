/*
 * TestModule1.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: sam
 */

#include "TestModule1.h"

void TestModule1::init()
{
  //pinMode(PUSH1, INPUT_PULLUP); // left - note _PULLUP
  pinMode(PUSH2, INPUT_PULLUP); // right - note _PULLUP
}

void TestModule1::execute()
{
}

void TestModule1::update(TestRepresentation1& theTestRepresentation1)
{
  theTestRepresentation1.blinkfast = theTestRepresentation1.blinkslow = false;
  //if (digitalRead(PUSH1) == LOW)
  //  theTestRepresentation1.blinkslow = true;
  if (digitalRead(PUSH2) == LOW)
   theTestRepresentation1.blinkfast = true;
}

MAKE_MODULE(TestModule1)

