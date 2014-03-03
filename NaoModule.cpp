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
  STREAM_SRT
  // Write the
  STREAM(theMPU9150Representation->fRoll)
  STREAM(theMPU9150Representation->fPitch)
  STREAM(theMPU9150Representation->fYaw)
  STREAM_END
#endif
}

MAKE_MODULE(NaoModule)
