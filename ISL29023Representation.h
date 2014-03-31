/*
 * ISL29023Representation.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef ISL29023REPRESENTATION_H_
#define ISL29023REPRESENTATION_H_

#include "Template.h"

REPRESENTATION(ISL29023Representation)
class ISL29023Representation: public ISL29023RepresentationBase
{
  public:
    float fAmbient;
    ISL29023Representation() :
        fAmbient(0)
    {
    }

};

#endif /* ISL29023REPRESENTATION_H_ */
