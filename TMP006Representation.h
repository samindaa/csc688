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
    float fObject;
    int_fast32_t i32IntegerPart;
    int_fast32_t i32FractionPart;
    TMP006Representation() :
        fAmbient(0), fObject(0), i32IntegerPart(0), i32FractionPart(0)
    {
    }
};

#endif /* TMP006REPRESENTATION_H_ */
