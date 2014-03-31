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
#else
#include "../Template.h"
#endif

#include "RS232Representation.h"

MODULE(LaunchPadLogModule)
  REQUIRES(RS232Representation)
END_MODULE
class LaunchPadLogModule: public LaunchPadLogModuleBase
{
  public:
    void execute();
};

#endif /* LAUNCHPADLOGMODULE_H_ */
