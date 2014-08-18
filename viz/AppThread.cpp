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
  Controller::getInstance().main(false);
}
