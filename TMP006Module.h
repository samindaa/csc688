/*
 * TMP006Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef TMP006MODULE_H_
#define TMP006MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "TMP006Representation.h"
#include "hw_tmp006.h"

//*****************************************************************************
//
// Define TMP006 I2C Address.
//
//*****************************************************************************
#define TMP006_I2C_ADDRESS      0x41

MODULE(TMP006Module)
  REQUIRES(LaunchPadRepresentation) //
  PROVIDES(TMP006Representation)
END_MODULE
class TMP006Module: public TMP006ModuleBase
{
  private:
    class TMP006
    {
      public:
        //
        // The I2C address of the TMP006.
        //
        uint8_t ui8Addr;

        //
        // Calibration factor.  Left to application to implement calibration
        // See term S0 in http://www.ti.com/lit/ug/sbou107/sbou107.pdf
        //
        float fCalibrationFactor;

        uint16_t i16Object;

        uint16_t i16Ambient;

        TMP006() :
            ui8Addr(TMP006_I2C_ADDRESS), fCalibrationFactor(6.40e-14), i16Object(0), i16Ambient(0)
        {
        }
    };

    TMP006 parameters;

  public:
    void update(TMP006Representation& theTMP006Representation);

  private:
    uint16_t readRegister(uint8_t cmd);
};

#endif /* TMP006MODULE_H_ */
