/*
 * AppThread.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#include "AppThread.h"

AppThread::AppThread(QObject * parent) :
    QThread(parent)
{
}

AppThread::~AppThread()
{
}

void AppThread::init()
{
  Graph& graph = Graph::getInstance();
  graph.computeGraph();
  graph.topoSort();
  graph.stream();
  graph.graphOutputInit();
}

void AppThread::run()
{
  while (true)
    Graph::getInstance().graphOutputUpdate();
}
