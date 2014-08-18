//============================================================================
// Name        : MSP430G2553_graphs.cpp
// Author      : Sam Abeyruwan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <QApplication>
#include "Window.h"
#include "AppThread.h"
#include "VizObject.h"

Q_DECLARE_METATYPE(std::vector<float>)

int main(int argc, char** argv)
{
  QApplication a(argc, argv);
  qRegisterMetaType<std::vector<float> >();

  Window* window = new Window;
  window->setWindowTitle("Viz");
  window->show();
  VizObject::getInstance().push_back(window->getPlot());

  AppThread* appThread = new AppThread;
  appThread->start();

  return a.exec();
}
