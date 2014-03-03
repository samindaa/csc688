/*
 * MPU9150Module.cpp
 *
 *  Created on: Feb 25, 2014
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

  parameters.ui8State = MPU9150_STATE_INIT_RESET;
  parameters.command.pui8Buffer[0] = MPU9150_O_PWR_MGMT_1;
  parameters.command.pui8Buffer[1] = MPU9150_PWR_MGMT_1_DEVICE_RESET;
  I2CMWrite(2);
  while (parameters.ui8State != MPU9150_STATE_LAST)
  {
    switch (parameters.ui8State)
    {
    //
    // All states that trivially transition to IDLE, and all unknown
    // states.
    //
    case MPU9150_STATE_READ:
    case MPU9150_STATE_LAST:
    case MPU9150_STATE_RD_DATA:
    default:
    {
      //
      // The state machine is now idle.
      //
      parameters.ui8State = MPU9150_STATE_IDLE;

      //
      // Done.
      //
      break;
    }

      //
      // MPU9150 Device reset was issued
      //
    case MPU9150_STATE_INIT_RESET:
    {
      //
      // Issue a read of the status register to confirm reset is done.
      //
      parameters.command.pui8Buffer[0] = MPU9150_O_PWR_MGMT_1;
      I2CMRead(1, 1);
      parameters.ui8State = MPU9150_STATE_INIT_RESET_WAIT;
      break;
    }

      //
      // Status register was read, check if reset is done before proceeding.
      //
    case MPU9150_STATE_INIT_RESET_WAIT:
    {
      //
      // Check the value read back from status to determine if device
      // is still in reset or if it is ready.  Reset state for this
      // register is 0x40, which has sleep bit set. Device may also
      // respond with an address NACK during very early stages of the
      // its internal reset.  Keep polling until we verify device is
      // ready.
      //
      if ((parameters.pui8Data[0] != MPU9150_PWR_MGMT_1_SLEEP))
      {
        //
        // Device still in reset so begin polling this register.
        //
        parameters.command.pui8Buffer[0] = MPU9150_O_PWR_MGMT_1;
        I2CMRead(1, 1);
        //
        // Intentionally stay in this state to create polling effect.
        //
      }
      else
      {
        //
        // Device is out of reset, bring it out of sleep mode.
        //
        parameters.command.pui8Buffer[0] = MPU9150_O_PWR_MGMT_1;
        parameters.command.pui8Buffer[1] = MPU9150_PWR_MGMT_1_CLKSEL_XG;
        I2CMWrite(2);

        //
        // Update state to show we are modifing user control and
        // power management 1 regs.
        //
        parameters.ui8State = MPU9150_STATE_INIT_PWR_MGMT;
      }
      break;
    }

      //
      // Reset complete now take device out of sleep mode.
      //
    case MPU9150_STATE_INIT_PWR_MGMT:
    {
      parameters.command.pui8Buffer[0] = MPU9150_O_USER_CTRL;
      parameters.command.pui8Buffer[1] = MPU9150_USER_CTRL_I2C_MST_EN;
      I2CMWrite(2);

      //
      // Update state to show we are modifing user control and
      // power management 1 regs.
      //
      parameters.ui8State = MPU9150_STATE_INIT_USER_CTRL;

      break;
    }

      //
      // Change to power mode complete, device is ready for configuration.
      //
    case MPU9150_STATE_INIT_USER_CTRL:
    {
      //
      // Load index 0 with the sample rate register number.
      //
      parameters.command.pui8Buffer[0] = MPU9150_O_SMPLRT_DIV;

      //
      // Set sample rate to 50 hertz.  1000 hz / (1 + 19)
      //
      parameters.command.pui8Buffer[1] = 19;
      I2CMWrite(2);

      //
      // update state to show are in process of configuring sensors.
      //
      parameters.ui8State = MPU9150_STATE_INIT_SAMPLE_RATE_CFG;
      break;
    }

      //
      // Sensor configuration is complete.
      //
    case MPU9150_STATE_INIT_SAMPLE_RATE_CFG:
    {
      //
      // Write the I2C Master delay control so we only sample the AK
      // every 5th time that we sample accel/gyro.  Delay Count itself
      // handled in next state.
      //
      parameters.command.pui8Buffer[0] = MPU9150_O_I2C_MST_DELAY_CTRL;
      parameters.command.pui8Buffer[1] = (MPU9150_I2C_MST_DELAY_CTRL_I2C_SLV0_DLY_EN |
      MPU9150_I2C_MST_DELAY_CTRL_I2C_SLV4_DLY_EN);
      I2CMWrite(2);

      //
      // Update state to show we are configuring i2c slave delay between
      // slave events.  Slave 0 and Slave 4 transaction only occur every
      // 5th sample cycle.
      //
      parameters.ui8State = MPU9150_STATE_INIT_I2C_SLAVE_DLY;
      break;
    }

      //
      // Master slave delay configuration complete.
      //
    case MPU9150_STATE_INIT_I2C_SLAVE_DLY:
    {
      //
      // Write the configuration for I2C master control clock 400khz
      // and wait for external sensor before asserting data ready
      //
      parameters.command.pui8Buffer[0] = MPU9150_O_I2C_MST_CTRL;
      parameters.command.pui8Buffer[1] = (MPU9150_I2C_MST_CTRL_I2C_MST_CLK_400 |
      MPU9150_I2C_MST_CTRL_WAIT_FOR_ES);

      //
      // Configure I2C Slave 0 for read of AK8975 (I2C Address 0x0C)
      // Start at AK8975 register status 1
      // Read 8 bytes and enable this slave transaction
      //
      parameters.command.pui8Buffer[2] = MPU9150_I2C_SLV0_ADDR_RW | 0x0C;
      parameters.command.pui8Buffer[3] = AK8975_O_ST1;
      parameters.command.pui8Buffer[4] = MPU9150_I2C_SLV0_CTRL_EN | 0x08;
      I2CMWrite(5);

      //
      // Update state.  Now in process of configuring slave 0.
      //
      parameters.ui8State = MPU9150_STATE_INIT_I2C_SLAVE_0;
      break;
    }

      //
      // I2C slave 0 init complete.
      //
    case MPU9150_STATE_INIT_I2C_SLAVE_0:
    {
      //
      // Write the configuration for I2C Slave 4 transaction to AK8975
      // 0x0c is the AK8975 address on i2c bus.
      // we want to write the control register with the value for a
      // starting a single measurement.
      //
      parameters.command.pui8Buffer[0] = MPU9150_O_I2C_SLV4_ADDR;
      parameters.command.pui8Buffer[1] = 0x0C;
      parameters.command.pui8Buffer[2] = AK8975_O_CNTL;
      parameters.command.pui8Buffer[3] = AK8975_CNTL_MODE_SINGLE;

      //
      // Enable the SLV4 transaction and set the master delay to
      // 0x04 + 1.  This means the slave transactions with delay enabled
      // will run every fifth accel/gyro sample.
      //
      parameters.command.pui8Buffer[4] = MPU9150_I2C_SLV4_CTRL_EN | 0x04;
      I2CMWrite(5);

      //
      // Update state.  Now in the final init state.
      //
      parameters.ui8State = MPU9150_STATE_LAST;
      break;
    }

    };
  }

  //
  // Write application specifice sensor configuration such as filter settings
  // and sensor range settings.
  //
  parameters.command.pui8Buffer[0] = MPU9150_O_CONFIG;
  parameters.command.pui8Buffer[1] = MPU9150_CONFIG_DLPF_CFG_94_98;
  parameters.command.pui8Buffer[2] = MPU9150_GYRO_CONFIG_FS_SEL_250;
  parameters.command.pui8Buffer[3] = (MPU9150_ACCEL_CONFIG_ACCEL_HPF_5HZ |
  MPU9150_ACCEL_CONFIG_AFS_SEL_2G);
  I2CMWrite(4);

  //
  // Configure the data ready interrupt pin output of the MPU9150.
  //
  parameters.command.pui8Buffer[0] = MPU9150_O_INT_PIN_CFG;
  parameters.command.pui8Buffer[1] = MPU9150_INT_PIN_CFG_INT_LEVEL |
  MPU9150_INT_PIN_CFG_INT_RD_CLEAR |
  MPU9150_INT_PIN_CFG_LATCH_INT_EN;
  parameters.command.pui8Buffer[2] = MPU9150_INT_ENABLE_DATA_RDY_EN;
  I2CMWrite(3);

  //
  // Initialize the DCM system. 50 hz sample rate.
  // accel weight = .2, gyro weight = .8, mag weight = .2
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
  parameters.command.pui8Buffer[0] = MPU9150_O_ACCEL_XOUT_H;
  I2CMRead(1, 22);

  MPU9150DataAccelGetFloat(theMPU9150Representation);
  //debug("AccX: ", theMPU9150Representation.fAccelX);
  //debug("AccY: ", theMPU9150Representation.fAccelY);
  //debug("AccxZ: ", theMPU9150Representation.fAccelZ);
  MPU9150DataGyroGetFloat(theMPU9150Representation);
  //debug("GyroX: ", theMPU9150Representation.fGyroX);
  //debug("GyroY: ", theMPU9150Representation.fGyroY);
  //debug("GyroZ: ", theMPU9150Representation.fGyroZ);
  MPU9150DataMagnetoGetFloat(theMPU9150Representation);
  //debug("MagnetoX: ", theMPU9150Representation.fMagnetoX);
  //debug("MagnetoY: ", theMPU9150Representation.fMagnetoY);
  //debug("MagnetoZ: ", theMPU9150Representation.fMagnetoZ);

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
    CompDCMGyroUpdate(&g_sCompDCMInst, theMPU9150Representation.fGyroX,
        theMPU9150Representation.fGyroY, theMPU9150Representation.fGyroZ);
    CompDCMStart(&g_sCompDCMInst);
  }
  else
  {
    CompDCMMagnetoUpdate(&g_sCompDCMInst, theMPU9150Representation.fMagnetoX,
        theMPU9150Representation.fMagnetoY, theMPU9150Representation.fMagnetoZ);
    CompDCMAccelUpdate(&g_sCompDCMInst, theMPU9150Representation.fAccelX,
        theMPU9150Representation.fAccelY, theMPU9150Representation.fAccelZ);
    CompDCMGyroUpdate(&g_sCompDCMInst, -theMPU9150Representation.fGyroX,
        -theMPU9150Representation.fGyroY, -theMPU9150Representation.fGyroZ);
    CompDCMUpdate(&g_sCompDCMInst);
  }

  //
  // Get Euler data. (Roll Pitch Yaw)
  //
  CompDCMComputeEulers(&g_sCompDCMInst, &theMPU9150Representation.fRoll,
      &theMPU9150Representation.fPitch, &theMPU9150Representation.fYaw);

  //
  // Get Quaternions.
  //
  CompDCMComputeQuaternion(&g_sCompDCMInst, theMPU9150Representation.fQuaternion);

  //
  // convert mag data to micro-tesla for better human interpretation.
  //
  theMPU9150Representation.fMagnetoX *= 1e6;
  theMPU9150Representation.fMagnetoY *= 1e6;
  theMPU9150Representation.fMagnetoZ *= 1e6;

  //
  // Convert Eulers to degrees. 180/PI = 57.29...
  // Convert Yaw to 0 to 360 to approximate compass headings.
  //
  theMPU9150Representation.fRoll *= 57.295779513082320876798154814105f;
  theMPU9150Representation.fPitch *= 57.295779513082320876798154814105f;
  theMPU9150Representation.fYaw *= 57.295779513082320876798154814105f;
  if (theMPU9150Representation.fYaw < 0)
  {
    theMPU9150Representation.fYaw += 360.0f;
  }

  //
  // Get the latest Euler data from the DCM. DCMUpdate is done
  // inside the interrupt routine to insure it is not skipped and
  // that the timing is consistent.
  //
  //debug("fRoll: ", theMPU9150Representation.fRoll);
  //debug("fPitch: ", theMPU9150Representation.fPitch);
  //debug("fYaw: ", theMPU9150Representation.fYaw);

#endif
}

void MPU9150Module::I2CMWrite(const uint8_t& ui8Count)
{
#if defined(ENERGIA)
  Wire.beginTransmission(parameters.ui8Addr);
  for (uint8_t i = 0; i < ui8Count; i++)
    Wire.write(parameters.command.pui8Buffer[i]);
  Wire.endTransmission();
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

void MPU9150Module::I2CMRead(const uint8_t& ui8CountOut, const uint8_t& ui8CountIn)
{
#if defined(ENERGIA)
  Wire.beginTransmission(parameters.ui8Addr);
  for (uint8_t i = 0; i < ui8CountOut; i++)
    Wire.write(parameters.command.pui8Buffer[i]);
  Wire.endTransmission(false);
  Wire.requestFrom(parameters.ui8Addr, ui8CountIn);
  if (ui8CountIn == 1)
  {
    while (Wire.available() == 0)
      ;
  }
  else
  {
    while (Wire.available() < ui8CountIn - 1)
      ;
  }
  for (uint8_t i = 0; i < ui8CountIn; i++)
    parameters.pui8Data[i] = Wire.read();
#endif
}

void MPU9150Module::debug(const char* msg, const float& fValue)
{
#if defined(ENERGIA)
  Serial.print(msg);
  Serial.println(fValue, 3);
#endif
}
MAKE_MODULE(MPU9150Module)
