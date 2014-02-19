#include "Framework.h"

void setup()
{
  // put your setup code here, to run once:
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  graph.graphOutputInit();
}

void loop()
{
  // put your main code here, to run repeatedly:
  graph.graphOutputUpdate();
}