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
    int16_t temperature;      // last calculated temperature in 0.1 C (or F)
    int32_t pressure;         // last calculated pressure in Pa
    uint16_t rawTemperature;  // last measured temperature
    uint32_t rawPressure;     // last measured pressure

    BMP180Representation() :
        temperature(0), pressure(0), rawTemperature(0), rawPressure(0)
    {
    }
};

#endif /* BMP180REPRESENTATION_H_ */
