/*
 * MPU9150Module.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#include "MPU9150Module.h"
#if defined(ENERGIA)
#include "Wire.h"
#endif

//*****************************************************************************
//
// Define MPU9150 data sampling frequency.
//
//*****************************************************************************
#define MOTION_SAMPLE_FREQ_HZ   50

//*****************************************************************************
//
// Weights the DCM should use for each sensor.  Must add to 1.0
//
//*****************************************************************************
#define DCM_MAG_WEIGHT          0.2f
#define DCM_GYRO_WEIGHT         0.6f
#define DCM_ACCEL_WEIGHT        0.2f

//*****************************************************************************
//
// Define the states of the motion state machine.
//
//*****************************************************************************
#define TO_DEG(a)               ((a) * 57.295779513082320876798154814105f)

//*****************************************************************************
//
// The factors used to convert the acceleration readings from the MPU9150 into
// floating point values in meters per second squared.
//
// Values are obtained by taking the g conversion factors from the data sheet
// and multiplying by 9.81 (1 g = 9.81 m/s^2).
//
//*****************************************************************************
static const float g_fMPU9150AccelFactors[] =
{ 0.0005985482,                           // Range = +/- 2 g (16384 lsb/g)
    0.0011970964,                           // Range = +/- 4 g (8192 lsb/g)
    0.0023941928,                           // Range = +/- 8 g (4096 lsb/g)
    0.0047883855                            // Range = +/- 16 g (2048 lsb/g)
    };

//*****************************************************************************
//
// The factors used to convert the acceleration readings from the MPU9150 into
// floating point values in radians per second.
//
// Values are obtained by taking the degree per second conversion factors
// from the data sheet and then converting to radians per sec (1 degree =
// 0.0174532925 radians).
//
//*****************************************************************************
static const float g_fMPU9150GyroFactors[] =
{ 1.3323124e-4,                           // Range = +/- 250 dps (131.0)
    2.6646248e-4,                           // Range = +/- 500 dps (65.5)
    5.3211258e-4,                           // Range = +/- 1000 dps (32.8)
    0.0010642252                            // Range = +/- 2000 dps (16.4)
    };

//*****************************************************************************
//
// Converting sensor data to tesla (0.3 uT per LSB)
//
//*****************************************************************************
#define CONVERT_TO_TESLA        0.0000003

MPU9150Module::MPU9150Module() :
    dcmInitialized(false)
{
}

void MPU9150Module::init()
{
#if defined(ENERGIA)
  //
  // Initialize the MPU9150 Driver.
  //
  //
  // Default range setting is +/- 2 g
  //
  parameters.ui8AccelAfsSel = (MPU9150_ACCEL_CONFIG_AFS_SEL_2G >> MPU9150_ACCEL_CONFIG_AFS_SEL_S);

  //
  // Default range setting is +/- 250 degrees/s
  //
  parameters.ui8GyroFsSel = (MPU9150_GYRO_CONFIG_FS_SEL_250 >> MPU9150_GYRO_CONFIG_FS_SEL_S);

  //
  // Load the buffer with command to perform device reset
  //
  parameters.command.pui8Buffer[0] = MPU9150_O_PWR_MGMT_1;
  parameters.command.pui8Buffer[1] = MPU9150_PWR_MGMT_1_DEVICE_RESET;
  Wire.beginTransmission(parameters.ui8Addr);
  for (int i = 0; i < 2; i++)
    Wire.write(parameters.command.pui8Buffer[i]);
  Wire.endTransmission();

  //
  // Write application specifice sensor configuration such as filter settings
  // and sensor range settings.
  //
  parameters.pui8Data[0] = MPU9150_CONFIG_DLPF_CFG_94_98;
  parameters.pui8Data[1] = MPU9150_GYRO_CONFIG_FS_SEL_250;
  parameters.pui8Data[2] = (MPU9150_ACCEL_CONFIG_ACCEL_HPF_5HZ | MPU9150_ACCEL_CONFIG_AFS_SEL_2G);
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(MPU9150_O_CONFIG);
  for (int i = 0; i < 3; i++)
    Wire.write(parameters.pui8Data[i]);
  Wire.endTransmission();

  //
  // Initialize the DCM system.
  //
  CompDCMInit(&g_sCompDCMInst, 1.0f / ((float) MOTION_SAMPLE_FREQ_HZ), DCM_ACCEL_WEIGHT,
  DCM_GYRO_WEIGHT, DCM_MAG_WEIGHT);

#endif

}

void MPU9150Module::update(MPU9150Representation& theMPU9150Representation)
{
#if defined(ENERGIA)
  //
  // Read the data registers from the MPU9150.
  //
  // (ACCEL_XOUT_H(0x3B) -> GYRO_ZOUT_L(0x48) = 14 bytes
  // Grab Ext Sens Data as well for another 8 bytes.  ST1 + Mag Data + ST2
  //
  Wire.beginTransmission(parameters.ui8Addr);
  Wire.write(MPU9150_O_ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(parameters.ui8Addr, (uint8_t) 22);
  while (Wire.available() < 21)
    ;
  for (int i = 0; i < 22; i++)
    parameters.pui8Data[i] = Wire.read();

  MPU9150DataAccelGetFloat(theMPU9150Representation);
  debug("AccX: ", theMPU9150Representation.fAccelX);
  debug("AccY: ", theMPU9150Representation.fAccelY);
  debug("AccxZ: ", theMPU9150Representation.fAccelZ);
  MPU9150DataMagnetoGetFloat(theMPU9150Representation);
  debug("MagnetoX: ", theMPU9150Representation.fMagnetoX);
  debug("MagnetoY: ", theMPU9150Representation.fMagnetoY);
  debug("MagnetoZ: ", theMPU9150Representation.fMagnetoZ);
  MPU9150DataGyroGetFloat(theMPU9150Representation);
  debug("GyroX: ", theMPU9150Representation.fGyroX);
  debug("GyroY: ", theMPU9150Representation.fGyroY);
  debug("GyroZ: ", theMPU9150Representation.fGyroZ);

  if (false)
  {
    if (!dcmInitialized)
    {
      dcmInitialized = true;
      //
      // Feed the initial measurements to the DCM and start it.
      // Due to the structure of our MotionMagCallback function,
      // the floating point magneto data is already in the local
      // data buffer.
      //
      CompDCMMagnetoUpdate(&g_sCompDCMInst, theMPU9150Representation.fMagnetoX,
          theMPU9150Representation.fMagnetoY, theMPU9150Representation.fMagnetoZ);
      CompDCMAccelUpdate(&g_sCompDCMInst, theMPU9150Representation.fAccelX,
          theMPU9150Representation.fAccelY, theMPU9150Representation.fAccelZ);
      CompDCMStart(&g_sCompDCMInst);
    }

    //
    // Get the latest Euler data from the DCM. DCMUpdate is done
    // inside the interrupt routine to insure it is not skipped and
    // that the timing is consistent.
    //
    CompDCMComputeEulers(&g_sCompDCMInst, &theMPU9150Representation.fRoll,
        &theMPU9150Representation.fPitch, &theMPU9150Representation.fYaw);
  }

#endif
}

void MPU9150Module::MPU9150DataAccelGetFloat(MPU9150Representation& theMPU9150Representation)
{
#if defined(ENERGIA)
  //
  // Get the acceleration conversion factor for the current data format.
  //
  float fFactor = g_fMPU9150AccelFactors[parameters.ui8AccelAfsSel];

  //
  // Convert the accelerometer values into m/sec^2
  //
  theMPU9150Representation.fAccelX = ((float) (int16_t) ((parameters.pui8Data[0] << 8)
      | parameters.pui8Data[1]) * fFactor);
  theMPU9150Representation.fAccelY = ((float) (int16_t) ((parameters.pui8Data[2] << 8)
      | parameters.pui8Data[3]) * fFactor);
  theMPU9150Representation.fAccelZ = ((float) (int16_t) ((parameters.pui8Data[4] << 8)
      | parameters.pui8Data[5]) * fFactor);
#endif
}

void MPU9150Module::MPU9150DataGyroGetFloat(MPU9150Representation& theMPU9150Representation)
{

#if defined(ENERGIA)
  float fFactor;
  int16_t i16Temp;

  //
  // Get the gyroscope conversion factor for the current data format.
  //
  fFactor = g_fMPU9150GyroFactors[parameters.ui8GyroFsSel];

  //
  // Convert the gyroscope values into rad/sec
  //
  i16Temp = (int16_t) parameters.pui8Data[8];
  i16Temp <<= 8;
  i16Temp += parameters.pui8Data[9];
  theMPU9150Representation.fGyroX = (float) i16Temp;
  theMPU9150Representation.fGyroX *= fFactor;

  i16Temp = (int16_t) parameters.pui8Data[10];
  i16Temp <<= 8;
  i16Temp += parameters.pui8Data[11];
  theMPU9150Representation.fGyroY = (float) i16Temp;
  theMPU9150Representation.fGyroY *= fFactor;

  i16Temp = (int16_t) parameters.pui8Data[12];
  i16Temp <<= 8;
  i16Temp += parameters.pui8Data[13];
  theMPU9150Representation.fGyroZ = (float) i16Temp;
  theMPU9150Representation.fGyroZ *= fFactor;

#endif
}

void MPU9150Module::MPU9150DataMagnetoGetFloat(MPU9150Representation& theMPU9150Representation)
{

#if defined(ENERGIA)
  int16_t *pi16Data = (int16_t *) (parameters.pui8Data + 15);

  //
  // Convert the magnetometer values into floating-point tesla values.
  //
  theMPU9150Representation.fMagnetoX = (float) pi16Data[0];
  theMPU9150Representation.fMagnetoX *= CONVERT_TO_TESLA;

  theMPU9150Representation.fMagnetoY = (float) pi16Data[1];
  theMPU9150Representation.fMagnetoY *= CONVERT_TO_TESLA;

  theMPU9150Representation.fMagnetoZ = (float) pi16Data[2];
  theMPU9150Representation.fMagnetoZ *= CONVERT_TO_TESLA;
#endif
}

void MPU9150Module::debug(const char* msg, const float& fValue)
{
#if defined(ENERGIA)
  Serial.print(msg);
  int32_t i32IntegerPart = (int32_t) fValue;
  int32_t i32FractionPart = (int32_t) (fValue * 1000.0f);
  i32FractionPart = i32FractionPart - (i32IntegerPart * 1000);
  if (i32FractionPart < 0)
    i32FractionPart *= -1;
  Serial.print(i32IntegerPart);
  Serial.print(".");
  Serial.println(i32FractionPart);
#endif
}

MAKE_MODULE(MPU9150Module)

