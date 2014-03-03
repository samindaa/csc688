#include <Wire.h>
#include "Framework.h"

unsigned long dutyCycleTime = 100; // default is set to 100 ms

void setup()
{
  Graph& graph = Graph::getInstance();
  // Default baud rate is set to 115200.
  // If this value needs to be changed, uncomment following line
  // graph.setBaudRate(9600);
  graph.computeGraph();
  graph.topoSort();
  graph.graphOutputInit();
}

void loop()
{
  unsigned long startTime = millis();
  Graph::getInstance().graphOutputUpdate();
  unsigned long elapsedTime = millis() - startTime;
  if (elapsedTime < dutyCycleTime)
    delay(dutyCycleTime - elapsedTime);
  //DEBUG
  //Serial.print("elapsedTime=");
  //Serial.println(elapsedTime);

  //delay(2000); //<< SLOW DOWN to DEBUG 
}
