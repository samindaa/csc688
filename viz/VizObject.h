/*
 * VizObject.h
 *
 *  Created on: Mar 24, 2014
 *      Author: sam
 */

#ifndef VIZOBJECT_H_
#define VIZOBJECT_H_

#include <vector>
#include "Plot.h"

class VizObject
{
  private:
    std::vector<Plot*> objects;
  public:
    static VizObject& getInstance();
    void push_back(Plot* obj);
    std::vector<Plot*>& getObjects();

  protected:
    VizObject();
    ~VizObject();
    VizObject(VizObject const&);
    VizObject& operator=(VizObject const&);
};

#endif /* VIZOBJECT_H_ */
