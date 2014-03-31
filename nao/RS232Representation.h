/*
 * RS232Representation.h
 *
 *  Created on: Mar 20, 2014
 *      Author: sam
 */

#ifndef RS232REPRESENTATION_H_
#define RS232REPRESENTATION_H_

#if defined(TARGET_NAO)
#include "framework/Representation.h"
#else
#include "../Template.h"
#endif

#include <vector>
#include <iostream>

REPRESENTATION(RS232Representation)
class RS232Representation: public RS232RepresentationBase
{
  public:
    std::vector<float> pfInputs;

    void draw() const
    {
      if (false)
      {
        for (std::vector<float>::const_iterator iter = pfInputs.begin(); iter != pfInputs.end();
            ++iter)
          std::cout << *iter << " ";
        std::cout << std::endl;
      }
    }
};

#endif /* RS232REPRESENTATION_H_ */
