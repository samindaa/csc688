/*
 * Window.h
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <QWidget>
#include <QGridLayout>
#include "Plot.h"

class Window: public QWidget
{
Q_OBJECT
protected:
  QGridLayout* grid;
  Plot* plot;

public:
  explicit Window(QWidget *parent = 0);
  virtual ~Window();
  Plot* getPlot() const;
};

#endif /* WINDOW_H_ */
