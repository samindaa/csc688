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

#include <map>
#include <vector>

REPRESENTATION(RS232Representation)
class RS232Representation: public RS232RepresentationBase
{
  public:
    // There may be multiple reading per cycle,
    // if the host machine is fast.
    typedef std::map<size_t, std::vector<float> > Inputs;
    Inputs pfInputs;
};

#endif /* RS232REPRESENTATION_H_ */
