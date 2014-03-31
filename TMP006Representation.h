/*
 * TMP006Representation.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef TMP006REPRESENTATION_H_
#define TMP006REPRESENTATION_H_

#include "Template.h"

REPRESENTATION(TMP006Representation)
class TMP006Representation: public TMP006RepresentationBase
{
  public:
    float fAmbient;
    TMP006Representation() :
        fAmbient(0)
    {
    }
};

#endif /* TMP006REPRESENTATION_H_ */
