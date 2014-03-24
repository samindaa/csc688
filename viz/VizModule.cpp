/*
 * VizModule.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: sam
 */

#include "VizModule.h"

void VizQObject::init()
{
  std::vector<QObject*>& objs = VizObject::getInstance().getObjects();
  for (std::vector<QObject*>::iterator iter = objs.begin(); iter != objs.end(); ++iter)
    connect(this, SIGNAL(signals_draw(const std::vector<float>&)), *iter,
        SLOT(slots_draw(const std::vector<float>&)));
}

void VizQObject::update(const std::vector<float>& value)
{
  emit signals_draw(value);
}

VizModule::VizModule() :
    vizObject(0)
{
}

void VizModule::init()
{
  vizObject = new VizQObject;
  vizObject->init();
}

void VizModule::execute()
{
  vizObject->update(theRS232Representation->pfInputs);
}

MAKE_MODULE(VizModule)
