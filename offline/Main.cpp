/*
 * Main.cpp
 *
 *  Created on: Feb 19, 2014
 *      Author: sam
 */

#include "../Framework.h"
#include <iostream>

int main(int argc, char** argv)
{
  std::cout << "*** start ***" << std::endl;
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  graph.stream();
  graph.graphOutputInit();
  for (int i = 0; i < 1000; i++)
    Graph::getInstance().graphOutputUpdate();
  std::cout << "*** end ***" << std::endl;
  return 0;
}

