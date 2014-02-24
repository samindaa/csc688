/*
 * TestModule2.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: sam
 */

#include "TestModule2.h"

void TestModule2::execute()
{
#if defined(ENERGIA)
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
  else
  {
    static uint8_t state = HIGH;
    digitalWrite(RED_LED, state);
    digitalWrite(GREEN_LED, state);
    digitalWrite(BLUE_LED, state);
    delay(250);
    state ^= HIGH;
  }

  Serial.print("TMP: ");
  int32_t i32IntegerPart = (int32_t) theTestRepresentation2->ulTempValueC;
  int32_t i32FractionPart = (int32_t) (theTestRepresentation2->ulTempValueC * 1000.0f);
  i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
  if (i32FractionPart < 0)
  i32FractionPart *= -1;
  Serial.print(i32IntegerPart);
  Serial.print(".");
  Serial.println(i32FractionPart);

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

