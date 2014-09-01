/*
 * ISL29023Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef ISL29023MODULE_H_
#define ISL29023MODULE_H_

#include "Template.h"
#include "ISL29023Representation.h"
#include "hw_isl29023.h"

//*****************************************************************************
//
// Define ISL29023 I2C Address.
//
//*****************************************************************************
#define ISL29023_I2C_ADDRESS    0x44

MODULE(ISL29023Module)
  PROVIDES(ISL29023Representation)
END_MODULE

class ISL29023Module: public ISL29023ModuleBase
{
  private:
    class ISL29023
    {
      public:
        //
        // The I2C address of the ISL29023.
        //
        uint8_t ui8Addr;

        //
        // Instance copy of the range setting.  Used in GetFloat functions
        //
        uint8_t ui8Range;

        //
        // Instance copy of the resolution setting.  Used in GetFloat function.
        //
        uint8_t ui8Resolution;

        uint_fast8_t ui8Reg;

        uint8_t ui8Mask;

        uint8_t ui8Value;

        //
        // The data buffer used for sending/receiving data to/from the ISL29023.
        //
        uint8_t pui8Data[4];

        ISL29023() :
            ui8Addr(ISL29023_I2C_ADDRESS), ui8Range(0), ui8Resolution(0), ui8Reg(0), ui8Mask(0), ui8Value(
                0)
        {
        }
    };

    ISL29023 parameters;

  public:
    void init();
    void update(ISL29023Representation& theISL29023Representation);

  private:
    void I2CMRead();
    void ISL29023ReadModifyWrite(const uint_fast8_t& ui8Reg, const uint8_t& ui8Mask,
        const uint8_t& ui8Value);
    void ISL29023AppAdjustRange(ISL29023Representation& theISL29023Representation);
};

#endif /* ISL29023MODULE_H_ */
