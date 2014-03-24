/*
 * Window.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#include "Window.h"

Window::Window(QWidget *parent) :
    QWidget(parent), grid(new QGridLayout(this)), plot(new Plot(this))
{
  grid->addWidget(plot);
  setLayout(grid);
}

Window::~Window()
{
}

Plot* Window::getPlot() const
{
  return plot;
}

