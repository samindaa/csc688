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
#include "hw_ak8975.h"
#include "comp_dcm.h"

//*****************************************************************************
//
// Define MPU9150 I2C Address.
//
//*****************************************************************************
#define MPU9150_I2C_ADDRESS     0x68

//*****************************************************************************
//
// The states of the MPU9150 state machine.
//
//*****************************************************************************
#define MPU9150_STATE_IDLE      0           // State machine is idle
#define MPU9150_STATE_LAST      1           // Last step in a sequence
#define MPU9150_STATE_READ      2           // Waiting for read
#define MPU9150_STATE_WRITE     3           // Waiting for write
#define MPU9150_STATE_RMW       4           // Waiting for read modify write
#define MPU9150_STATE_INIT_RESET                                              \
                                5           // reset request issued.
#define MPU9150_STATE_INIT_RESET_WAIT                                         \
                                6           // polling wait for reset complete
#define MPU9150_STATE_INIT_PWR_MGMT                                           \
                                7           // wake up the device.
#define MPU9150_STATE_INIT_USER_CTRL                                          \
                                8           // init user control
#define MPU9150_STATE_INIT_SAMPLE_RATE_CFG                                    \
                                9           // init the sensors and filters
#define MPU9150_STATE_INIT_I2C_SLAVE_DLY                                      \
                                10          // set the ak8975 polling delay
#define MPU9150_STATE_INIT_I2C_SLAVE_0                                        \
                                11          // config ak8975 automatic read
#define MPU9150_STATE_RD_DATA   12          // Waiting for data read


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
        // The state of the state machine used while accessing the MPU9150.
        //
        uint8_t ui8State;

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
            ui8Addr(MPU9150_I2C_ADDRESS), ui8State(0), ui8AccelAfsSel(0), ui8GyroFsSel(0)
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

    void I2CMWrite(const uint8_t& ui8Count);
    void I2CMRead(const uint8_t& ui8CountOut, const uint8_t& ui8CountIn);
    void debug(const char* msg, const float& fValue);
};

#endif /* MPU9150MODULE_H_ */
