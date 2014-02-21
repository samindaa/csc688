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
    oversampling(0), eocpin(0), tempunit(BMP180_C), i2caddress(BMP180_I2C_ADDR), calcB4(0), calcB7(
        0)
{
}

BMP180Module::~BMP180Module()
{
}

void BMP180Module::init()
{
  oversampling = 0;
  eocpin = 0;
  tempunit = BMP180_C;
  begin();
}

void BMP180Module::update(BMP180Representation& theBMP180Representation)
{
#if defined(ENERGIA)
  refresh(theBMP180Representation);                    // read current sensor data
  calculate(theBMP180Representation);               // run calculations for temperature and pressure
  Serial.print("Temperature: ");
  Serial.print(theBMP180Representation.temperature / 10);  // display temperature in Celsius
  Serial.print(".");
  Serial.print(theBMP180Representation.temperature % 10);  // display temperature in Celsius
  Serial.println("C");
  Serial.print("Pressure:    ");
  Serial.print((theBMP180Representation.pressure + 50) / 100);   // display pressure in hPa
  Serial.println("hPa");
#endif
}

// call once to initialize, reads sensor calibration data
void BMP180Module::begin()
{
#if defined(ENERGIA)
#ifdef DEBUG_BMP180
  // verify that we're actually talking to a BMP180 sensor
  if(read8(BMP180_REG_ID) != BMP180_CHIP_ID)
  {
    Serial.println("BMP180 device not found");
    return;
  }
#endif

  // read calibration data from sensor EEPROM
  int16_t *calarray = (int16_t*) &cal;
  uint8_t addr = 0;
  while (addr < 11)
  {
    calarray[addr] = read16(BMP180_REG_CAL + (addr * 2));
    addr++;
  }

#ifdef DEBUG_BMP180
  Serial.print("AC1=");
  Serial.println(cal.ac1);
  Serial.print("AC2=");
  Serial.println(cal.ac2);
  Serial.print("AC3=");
  Serial.println(cal.ac3);
  Serial.print("AC4=");
  Serial.println(cal.ac4);
  Serial.print("AC5=");
  Serial.println(cal.ac5);
  Serial.print("AC6=");
  Serial.println(cal.ac6);
  Serial.print("B1=");
  Serial.println(cal.b1);
  Serial.print("B2=");
  Serial.println(cal.b2);
  Serial.print("MB=");
  Serial.println(cal.mb);
  Serial.print("MC=");
  Serial.println(cal.mc);
  Serial.print("MD=");
  Serial.println(cal.md);
#endif

  if (eocpin)
  {                         // if EOC pin is configured to detect end of conversion..
    pinMode(eocpin, INPUT);            // setup EOC pin as digital input
  }

  // done with initialization
}

