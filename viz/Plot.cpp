/*
 * Plot.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#include "Plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent), nbPlots(10)
{
  grid = new QHBoxLayout(this);
  plot = new QCustomPlot(this);
  plot->setGeometry(0, 0, sizeHint().width(), sizeHint().height());
  grid->addWidget(plot);

  x.resize(sizeHint().width());

  for (int i = 0; i < x.size(); i++)
    x[i] = i;

  globalColors.push_back(Qt::red);
  globalColors.push_back(Qt::green);
  globalColors.push_back(Qt::blue);
  globalColors.push_back(Qt::cyan);
  globalColors.push_back(Qt::magenta);

  globalColors.push_back(Qt::darkRed);
  globalColors.push_back(Qt::darkGreen);
  globalColors.push_back(Qt::darkBlue);
  globalColors.push_back(Qt::darkCyan);
  globalColors.push_back(Qt::darkMagenta);

  // Up-to-4 graphs
  for (int i = 0; i < nbPlots; i++)
  {
    yValues.push_back(QVector<double>());
    yValues[i].resize(sizeHint().width());
    for (int j = 0; j < sizeHint().width(); j++)
      yValues[i][j] = 0.0f;
  }

  // create graph and assign data to it:
  for (int i = 0; i < nbPlots; i++)
    plot->addGraph();

  for (int i = 0; i < nbPlots; i++)
    plot->graph(i)->setPen(QPen(globalColors[i]));

  plot->xAxis->setLabel("X");
  plot->yAxis->setLabel("Y");
  setLayout(grid);
}

Plot::~Plot()
{
}

void Plot::slots_draw(const std::vector<float>& pfInput)
{
  // O(N)
  for (size_t i = 0; i < yValues.size(); i++)
  {
    for (int j = 1; j < yValues[i].size(); j++)
      yValues[i][j - 1] = yValues[i][j];
  }

  for (int i = 0; i < std::min((int) pfInput.size(), nbPlots); i++)
  {
    yValues[i][yValues[i].size() - 1] = pfInput[i];
    plot->graph(i)->setData(x, yValues[i]);
    plot->graph(i)->rescaleAxes();
  }
  plot->rescaleAxes();
  plot->replot();
}

QSize Plot::minimumSizeHint() const
{
  return QSize(200, 200);
}

QSize Plot::sizeHint() const
{
  return QSize(400, 400);
}

