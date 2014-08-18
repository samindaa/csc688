#include <Wire.h>
#include "Framework.h"

void setup()
{
  // Default baud rate is set to 115200.
  // If this value needs to be changed, uncomment following line:
  // Controller::getInstance().setBaudRate(9600);
  Controller::getInstance().setup(); 
}

void loop()
{
  Controller::getInstance().loop();
}
