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
#else
#include "../Template.h"
#endif

#include "RS232Representation.h"

#include <iostream>
#include <fstream>

MODULE(LaunchPadLogModule)
  REQUIRES(RS232Representation) //
#if defined(TARGET_NAO)
  REQUIRES(FrameInfo)
#endif
END_MODULE
class LaunchPadLogModule: public LaunchPadLogModuleBase
{
  private:
    std::ofstream ofs;
  public:
    LaunchPadLogModule();
    ~LaunchPadLogModule();
    void init();
    void execute();
};

#endif /* LAUNCHPADLOGMODULE_H_ */
