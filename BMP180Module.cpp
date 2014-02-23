/*
 * BMP180Module.cpp
 *
 *  Created on: Feb 20, 2014
 *      Author: Saminda
 */

#include "BMP180Module.h"

#if defined(ENERGIA)
#include "Wire.h"
#endif

BMP180Module::BMP180Module() :
    calcB4(0), calcB7(0)
{

}

BMP180Module::~BMP180Module()
{
}

void BMP180Module::init()
{
  parameters.ui8Addr = BMP180_I2C_ADDRESS;
  parameters.tunit = BMP180::BMP180_C;
  parameters.oversampling = 0;
  calibration();
}

void BMP180Module::update(BMP180Representation& theBMP180Representation)
{
#if defined(ENERGIA)
  // Temperature
  theBMP180Representation.rawTemperature = cmdI2CMRead(
      (BMP180_CTRL_MEAS_SCO | BMP180_CTRL_MEAS_TEMPERATURE), BMP180_O_OUT_MSB);
#ifdef DEBUG_BMP180
  Serial.print("UT=");
  Serial.println(theBMP180Representation.rawTemperature);
#endif
  // Pressure
  // This is with no sampling
  theBMP180Representation.rawPressure = cmdI2CMRead(
      (BMP180_CTRL_MEAS_SCO | BMP180_CTRL_MEAS_PRESSURE | parameters.oversampling << 6),
      BMP180_O_OUT_MSB);
#ifdef DEBUG_BMP180
  Serial.print("UP=");
  Serial.println(theBMP180Representation.rawPressure);
#endif
  calculation(theBMP180Representation);             // run calculations for temperature and pressure

#ifdef DEBUG_BMP180
  Serial.print("Temperature: ");
  Serial.print(theBMP180Representation.temperature / 10);          // display temperature in Celsius
  Serial.print(".");
  Serial.print(theBMP180Representation.temperature % 10);// display temperature in Celsius
  Serial.println("C");
  Serial.print("Pressure:    ");
  Serial.print((theBMP180Representation.pressure + 50) / 100);// display pressure in hPa
  Serial.println("hPa");
#endif

#endif

}

void BMP180Module::calibration()
{
#if defined(ENERGIA)
  parameters.i16AC1 = I2CMRead(BMP180_O_AC1_MSB);
  parameters.i16AC2 = I2CMRead(BMP180_O_AC2_MSB);
  parameters.i16AC3 = I2CMRead(BMP180_O_AC3_MSB);

  parameters.ui16AC4 = I2CMRead(BMP180_O_AC4_MSB);
  parameters.ui16AC5 = I2CMRead(BMP180_O_AC5_MSB);
  parameters.ui16AC6 = I2CMRead(BMP180_O_AC6_MSB);

  parameters.i16B1 = I2CMRead(BMP180_O_B1_MSB);
  parameters.i16B2 = I2CMRead(BMP180_O_B2_MSB);

  parameters.i16MC = I2CMRead(BMP180_O_MC_MSB);
  parameters.i16MD = I2CMRead(BMP180_O_MD_MSB);

#if defined(DEBUG_BMP180)
  Serial.print("i16AC1=");
  Serial.println(parameters.i16AC1);
  Serial.print("i16AC2=");
  Serial.println(parameters.i16AC2);
  Serial.print("i16AC3=");
  Serial.println(parameters.i16AC3);
  Serial.print("ui16AC4=");
  Serial.println(parameters.ui16AC4);
  Serial.print("ui16AC5=");
  Serial.println(parameters.ui16AC5);
  Serial.print("ui16AC6=");
  Serial.println(parameters.ui16AC6);
  Serial.print("i16B1=");
  Serial.println(parameters.i16B1);
  Serial.print("i16B2=");
  Serial.println(parameters.i16B2);
  Serial.print("i16MC=");
  Serial.println(parameters.i16MC);
  Serial.print("i16MD=");
  Serial.println(parameters.i16MD);
#endif

#endif
}

// read 16-bits from I2C
uint16_t BMP180Module::I2CMRead(const uint8_t& addr)
{
#if defined(ENERGIA)
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(addr);
  Wire.endTransmission(false);
  Wire.requestFrom(parameters.ui8Addr, (uint8_t) 2);
  while (Wire.available() == 0)
  ;
  return (Wire.read() << 8 | Wire.read());
#else
  return 0;
#endif
}

uint16_t BMP180Module::cmdI2CMRead(const uint8_t& cmd, const uint8_t& addr)
{
#if defined(ENERGIA)
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(BMP180_O_CTRL_MEAS);
  Wire.write(cmd);
  Wire.endTransmission();
  return I2CMRead(addr);
#else
  return 0;
#endif
}

