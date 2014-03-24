/*
 * AppThread.h
 *
 *  Created on: Jan 30, 2014
 *      Author: sam
 */

#ifndef APPTHREAD_H_
#define APPTHREAD_H_

#include <QThread>
#include "../Framework.h"

class AppThread: public QThread
{
  Q_OBJECT

  public:
    explicit AppThread(QObject * parent = 0);
    virtual ~AppThread();
    void init();

  protected:
    void run();
};

#endif /* APPTHREAD_H_ */
