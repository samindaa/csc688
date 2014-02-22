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

//*****************************************************************************
//
// Define ISL29023 I2C Address.
//
//*****************************************************************************
#define ISL29023_I2C_ADDRESS    0x44

#define ISL29023_CMD_II_ADC_RES_16                                            \
                                0x00        // 16 bit resolution
#define ISL29023_CMD_II_ADC_RES_12                                            \
                                0x04        // 12 bit resolution
#define ISL29023_CMD_II_ADC_RES_8                                             \
                                0x08        // 8 bit resolution
#define ISL29023_CMD_II_ADC_RES_4                                             \
                                0x0C        // 4 bit resolution

#define ISL29023_CMD_II_RANGE_1K                                              \
                                0x00        // 1000 lux range
#define ISL29023_CMD_II_RANGE_4K                                              \
                                0x01        // 4000 lux range
#define ISL29023_CMD_II_RANGE_16K                                             \
                                0x02        // 16000 lux range
#define ISL29023_CMD_II_RANGE_64K                                             \
                                0x03        // 64000 lux range

#define ISL29023_CMD_II_RANGE_S 0

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
