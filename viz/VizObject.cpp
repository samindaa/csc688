/*
 * VizObject.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: sam
 */

#include "VizObject.h"

VizObject::VizObject()
{
}

VizObject::~VizObject()
{
  objects.clear();
}

VizObject& VizObject::getInstance()
{
  static VizObject theInstance;
  return theInstance;
}

void VizObject::push_back(QObject* obj)
{
  objects.push_back(obj);
}

std::vector<QObject*>& VizObject::getObjects()
{
  return objects;
}

