/*
 * TestModule5.cpp
 *
 *  Created on: Aug 27, 2014
 *      Author: sam
 */

#include "TestModule5.h"

//MAKE_MODULE(TestModule5)

TestModule5::TestModule5() :
    ledState(0x1), prevTime(0)
{
}

void TestModule5::init()
{
#if defined(ENERGIA)
  //ButtonsInit();
//  LPRFInit();
#endif
}

void TestModule5::execute()
{
#if defined(ENERGIA)
  // Toggle LED
  /*if (millis() - prevTime > 250)
   {
   prevTime = millis();
   ledState ^= HIGH;
   }

   digitalWrite(RED_LED, LOW);
   digitalWrite(GREEN_LED, LOW); // ledState
   digitalWrite(BLUE_LED, LOW);
*/
#endif

#if defined(ENERGIA)
//  LPRFMain();
#endif

}

