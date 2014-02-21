/*
 * BMP180Module.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Saminda
 */

#ifndef BMP180MODULE_H_
#define BMP180MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "BMP180Representation.h"


/**
 * This module is based-off of:
 *  https://github.com/astuder/BMP180-template-library-Energia/blob/master/BMP180t/BMP180_t.h


 I2C-based template library for Bosch BMP180 digital pressure sensor.
 Created by Adrian Studer, April 2013.

 Distributed under MIT License, see license.txt for details.

 The Bosch BMP180 is a barometric pressure sensor with I2C interface.
 Multiple vendors like Adafruit and Sparkfun sell breakout boards. It is also quite commonly found
 on cheap chinese IMUs, for example the GY-80.

 MSP430
 ------

 Tested with Energia 0101E0010
 * LaunchPad Rev1.5 w/ TI MSP430G2553
 * LaunchPad w/ TI MSP430F5529

 Connections MSP430G2553
 * P1.6 => I2C SCL
 * P1.7 => I2C SDA
 * You might have to remove LED2 jumper for I2C to work properly.

 Connections MSP430F5529
 * P3.0 => I2C SCL
 * P3.1 => I2C SDA

 Tiva and Stellaris
 ------------------

 Tested with Energia 0101E0010
 * Stellaris LaunchPad Rev A, TI LM4F120H5QR
 * Tiva C Series LaunchPad Rev A, TI TM4C123GH6PM

 Connections
 * PD.0 / SCL(3) => I2C SCL
 * PD.1 / SDA(3) => I2C SDA

 Energia uses I2C module 3 as default. To use a different I2C module call
 Wire.setModule(N) before Wire.begin(), where N is the desired module.

 Arduino
 -------

 Tested with Arduino 1.0.4
 Arduino Uno R3, Atmel ATmega328

 Connections
 * A5 => I2C SCL
 * A4 => I2C SDA

 Usage
 -----

 - Instantiate sensor template
 - Initalize Wire I2C library
 - Call **begin()** once when starting up
 - Call **refresh()** to retrive fresh raw data from sensor
 - Raw readings are now available through attributes **rawTemperature** and **rawPressure**
 - Call **calculate()** to calculate temperature and pressure based on raw data
 - Access temperature and pressure through attributes **temperature** and **pressure**

 Examples
 --------

 Instantiating sensor with default settings:

 BMP180<> MySensor;

 Instantiating sensor connecting EOC to pin 1.5, no oversampling:

 BMP180<0,P1_5> MySensor;

 Instantiating sensor for highest precision pressure reading

 BMP180<3> MySensor;

 Instantiating sensor for temperature reading only, output in Fahrenheit

 BMP180<4,0,BMP180_F> MySensor;

 Initalizing I2C and sensor on startup

 Wire.begin();
 MySensor.begin();

 Retrieving a new temperature and pressure reading

 MySensor.refresh();
 MySensor.calculate();
 int myTemperature = MySensor.temperature;
 long myPressure = MySensor.pressure;

 Template
 --------

 BMP180<oversampling, eocpin, tempunit, i2caddress>

 oversampling - Precision of pressure reading
 * 0-3 (low-high), 4=read temperature only, default is 0
 * 0 is fastest (max 10ms), 3 slowest (max 31ms)
 * Oversampling also increases code size by 80-90 bytes
 * Reading temperature only takes 5ms and reduces code size by 700 bytes

 eocpin - Digital pin connected to the sensor's EOC pin
 * 0=not connected, default is 0
 * The EOC pin indicates when sensor data is ready to be read.
 * Using the EOC pin is typically 30% faster than waiting a fixed time when reading sensor data
 * Sketch size grows by 20-200 bytes depending on use of digitalRead() in your sketch

 tempunit - Unit for temperature calculation
 * BMP180_C=Celsius, BMP180_F=Fahrenheit
 * default is BMP180_C

 i2caddress - I2C address of sensor
 * default is 0x77

 Methods
 -------

 * begin - Reads sensor calibration data, configures EOC pin as input (optional)
 * refresh - Retrieves fresh raw data from sensor
 * calculate - Calculates temperature and pressure from raw sensor data

 Attributes
 ----------

 * temperature - Temperature in 0.1 degree Celsius
 * pressure - Pressure in Pascal
 * rawTemperature - Raw temperature reading from sensor
 * rawPressure - Raw pressure reading from sensor

 */


#define DEBUG_BMP180           // uncomment for debug output (init serial in your sketch!)

#define BMP180_I2C_ADDR  0x77  // I2C address of BMP180 sensor, factory default is 0x77
// Bosch BMP180 chip ID
#define BMP180_CHIP_ID   85    // Bosch BMP180 chip identifier - not mentioned in datasheet but found in example source code
// register addresses
#define BMP180_REG_ID    0xd0  // chip ID register
#define BMP180_REG_VER   0xd1  // chip version register
#define BMP180_REG_CTRL  0xf4  // measurement control
#define BMP180_REG_ADC   0xf6  // measurement result (MSB, LSB, XLSB)
#define BMP180_REG_RST   0xe0  // soft reset
#define BMP180_REG_CAL   0xaa  // start of calibration EEPROM
// measurement commands
#define BMP180_CMD_T     0x2e  // measure temperature
#define BMP180_CMD_P     0x34  // measure pressure
MODULE(BMP180Module)
  REQUIRES(LaunchPadRepresentation)
  PROVIDES(BMP180Representation)
END_MODULE
class BMP180Module: public BMP180ModuleBase
{
  protected:
    // structure of calibration EEPROM
    struct BMP180_cal_t
    {
        int16_t ac1;
        int16_t ac2;
        int16_t ac3;
        uint16_t ac4;
        uint16_t ac5;
        uint16_t ac6;
        int16_t b1;
        int16_t b2;
        int16_t mb;
        int16_t mc;
        int16_t md;
    };

    enum BMP180_temp_t
    {
      BMP180_C, BMP180_F
    };

    uint8_t oversampling;
    uint16_t eocpin;
    BMP180_temp_t tempunit;
    uint8_t i2caddress;

    BMP180_cal_t cal;    // Sensor calibration data, initialized during begin()
    //  int32_t       calcX1, calcX2, calcX3;    // temporary variables used in calculations
    //  int32_t       calcB3, calcB5, calcB6;    // code size is smaller if some are declared local
    uint32_t calcB4, calcB7;              // but larger if there are too many..

  public:
    BMP180Module();
    ~BMP180Module();
    void init();
    void update(BMP180Representation& theBMP180Representation);

  private:
    // call once to initialize, reads sensor calibration data
    void begin();
    // reading fresh raw data from sensor
    void refresh(BMP180Representation& theBMP180Representation);
    // calculating temperature and pressure from raw data
    void calculate(BMP180Representation& theBMP180Representation);

    // send command to BMP180
    void sendCmd(uint8_t cmd);
    // read 8 bits from I2C
    uint8_t read8(uint8_t addr);
    // read 16 bits from I2C
    uint16_t read16(uint8_t addr);


};

#endif /* BMP180MODULE_H_ */
