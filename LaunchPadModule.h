/*
 * LaunchPadModule.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Saminda
 */

#ifndef LAUNCHPADMODULE_H_
#define LAUNCHPADMODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"

MODULE(LaunchPadModule)
PROVIDES(LaunchPadRepresentation)
END_MODULE
class LaunchPadModule : public LaunchPadModuleBase
{
  public:
    void init();
    void update(LaunchPadRepresentation& theLaunchPadRepresentation);
};

#endif /* LAUNCHPADMODULE_H_ */
