/*
 * TMP006Module.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#include "TMP006Module.h"
#if defined(ENERGIA)
#include "Wire.h"
#endif

MAKE_MODULE(TMP006Module)

//*****************************************************************************
//
// The constants used to calculate object temperature.
//
//*****************************************************************************
#define T_REF                   298.15
#define A1                      1.75e-03
#define A2                      -1.678e-05
#define B0                      -2.94e-05
#define B1                      -5.70e-07
#define B2                      4.63e-09
#define C2                      13.4

void TMP006Module::update(TMP006Representation& theTMP006Representation)
{
#if defined(ENERGIA)
  parameters.i16Object = read16(TMP006_O_VOBJECT);
  parameters.i16Ambient = read16(TMP006_O_TAMBIENT);

  float fTdie2, fS, fVo, fVx, fObj;

  //
  // The bottom two bits are not temperature data, so discard them but keep
  // the sign information.
  //
  theTMP006Representation.fAmbient = (float) (parameters.i16Ambient / 4);

  //
  // Divide by 32 to get unit scaling correct.
  //
  theTMP006Representation.fAmbient = theTMP006Representation.fAmbient / 32.0;

  //
  // fTdie2 is measured ambient temperature in degrees Kelvin.
  //
  fTdie2 = theTMP006Representation.fAmbient + T_REF;

  //
  // S is the sensitivity.
  //
  //fS = psInst->fCalibrationFactor * (1.0f + (A1 * (*pfAmbient)) +
  //    (A2 * ((*pfAmbient) * (*pfAmbient))));
  //
  // Set the calibration factor to a reasonable estimate, applications
  // should perform a calibration in their environment and directly overwrite
  // this value after calling TMP006Init with the system specific value.
  //
  fS = parameters.fCalibrationFactor
      * (1.0f + (A1 * (theTMP006Representation.fAmbient))
          + (A2 * ((theTMP006Representation.fAmbient) * (theTMP006Representation.fAmbient))));

  //
  // Vos is the offset voltage.
  //
  fVo = B0 + (B1 * (theTMP006Representation.fAmbient))
      + (B2 * ((theTMP006Representation.fAmbient) * (theTMP006Representation.fAmbient)));

  //
  // Vx is the difference between raw object voltage and Vos
  // 156.25e-9 is nanovolts per least significant bit from the voltage
  // register.
  //
  fVx = (((float) parameters.i16Object) * 156.25e-9) - fVo;

  //
  // fObj is the feedback coefficient.
  //
  fObj = fVx + C2 * (fVx * fVx);

  //
  // Finally calculate the object temperature.
  //
  parameters.fObject = (sqrtf(sqrtf((fTdie2 * fTdie2 * fTdie2 * fTdie2) + (fObj / fS))) - T_REF);

  //
  // Convert the floating point ambient temperature  to an integer part
  // and fraction part for easy printing.
  //
  //Serial.print("Ambient=");
  //Serial.println(theTMP006Representation.fAmbient, 3);

  //
  // Convert the floating point ambient temperature  to an integer part
  // and fraction part for easy printing.
  //
  //Serial.print("Object=");
  //Serial.println(parameters.fObject, 3);

#endif
}

uint16_t TMP006Module::read16(uint8_t cmd)
{
#if defined(ENERGIA)
  // Send request
  Wire.beginTransmission(parameters.ui8Addr);
  // Start talking
  // Ask for register
  Wire.write(cmd);
  // Complete transmission
  Wire.endTransmission(false);
  // Request bytes
  // Wait for response
  //
  Wire.requestFrom(parameters.ui8Addr, (uint8_t) 2);
  while (Wire.available() < 1)
    ;
  return (Wire.read() << 8 | Wire.read());
#else
  return 0;
#endif
}
