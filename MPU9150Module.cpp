/*
 * MPU9150Module.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#include "MPU9150Module.h"

MPU9150Module::MPU9150Module() :
    ui8Addr(MPU9150_I2C_ADDRESS)
{
  //
  // Default range setting is +/- 2 g
  //
  ui8AccelAfsSel = (MPU9150_ACCEL_CONFIG_AFS_SEL_2G >> MPU9150_ACCEL_CONFIG_AFS_SEL_S);

  //
  // Default range setting is +/- 250 degrees/s
  //
  ui8GyroFsSel = (MPU9150_GYRO_CONFIG_FS_SEL_250 >> MPU9150_GYRO_CONFIG_FS_SEL_S);
}

void MPU9150Module::update(MPU9150Representation& theMPU9150Representation)
{
  // TODO:
}

MAKE_MODULE(MPU9150Module)

