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
}
