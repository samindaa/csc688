/*
 * TestModule1.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: sam
 */

#include "TestModule1.h"

void TestModule1::update(TestRepresentation1& theTestRepresentation1)
{
  theTestRepresentation1.leftButton = theTestRepresentation1.rightButton = false;
#if defined(ENERGIA)
  if (digitalRead(PUSH1) == LOW)
    theTestRepresentation1.leftButton = true;
  if (digitalRead(PUSH2) == LOW)
    theTestRepresentation1.rightButton = true;
#endif
}

MAKE_MODULE(TestModule1)

