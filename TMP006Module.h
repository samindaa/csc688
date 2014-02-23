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
REQUIRES(LaunchPadRepresentation)
PROVIDES(TMP006Representation)
END_MODULE
class TMP006Module: public TMP006ModuleBase
{
  public:
    void update(TMP006Representation& theTMP006Representation);

  private:
    uint16_t read16(uint8_t cmd);
};

#endif /* TMP006MODULE_H_ */
