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
#include "comp_dcm.h"

//*****************************************************************************
//
// Define MPU9150 I2C Address.
//
//*****************************************************************************
#define MPU9150_I2C_ADDRESS     0x68


MODULE(MPU9150Module)
REQUIRES(LaunchPadRepresentation)
PROVIDES(MPU9150Representation)
END_MODULE
class MPU9150Module: public MPU9150ModuleBase
{
  private:
    class MPU9150
    {
      public:
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

        //
        // The data buffer used for sending/receiving data to/from the MPU9150.
        //
        uint8_t pui8Data[24];

        class Command
        {
          public:
            //
            // A buffer used to store the write portion of a register read.
            //
            uint8_t pui8Buffer[6];
        };

        Command command;

        MPU9150() :
            ui8Addr(MPU9150_I2C_ADDRESS), ui8AccelAfsSel(0), ui8GyroFsSel(0)
        {
        }
    };

    MPU9150 parameters;

    //*****************************************************************************
    //
    // Global Instance structure to manage the DCM state.
    //
    //*****************************************************************************
    tCompDCM g_sCompDCMInst;
    bool dcmInitialized;

  public:
    MPU9150Module();
    void init();
    void update(MPU9150Representation& theMPU9150Representation);

  private:
    void MPU9150DataAccelGetFloat(MPU9150Representation& theMPU9150Representation);
    void MPU9150DataGyroGetFloat(MPU9150Representation& theMPU9150Representation);
    void MPU9150DataMagnetoGetFloat(MPU9150Representation& theMPU9150Representation);

    void debug(const char* msg, const float& fValue);
};

#endif /* MPU9150MODULE_H_ */