void BMP180Module::calculation(BMP180Representation& theBMP180Representation)
{
#if defined(ENERGIA)
  // calculating temperature
  int32_t calcX1 =
  (((int32_t) theBMP180Representation.rawTemperature - (int32_t) parameters.ui16AC6)
      * (int32_t) parameters.ui16AC5) >> 15;
  int32_t calcX2 = ((int32_t) parameters.i16MC << 11) / (calcX1 + parameters.i16MD);
  int32_t calcB5 = calcX1 + calcX2;

  if (parameters.tunit == BMP180::BMP180_F)
  theBMP180Representation.temperature = (calcB5 * 9 / 5 + 5128) >> 4;// calculate temperature in 0.1 F
  else
  theBMP180Representation.temperature = (calcB5 + 8) >> 4;// calculate temperature in 0.1 C

#ifdef DEBUG_BMP180
  Serial.print("X1=");
  Serial.println(calcX1);
  Serial.print("X2=");
  Serial.println(calcX2);
  Serial.print("B5=");
  Serial.println(calcB5);
  Serial.print("T=");
  Serial.println(theBMP180Representation.temperature);
#endif

  if (parameters.oversampling >= 4)
  return;    // done if in temperature only mode

  // calculating pressure
  int32_t calcB6 = calcB5 - 4000;
  calcX1 = ((int32_t) parameters.i16B2 * (calcB6 * calcB6 >> 12)) >> 11;
  calcX2 = (int32_t) parameters.i16AC2 * calcB6 >> 11;
  int32_t calcX3 = calcX1 + calcX2;
  int32_t calcB3 = ((((int32_t) parameters.i16AC1 * 4 + calcX3) << parameters.oversampling) + 2)
  >> 2;
#ifdef DEBUG_BMP180
  Serial.print("B6=");
  Serial.println(calcB6);
  Serial.print("X1=");
  Serial.println(calcX1);
  Serial.print("X2=");
  Serial.println(calcX2);
  Serial.print("X3=");
  Serial.println(calcX3);
  Serial.print("B3=");
  Serial.println(calcB3);
#endif
  calcX1 = (int32_t) parameters.i16AC3 * calcB6 >> 13;
  calcX2 = ((int32_t) parameters.i16B1 * (calcB6 * calcB6 >> 12)) >> 16;
  calcX3 = ((calcX1 + calcX2) + 2) >> 2;
  calcB4 = (int32_t) parameters.ui16AC4 * (uint32_t) (calcX3 + 32768) >> 15;
  calcB7 = ((uint32_t) theBMP180Representation.rawPressure - calcB3)
  * (50000 >> parameters.oversampling);
  if (calcB7 < 0x80000000)
  theBMP180Representation.pressure = ((calcB7 * 2) / calcB4);
  else
  theBMP180Representation.pressure = (calcB7 / calcB4) * 2;
#ifdef DEBUG_BMP180
  Serial.print("X1=");
  Serial.println(calcX1);
  Serial.print("X2=");
  Serial.println(calcX2);
  Serial.print("X3=");
  Serial.println(calcX3);
  Serial.print("B4=");
  Serial.println(calcB4);
  Serial.print("B7=");
  Serial.println(calcB7);
  Serial.print("p=");
  Serial.println(theBMP180Representation.pressure);
#endif
  calcX1 = (theBMP180Representation.pressure >> 8) * (theBMP180Representation.pressure >> 8);
#ifdef DEBUG_BMP180
  Serial.print("X1=");
  Serial.println(calcX1);
#endif
  calcX1 = (calcX1 * 3038) >> 16;
  calcX2 = (-7357 * theBMP180Representation.pressure) >> 16;
  theBMP180Representation.pressure = theBMP180Representation.pressure
  + ((calcX1 + calcX2 + 3791) >> 4);
#ifdef DEBUG_BMP180
  Serial.print("X1=");
  Serial.println(calcX1);
  Serial.print("X2=");
  Serial.println(calcX2);
  Serial.print("p=");
  Serial.println(theBMP180Representation.pressure);
#endif
  theBMP180Representation.fAltitude = 44330.0f * (1.0f - powf(theBMP180Representation.pressure / 101325.0f,
          1.0f / 5.255f)) / 100.0f;

#ifdef DEBUG_BMP180
  //
  // Convert the floats to an integer part and fraction part for easy
  // print.
  //
  int32_t i32IntegerPart = (int32_t) theBMP180Representation.fAltitude;
  int32_t i32FractionPart = (int32_t) (theBMP180Representation.fAltitude * 1000.0f);
  i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
  if(i32FractionPart < 0)
  {
    i32FractionPart *= -1;
  }

  Serial.print("Altitude: ");
  Serial.print(i32IntegerPart);
  Serial.print(".");
  Serial.println(i32FractionPart);

#endif

#endif
}

MAKE_MODULE(BMP180Module)
