#include <Wire.h>
#include "Framework.h"

void setup()
{
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  graph.graphOutputInit();
}

void loop()
{
  Graph::getInstance().graphOutputUpdate();
  Serial.println("*** cycle ***");
  delay(100); // ADC to recover
}
