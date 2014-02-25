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
  //delay(2000); //<< SLOW DOWN to DEBUG 
}
