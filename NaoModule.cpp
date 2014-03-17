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
  STREAM(theMPU9150Representation->fRoll)
  STREAM(theMPU9150Representation->fPitch)
  STREAM(theMPU9150Representation->fYaw)
  STREAM_DELIMITER
#endif
}

MAKE_MODULE(NaoModule)
