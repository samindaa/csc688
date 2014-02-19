//============================================================================
// Name        : Main.cpp
// Author      : Sam Abeyruwan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Framework.h"

int main()
{
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  graph.graphOutputInit();
  int loop = 0;
  while (loop++ < 100)
    graph.graphOutputUpdate();
  return 0;
}
