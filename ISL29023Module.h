/*
 * ISL29023Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef ISL29023MODULE_H_
#define ISL29023MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "ISL29023Representation.h"
#include "hw_isl29023.h"

//*****************************************************************************
//
// Define ISL29023 I2C Address.
//
//*****************************************************************************
#define ISL29023_I2C_ADDRESS    0x44


MODULE(ISL29023Module)
  REQUIRES(LaunchPadRepresentation)
  PROVIDES(ISL29023Representation)
END_MODULE

class ISL29023Module: public ISL29023ModuleBase
{
  private:
    //
    // Instance copy of the range setting.  Used in GetFloat functions
    //
    uint8_t ui8Range;

    //
    // Instance copy of the resolution setting.  Used in GetFloat function.
    //
    uint8_t ui8Resolution;

  public:
    ISL29023Module();
    void update(ISL29023Representation& theISL29023Representation);
};

#endif /* ISL29023MODULE_H_ */
