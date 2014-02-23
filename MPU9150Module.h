/*
 * MPU9150Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef MPU9150MODULE_H_
#define MPU9150MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "MPU9150Representation.h"
#include "hw_mpu9150.h"

//*****************************************************************************
//
// Define MPU9150 I2C Address.
//
//*****************************************************************************
#define MPU9150_I2C_ADDRESS     0x68

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




MODULE(MPU9150Module)
REQUIRES(LaunchPadRepresentation)
PROVIDES(MPU9150Representation)
END_MODULE
class MPU9150Module: public MPU9150ModuleBase
{
  private:
    //
    // The I2C address of the MPU9150.
    //
    uint8_t ui8Addr;

    //
    // The current accelerometer afs_sel setting
    //
    uint8_t ui8AccelAfsSel;

    //
    // The current gyroscope fs_sel setting
    //
    uint8_t ui8GyroFsSel;


  public:
    MPU9150Module();
    void update(MPU9150Representation& theMPU9150Representation);
};

#endif /* MPU9150MODULE_H_ */
