/*
 * BMP180Representation.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Saminda
 */

#ifndef BMP180REPRESENTATION_H_
#define BMP180REPRESENTATION_H_

#include "Template.h"

REPRESENTATION(BMP180Representation)
class BMP180Representation: public BMP180RepresentationBase
{
  public:
    float fTemperature, fPressure, fAltitude;

    BMP180Representation() :
        fTemperature(0), fPressure(0), fAltitude(0)
    {
    }
};

#endif /* BMP180REPRESENTATION_H_ */
