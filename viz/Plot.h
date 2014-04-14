/*
 * Plot.h
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#ifndef PLOT_H_
#define PLOT_H_

#include <QWidget>
#include <QHBoxLayout>
#include <vector>
#include <QVector>
#include "plot/qcustomplot.h"

class Plot: public QWidget
{
Q_OBJECT
protected:
  QHBoxLayout* grid;
  QCustomPlot* plot;
  QVector<double> x;

  std::vector<QVector<double> > yValues;
  std::vector<Qt::GlobalColor> nbPlots;

public:
  Plot(QWidget *parent = 0);
  virtual ~Plot();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

public slots:
  void slots_draw(const std::vector<float>& pfInput);

};

#endif /* PLOT_H_ */
