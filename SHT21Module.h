/*
 * SHT21Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef SHT21MODULE_H_
#define SHT21MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "SHT21Representation.h"
#include "hw_sht21.h"

//*****************************************************************************
//
// Define SHT21 I2C Address.
//
//*****************************************************************************
#define SHT21_I2C_ADDRESS  0x40

MODULE(SHT21Module)
  REQUIRES(LaunchPadRepresentation) //
  PROVIDES(SHT21Representation)
END_MODULE
class SHT21Module: public SHT21ModuleBase
{
  private:
    class SHT21
    {
      public:
        //
        // the I2C address of the SHT21.
        //
        uint8_t ui8Addr;

        //
        // the 16 bit raw temperature reading
        //
        uint16_t ui16Temperature;

        //
        // the 16 bit raw humidity reading
        //
        uint16_t ui16Humidity;

        SHT21() :
            ui8Addr(SHT21_I2C_ADDRESS), ui16Temperature(0), ui16Humidity(0)
        {
        }
    };

    SHT21 parameters;

  public:
    void init();
    void update(SHT21Representation& theSHT21Representation);
};

#endif /* SHT21MODULE_H_ */
