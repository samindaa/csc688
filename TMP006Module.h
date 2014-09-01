/*
 * TMP006Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef TMP006MODULE_H_
#define TMP006MODULE_H_

#include "Template.h"
#include "TMP006Representation.h"
#include "hw_tmp006.h"

//*****************************************************************************
//
// Define TMP006 I2C Address.
//
//*****************************************************************************
#define TMP006_I2C_ADDRESS      0x41

MODULE(TMP006Module)
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

        //
        // The data buffer used for sending/receiving data to/from the TMP006.
        //
        uint8_t pui8Data[4];

        TMP006() :
            ui8Addr(TMP006_I2C_ADDRESS), fCalibrationFactor(6.40e-14)
        {
        }
    };

    TMP006 parameters;

  public:
    void init();
    void update(TMP006Representation& theTMP006Representation);

  private:
    int16_t readRegister(const uint8_t& cmd);
    void writeRegister(const uint8_t& ui8Count);
};

#endif /* TMP006MODULE_H_ */
