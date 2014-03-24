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

void VizObject::push_back(Plot* obj)
{
  objects.push_back(obj);
}

std::vector<Plot*>& VizObject::getObjects()
{
  return objects;
}

