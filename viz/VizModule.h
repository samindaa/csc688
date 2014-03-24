/*
 * VizModule.h
 *
 *  Created on: Mar 24, 2014
 *      Author: sam
 */

#ifndef VIZMODULE_H_
#define VIZMODULE_H_

#include <vector>
#include <QObject>
#include "VizObject.h"
#include "../Template.h"
#include "../nao/RS232Representation.h"

class VizQObject: public QObject
{
  Q_OBJECT
  public:
    void init();
    void update(const std::vector<float>& value);

  public:
  signals:
    void signals_draw(const std::vector<float>& value);
};

MODULE(VizModule)
  REQUIRES(RS232Representation)
END_MODULE
class VizModule: public VizModuleBase
{
  private:
    VizQObject* vizObject;
  public:
    VizModule();
    void init();
    void execute();
};


#endif /* VIZMODULE_H_ */
