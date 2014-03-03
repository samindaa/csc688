/*
 * ISL29023Module.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#include "ISL29023Module.h"

#if defined(ENERGIA)
#include "Wire.h"
#endif

//*****************************************************************************
//
// Constants to hold the floating point version of the thresholds for each
// range setting. Numbers represent an 81% and 19 % threshold levels. This
// creates a +/- 1% hysteresis band between range adjustments.
//
//*****************************************************************************
const float g_fThresholdHigh[4] =
{ 810.0f, 3240.0f, 12960.0f, 64000.0f };
const float g_fThresholdLow[4] =
{ 0.0f, 760.0f, 3040.0f, 12160.0f };

//*****************************************************************************
//
// Range setting to floating point range value lookup table
//
//*****************************************************************************
const float g_fRangeLookup[4] =
{ 1000.0, 4000.0, 16000.0, 64000.0 };

//*****************************************************************************
//
// Resolution setting to floating point resolution value lookup table
//
//*****************************************************************************
const float g_fResolutionLookup[4] =
{ 65536.0, 4096.0, 256.0, 16.0 };

//*****************************************************************************
//
// Beta value lookup based on datasheet typical values for DATA_IR1, DATA_IR2,
// DATA_IR3, DATA_IR4.  These should be reasonable for 16 bit conversions.
// However, Beta changes with resolution and background IR conditions.
//
//*****************************************************************************
const float g_fBetaLookup[4] =
{ 95.238, 23.810, 5.952, 1.486 };

void ISL29023Module::update(ISL29023Representation& theISL29023Representation)
{
#if defined(ENERGIA)
  //
  // Get the raw light data from the instance structure
  //
  I2CMRead();
  uint16_t ui16Light = (parameters.pui8Data[1] << 8) | parameters.pui8Data[0];

  //
  // Get the floating point values for range and resolution from the lookup.
  //
  float fRange = g_fRangeLookup[parameters.ui8Range];
  float fResolution = g_fResolutionLookup[parameters.ui8Resolution];

  //
  // Calculate light reading in lux.
  //
  theISL29023Representation.fAmbient = ((float) ui16Light) * (fRange / fResolution);

  //
  // Check if the intensity of light has crossed a threshold. If so
  // then adjust range of sensor readings to track intensity.
  //
  ISL29023AppAdjustRange(theISL29023Representation);

  //
  // Print the temperature as integer and fraction parts.
  //
  //Serial.print("Visible Lux: ");
  //Serial.println(theISL29023Representation.fAmbient, 3);

#endif
}

void ISL29023Module::init()
{
#if defined(ENERGIA)
  parameters.ui8Range = ISL29023_CMD_II_RANGE_1K >> ISL29023_CMD_II_RANGE_S;
  parameters.ui8Resolution = (ISL29023_CMD_II_ADC_RES_16 >> ISL29023_CMD_II_ADC_RES_S);

  //
  // Put the device into power down mode.
  //
  parameters.pui8Data[0] = ISL29023_O_CMD_I;
  parameters.pui8Data[1] = ISL29023_CMD_I_OP_MODE_POWER_DOWN;
  Wire.beginTransmission(parameters.ui8Addr);
  for (int i = 0; i < 2; i++)
    Wire.write(parameters.pui8Data[i]);
  Wire.endTransmission();

  //
  // Configure the ISL29023 to measure ambient light continuously. Set a 8
  // sample persistence before the INT pin is asserted. Clears the INT flag.
  // Persistence setting of 8 is sufficient to ignore camera flashes.
  //
  ISL29023ReadModifyWrite(ISL29023_O_CMD_I,
      ~(ISL29023_CMD_I_OP_MODE_M | ISL29023_CMD_I_INT_PERSIST_M | ISL29023_CMD_I_INT_FLAG_M),
      (ISL29023_CMD_I_OP_MODE_ALS_CONT | ISL29023_CMD_I_INT_PERSIST_8));
  //
  // Configure the upper threshold to 80% of maximum value
  //
  parameters.pui8Data[0] = 0xCC;
  parameters.pui8Data[1] = 0xCC;
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(ISL29023_O_INT_HT_LSB);
  for (int i = 0; i < 2; i++)
    Wire.write(parameters.pui8Data[i]);
  Wire.endTransmission();

  //
  // Configure the lower threshold to 20% of maximum value
  //
  parameters.pui8Data[0] = 0x33;
  parameters.pui8Data[1] = 0x33;
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(ISL29023_O_INT_LT_LSB);
  for (int i = 0; i < 2; i++)
    Wire.write(parameters.pui8Data[i]);
  Wire.endTransmission();

#endif

}

void ISL29023Module::I2CMRead()
{
#if defined(ENERGIA)
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(ISL29023_O_DATA_OUT_LSB);
  Wire.endTransmission(false);
  Wire.requestFrom(parameters.ui8Addr, (uint8_t) 2);
  while (Wire.available() < 1)
    ;
  parameters.pui8Data[0] = Wire.read();
  parameters.pui8Data[1] = Wire.read();
#endif
}

void ISL29023Module::ISL29023ReadModifyWrite(const uint_fast8_t& ui8Reg, const uint8_t& ui8Mask,
    const uint8_t& ui8Value)
{
#if defined(ENERGIA)
  //
  // Configure the ISL29023 to measure ambient light continuously. Set a 8
  // sample persistence before the INT pin is asserted. Clears the INT flag.
  // Persistence setting of 8 is sufficient to ignore camera flashes.
  //
  parameters.ui8Reg = ui8Reg;
  parameters.ui8Mask = ui8Mask;
  parameters.ui8Value = ui8Value;

  //
  // Construct the I2C command to access the requested register.
  //
  parameters.pui8Data[0] = parameters.ui8Reg;
  if (parameters.ui8Mask == 0)
  {
    //
    // Set the new register value in the command buffer.
    //
    parameters.pui8Data[1] = parameters.ui8Value;
    Wire.beginTransmission(parameters.ui8Addr);
    for (int i = 0; i < 2; i++)
      Wire.write(parameters.pui8Data[i]);
    Wire.endTransmission();

  }
  else
  {
    Wire.beginTransmission(parameters.ui8Addr);
    Wire.write(parameters.pui8Data[0]);
    Wire.endTransmission(false);
    Wire.requestFrom(parameters.ui8Addr, (uint8_t) 1);
    while (Wire.available() == 0)
      ;
    parameters.pui8Data[1] = Wire.read();

    //
    // The read portion of the read-modify-write has completed.
    //

    //
    // Modify the register data that was just read.
    //
    parameters.pui8Data[1] = ((parameters.pui8Data[1] & parameters.ui8Mask) | parameters.ui8Value);

    //
    // Write the data back to the device.
    //
    Wire.beginTransmission(parameters.ui8Addr);
    for (int i = 0; i < 2; i++)
      Wire.write(parameters.pui8Data[i]);
    Wire.endTransmission();

  }
#endif
}

void ISL29023Module::ISL29023AppAdjustRange(ISL29023Representation& theISL29023Representation)
{
#if defined(ENERGIA)
  bool isRangeChanged = false;
  //
  // Check if we crossed the upper threshold.
  //
  if (theISL29023Representation.fAmbient > g_fThresholdHigh[parameters.ui8Range])
  {
    //
    // The current intensity is over our threshold so adjsut the range
    // accordingly
    //
    if (parameters.ui8Range < ISL29023_CMD_II_RANGE_64K)
    {
      parameters.ui8Range = parameters.ui8Range + 1;
      isRangeChanged = true;
    }
  }

  //
  // Check if we crossed the lower threshold
  //
  if (theISL29023Representation.fAmbient < g_fThresholdLow[parameters.ui8Range])
  {
    //
    // If possible go to the next lower range setting and reconfig the
    // thresholds.
    //
    if (parameters.ui8Range > ISL29023_CMD_II_RANGE_1K)
    {
      parameters.ui8Range = parameters.ui8Range - 1;
      isRangeChanged = true;
    }
  }

  //
  // If the desired range value changed then send the new range to the sensor
  //
  if (isRangeChanged)
  {
    ISL29023ReadModifyWrite(ISL29023_O_CMD_II, ~ISL29023_CMD_II_RANGE_M, parameters.ui8Range);

    if (parameters.pui8Data[0] == ISL29023_O_CMD_II)
    {
      //
      // Store the latest range and resolution settings
      //
      parameters.ui8Range = ((parameters.pui8Data[1] & ISL29023_CMD_II_RANGE_M) >>
      ISL29023_CMD_II_RANGE_S);
      parameters.ui8Resolution = ((parameters.pui8Data[1] &
      ISL29023_CMD_II_ADC_RES_M) >>
      ISL29023_CMD_II_ADC_RES_S);
    }

    //
    // Now we must manually clear the flag in the ISL29023
    // register.
    //
    Wire.beginTransmission(parameters.ui8Addr);
    Wire.write(ISL29023_O_CMD_I);
    Wire.endTransmission(false);
    Wire.requestFrom(parameters.ui8Addr, (uint8_t) 1);
    while (Wire.available() == 0)
      ;
    parameters.pui8Data[0] = Wire.read();

  }
#endif
}

MAKE_MODULE(ISL29023Module)

