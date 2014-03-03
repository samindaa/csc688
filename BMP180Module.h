/*
 * BMP180Module.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Saminda
 */

#ifndef BMP180MODULE_H_
#define BMP180MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "BMP180Representation.h"
#include "hw_bmp180.h"


//*****************************************************************************
//
// Define BMP180 I2C Address.
//
//*****************************************************************************
#define BMP180_I2C_ADDRESS      0x77

MODULE(BMP180Module)
  REQUIRES(LaunchPadRepresentation)
  PROVIDES(BMP180Representation)
END_MODULE
class BMP180Module: public BMP180ModuleBase
{
  private:
    class BMP180
    {
      public:
        //
        // The I2C address of the BMP180.
        //
        uint8_t ui8Addr;

        //
        // The AC1 calibration from the BMP180.
        //
        int16_t i16AC1;

        //
        // The AC2 calibration from the BMP180.
        //
        int16_t i16AC2;

        //
        // The AC3 calibration from the BMP180.
        //
        int16_t i16AC3;

        //
        // The AC4 calibration from the BMP180.
        //
        uint16_t ui16AC4;

        //
        // The AC5 calibration from the BMP180.
        //
        uint16_t ui16AC5;

        //
        // The AC6 calibration from the BMP180.
        //
        uint16_t ui16AC6;

        //
        // The B1 calibration from the BMP180.
        //
        int16_t i16B1;

        //
        // The B2 calibration from the BMP180.
        //
        int16_t i16B2;

        //
        // The MC calibration from the BMP180.
        //
        int16_t i16MC;

        //
        // The MD calibration from the BMP180.
        //
        int16_t i16MD;

        //
        // The sampling mode to be used by the BMP180.
        //
        uint8_t ui8Mode;

        //
        // The data buffer used for sending/receiving data to/from the BMP180.
        //
        uint8_t pui8Data[3];

        BMP180() :
            ui8Addr(BMP180_I2C_ADDRESS), i16AC1(0), i16AC2(0), i16AC3(0), ui16AC4(0), ui16AC5(0), ui16AC6(
                0), i16B1(0), i16B2(0), i16MC(0), i16MD(0), ui8Mode(0)
        {
        }
    };

    BMP180 parameters;

  public:
    void init();
    void update(BMP180Representation& theBMP180Representation);

  private:
    void calibration();
    void calculation(BMP180Representation& theBMP180Representation);
    // read 16-bits from I2C (signed and unsigned)
    void I2CMRead(const uint8_t& addr, const uint8_t& bytes = 2);
    void cmdI2CMRead(const uint8_t& cmd, const uint8_t& addr, const uint8_t& bytes);
};

#endif /* BMP180MODULE_H_ */
