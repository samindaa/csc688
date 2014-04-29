/*
 * SHT21Module.cpp
 *
 *  Created on: Feb 23, 2014
 *      Author: Saminda
 */

#include "SHT21Module.h"

#if defined(ENERGIA)
#include "Wire.h"
#endif

// This is a very slow component.
// Use it only if necessary.
//MAKE_MODULE(SHT21Module)

void SHT21Module::init()
{
#if defined(ENERGIA)
  //
  // Perform a soft reset of the SHT21.
  //
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(SHT21_CMD_SOFT_RESET);
  Wire.endTransmission();

#endif
}

void SHT21Module::update(SHT21Representation& theSHT21Representation)
{
#if defined(ENERGIA)
  //
  // Write the command to start a humidity measurement
  //
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(SHT21_CMD_MEAS_RH);
  Wire.endTransmission(false);

  //
  // Wait 33 milliseconds before attempting to get the result. Datasheet
  // claims this can take as long as 29 milliseconds
  //
  delay(33);
  Wire.requestFrom(parameters.ui8Addr, (uint8_t) 2);
  while (Wire.available() < 1)
    ;
  //
  // Return the raw humidity value.
  //
  parameters.ui16Humidity = ((((uint16_t) Wire.read()) << 8) | (uint16_t) Wire.read());

  //
  // Convert the raw measure to float for later math.
  //
  theSHT21Representation.fHumidity = (float) (parameters.ui16Humidity & 0xFFFC);

  //
  // Convert from raw measurement to percent relative humidity over water
  // per the datasheet formula.
  //
  theSHT21Representation.fHumidity = -6.0 + 125.0 * (theSHT21Representation.fHumidity / 65536.0);

  //
  // Convert to a number from 0 to 1.0 instead of 0 to 100%.
  //
  theSHT21Representation.fHumidity /= 100.0;

  //
  // Write the command to start a temperature measurement
  //
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(SHT21_CMD_MEAS_T);
  Wire.endTransmission(false);

  //
  // Wait 100 milliseconds before attempting to get the result. Datasheet
  // claims this can take as long as 85 milliseconds
  //
  delay(100);
  Wire.requestFrom(parameters.ui8Addr, (uint8_t) 2);
  while (Wire.available() < 1)
    ;

  //
  // Return the raw temperature value.
  //
  parameters.ui16Temperature = ((((uint16_t) Wire.read()) << 8) | (uint16_t) Wire.read());

  //
  // Get the raw temperature into a floating point variable
  //
  theSHT21Representation.fTemperature = (float) (parameters.ui16Temperature & 0xFFFC);

  //
  // Equation from SHT21 datasheet for raw to Celsius conversion.
  //
  theSHT21Representation.fTemperature = -46.85
      + 175.72 * (theSHT21Representation.fTemperature / 65536.0);

  //
  // Convert the floats to an integer part and fraction part for easy
  // print. Humidity is returned as 0.0 to 1.0 so multiply by 100 to get
  // percent humidity.
  //
  theSHT21Representation.fHumidity *= 100.0f;
  //Serial.print("Humidity: ");
  //Serial.println(theSHT21Representation.fHumidity, 3);
  //
  // Perform the conversion from float to a printable set of integers
  //
  //Serial.print("\t Temperature: ");
  //Serial.println(theSHT21Representation.fTemperature, 3);

#endif
}
