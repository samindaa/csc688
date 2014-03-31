/*
 * TestModule1.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: sam
 */

#include "TestModule1.h"

void TestModule1::update(TestRepresentation1& theTestRepresentation1)
{
  theTestRepresentation1.blinkfast = theTestRepresentation1.blinkslow = false;
#if defined(ENERGIA)
  if (digitalRead(PUSH1) == LOW)
    theTestRepresentation1.blinkslow = true;
  if (digitalRead(PUSH2) == LOW)
    theTestRepresentation1.blinkfast = true;
#endif
}

MAKE_MODULE(TestModule1)

