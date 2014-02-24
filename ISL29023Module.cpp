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

ISL29023Module::ISL29023Module() :
    i32IntegerPart(0), i32FractionPart(0)
{
}

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
  // Perform the conversion from float to a printable set of integers
  //
  i32IntegerPart = (int32_t) theISL29023Representation.fAmbient;
  i32FractionPart = (int32_t) (theISL29023Representation.fAmbient * 1000.0f);
  i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
  if (i32FractionPart < 0)
  {
    i32FractionPart *= -1;
  }

  //
  // Print the temperature as integer and fraction parts.
  //
  Serial.print("Visible Lux:");
  Serial.print(i32IntegerPart);
  Serial.print(".");
  Serial.println(i32FractionPart);

  // TODO range setting

#endif
}

void ISL29023Module::init()
{
#if defined(ENERGIA)
  parameters.ui8Range = ISL29023_CMD_II_RANGE_1K >> ISL29023_CMD_II_RANGE_S;
  parameters.ui8Resolution = (ISL29023_CMD_II_ADC_RES_16 >>
  ISL29023_CMD_II_ADC_RES_S);

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
  /*uint8_t ui8Mask = (ISL29023_CMD_I_OP_MODE_M | ISL29023_CMD_I_INT_PERSIST_M |
  ISL29023_CMD_I_INT_FLAG_M);
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(~ui8Mask);
  Wire.write((ISL29023_CMD_I_OP_MODE_ALS_CONT | ISL29023_CMD_I_INT_PERSIST_8));
  Wire.endTransmission();*/

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

void ISL29023Module::ISL29023AppAdjustRange(ISL29023Representation& theISL29023Representation)
{
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
      parameters.ui8Range = parameters.ui8Range + 1;
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
    }
  }

  //
  // If the desired range value changed then send the new range to the sensor
  // TODO: is this necessary?
  //

}

MAKE_MODULE(ISL29023Module)

