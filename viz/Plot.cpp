/*
 * Plot.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#include "Plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent)
{
  grid = new QHBoxLayout(this);
  plot = new QCustomPlot(this);
  plot->setGeometry(0, 0, sizeHint().width(), sizeHint().height());
  grid->addWidget(plot);

  x.resize(sizeHint().width());

  for (int i = 0; i < x.size(); i++)
    x[i] = i;

  nbPlots.push_back(Qt::lightGray);
  nbPlots.push_back(Qt::red);
  nbPlots.push_back(Qt::green);
  nbPlots.push_back(Qt::blue);

  nbPlots.push_back(Qt::black);
  nbPlots.push_back(Qt::darkRed);
  nbPlots.push_back(Qt::darkGreen);
  nbPlots.push_back(Qt::darkBlue);

  nbPlots.push_back(Qt::cyan);
  nbPlots.push_back(Qt::magenta);
  nbPlots.push_back(Qt::yellow);
  nbPlots.push_back(Qt::gray);

  nbPlots.push_back(Qt::darkCyan);
  nbPlots.push_back(Qt::darkMagenta);
  nbPlots.push_back(Qt::darkYellow);
  nbPlots.push_back(Qt::darkGray);

  // Up-to-4 graphs
  for (int i = 0; i < (int) nbPlots.size(); i++)
  {
    yValues.push_back(QVector<double>());
    yValues[i].resize(sizeHint().width());
    for (int j = 0; j < sizeHint().width(); j++)
      yValues[i][j] = 0.0f;
  }

  // create graph and assign data to it:
  for (int i = 0; i < (int) nbPlots.size(); i++)
  {
    plot->addGraph();
    plot->graph(i)->setPen(QPen(nbPlots[i]));
  }

  plot->xAxis->setLabel("Time");
  plot->yAxis->setLabel("Values");
  setLayout(grid);
}

Plot::~Plot()
{
}

void Plot::slots_draw(const std::vector<float>& pfInput)
{
  for (int i = 1; i < x.size(); i++)
    x[i - 1] = x[i];
  ++x[x.size() - 1];
  // O(N)
  for (size_t i = 0; i < yValues.size(); i++)
  {
    for (int j = 1; j < yValues[i].size(); j++)
      yValues[i][j - 1] = yValues[i][j];
  }

  for (int i = 0; i < std::min((int) pfInput.size(), (int) nbPlots.size()); i++)
  {
    yValues[i][yValues[i].size() - 1] = pfInput[i];
    plot->graph(i)->setData(x, yValues[i]);
    plot->graph(i)->rescaleAxes();
  }
  plot->xAxis->setRange(x[0], x[x.size() - 1]);
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

