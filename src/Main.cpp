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
  std::cout << "*** starts ****" << std::endl; // prints !!!Hello World!!!
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  std::cout << graph << std::endl;
  graph.graphOutputInit();
  graph.graphOutputUpdate();
  std::cout << "*** ends ****" << std::endl;
  return 0;
}