// reading fresh raw data from sensor
void BMP180Module::refresh(BMP180Representation& theBMP180Representation)
{
#ifdef DEBUG_BMP180
  Serial.println("Reading temperature..");
#endif

  sendCmd(BMP180_CMD_T);                // send command to read temperature

  if (eocpin)
  {                            // if EOC pin is configured..
    while (digitalRead(eocpin) == LOW)
      ;      // wait for end of conversion
  }
  else
  {                                // if not..
    delay(5);                             // wait 5ms for data to be ready
  }

  theBMP180Representation.rawTemperature = read16(BMP180_REG_ADC);    // read raw temperature data

#ifdef DEBUG_BMP180
  Serial.print("UT=");
  Serial.println(theBMP180Representation.rawTemperature);
#endif

  if (oversampling >= 4)
    return;            // done if in temperature only mode

#ifdef DEBUG_BMP180
  Serial.println("Reading pressure..");
#endif

  sendCmd(BMP180_CMD_P | (oversampling << 6)); // send command to read pressure, incl. oversampling mode

  if (eocpin)
  {                            // if EOC pin is configured..
    while (digitalRead(eocpin) == LOW)
      ;      // wait for end of conversion
  }
  else
  {                                // if not..
    if (oversampling == 0)
      delay(5);       // delay for conversion to complete
    else if (oversampling == 1)
      delay(8);  //   using maximum conversion times
    else if (oversampling == 2)
      delay(14); //   depending on level of oversampling
    else if (oversampling == 3)
      delay(26);
  }

  if (oversampling == 0)
  {                    // if no oversampling..
    theBMP180Representation.rawPressure = read16(BMP180_REG_ADC);  // read 16 bit raw pressure
  }
  else
  {                                   // if there is oversampling read more bits
    theBMP180Representation.rawPressure = (((uint32_t) read16(BMP180_REG_ADC) << 8)
        | read8(BMP180_REG_ADC + 2)) >> (8 - oversampling);
  }

#ifdef DEBUG_BMP180
  Serial.print("UP=");
  Serial.println(theBMP180Representation.rawPressure);
#endif
#endif
  // done refreshing raw data
}
// calculating temperature and pressure from raw data
void BMP180Module::calculate(BMP180Representation& theBMP180Representation)
{
#if defined(ENERGIA)
#ifdef DEBUG_BMP180
  Serial.println("Calculating temperature in 0.1 Celcius..");
#endif

  // calculating temperature
  int32_t calcX1 = (((int32_t) theBMP180Representation.rawTemperature - (int32_t) cal.ac6)
      * (int32_t) cal.ac5) >> 15;
  int32_t calcX2 = ((int32_t) cal.mc << 11) / (calcX1 + cal.md);
  int32_t calcB5 = calcX1 + calcX2;

  if (tempunit == BMP180_F)
  {
    theBMP180Representation.temperature = (calcB5 * 9 / 5 + 5128) >> 4; // calculate temperature in 0.1 F
  }
  else
  {
    theBMP180Representation.temperature = (calcB5 + 8) >> 4;      // calculate temperature in 0.1 C
  }

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

  if (oversampling >= 4)
    return;    // done if in temperature only mode

#ifdef DEBUG_BMP180
  Serial.println("Calculating pressure in Pascal..");
#endif

  // calculating pressure
  int32_t calcB6 = calcB5 - 4000;
  calcX1 = ((int32_t) cal.b2 * (calcB6 * calcB6 >> 12)) >> 11;
  calcX2 = (int32_t) cal.ac2 * calcB6 >> 11;
  int32_t calcX3 = calcX1 + calcX2;
  int32_t calcB3 = ((((int32_t) cal.ac1 * 4 + calcX3) << oversampling) + 2) >> 2;
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
  calcX1 = (int32_t) cal.ac3 * calcB6 >> 13;
  calcX2 = ((int32_t) cal.b1 * (calcB6 * calcB6 >> 12)) >> 16;
  calcX3 = ((calcX1 + calcX2) + 2) >> 2;
  calcB4 = (int32_t) cal.ac4 * (uint32_t) (calcX3 + 32768) >> 15;
  calcB7 = ((uint32_t) theBMP180Representation.rawPressure - calcB3) * (50000 >> oversampling);
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
#endif
  // done with calculations
}

// send command to BMP180
void BMP180Module::sendCmd(uint8_t cmd)
{
#if defined(ENERGIA)
  Wire.beginTransmission(i2caddress);
  Wire.write(byte(BMP180_REG_CTRL));
  Wire.write(byte(cmd));
  Wire.endTransmission();
#endif
}
// read 8 bits from I2C
uint8_t BMP180Module::read8(uint8_t addr)
{
#if defined(ENERGIA)
  Wire.beginTransmission(i2caddress);
  Wire.write(addr);
  Wire.endTransmission(false);

  Wire.requestFrom(i2caddress, (uint8_t) 1);    // need to cast int to avoid compiler warnings
  return Wire.read();
#else
  return 0;
#endif
}
// read 16 bits from I2C
uint16_t BMP180Module::read16(uint8_t addr)
{
#if defined(ENERGIA)
  Wire.beginTransmission(i2caddress);
  Wire.write(addr);
  Wire.endTransmission(false);

  Wire.requestFrom(i2caddress, (uint8_t) 2);    // need to cast int to avoid compiler warnings
  return (Wire.read() << 8 | Wire.read());
#else
  return 0;
#endif
}

MAKE_MODULE(BMP180Module)
