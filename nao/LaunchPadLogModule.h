/*
 * LaunchPadLogModule.h
 *
 *  Created on: Mar 31, 2014
 *      Author: sam
 */

#ifndef LAUNCHPADLOGMODULE_H_
#define LAUNCHPADLOGMODULE_H_

#if defined(TARGET_NAO)
#include "framework/Module.h"
#include "representations/perception/FrameInfo.h"
#include "representations/modeling/GroundContactState.h"
#include "representations/motion/WalkingEngineOutput.h"
#else
#include "../Template.h"
#endif

#include "RS232Representation.h"

#include <iostream>
#include <fstream>

MODULE(LaunchPadLogModule)
  REQUIRES(RS232Representation) //
#if defined(TARGET_NAO)
  REQUIRES(FrameInfo) //
  REQUIRES(GroundContactState) //
  REQUIRES(WalkingEngineOutput) //
#endif
END_MODULE
class LaunchPadLogModule: public LaunchPadLogModuleBase
{
  private:
    std::ofstream ofs;
    //bool collectSamples;
    //uint32_t vTime;

  public:
    LaunchPadLogModule();
    ~LaunchPadLogModule();
    void init();
    void execute();

#if defined(TARGET_NAO)
    //void update(LedRequest* theLedRequest);
#endif
};

#endif /* LAUNCHPADLOGMODULE_H_ */
