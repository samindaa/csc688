/*
 * NaoModule.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#include "NaoModule.h"

void NaoModule::execute()
{
#if defined(ENERGIA)
  // Write the
  STREAM(theMPU9150Representation->fAccelX)
  STREAM(theMPU9150Representation->fAccelY)
  STREAM(theMPU9150Representation->fAccelZ)
  STREAM(theMPU9150Representation->fGyroX)
  STREAM(theMPU9150Representation->fGyroY)
  STREAM(theMPU9150Representation->fGyroZ)
  STREAM(theMPU9150Representation->fMagnetoX)
  STREAM(theMPU9150Representation->fMagnetoY)
  STREAM(theMPU9150Representation->fMagnetoZ)
  STREAM(theMPU9150Representation->fRoll)
  STREAM(theMPU9150Representation->fPitch)
  STREAM(theMPU9150Representation->fYaw)
  STREAM_DELIMITER
#endif
}

MAKE_MODULE(NaoModule)
