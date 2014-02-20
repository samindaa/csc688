/*
 * Main.cpp
 *
 *  Created on: Feb 19, 2014
 *      Author: sam
 */

#ifdef OFFLINE
#include "Framework.h"

int main(int argc, char** argv)
{
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  graph.stream();
  graph.graphOutputInit();
  Graph::getInstance().graphOutputUpdate();
  return 0;
}
#endif

