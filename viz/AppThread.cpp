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

void AppThread::run()
{
  Controller::getInstance().setup(0);
  for (;;)
  {
    Controller::getInstance().loop();
    QThread::msleep(2); // Give other threads some chance to run
  }
}
