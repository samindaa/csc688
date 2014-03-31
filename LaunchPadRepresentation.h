/*
 * LaunchPadRepresentation.h
 *
 *  Created on: Feb 20, 2014
 *      Author: Saminda
 */

#ifndef LAUNCHPADREPRESENTATION_H_
#define LAUNCHPADREPRESENTATION_H_

#include "Template.h"

REPRESENTATION(LaunchPadRepresentation)
class LaunchPadRepresentation: public LaunchPadRepresentationBase
{
  public:
    bool serialActive;
    bool redLEDActive;
    bool greenLEDActive;
    bool blueLEDActive;
    bool push1Active;
    bool push2Active;
    bool wireActive;
    LaunchPadRepresentation() :
        serialActive(false), redLEDActive(false), greenLEDActive(false), blueLEDActive(false), push1Active(
            false), push2Active(false), wireActive(false)
    {
    }
};

#endif /* LAUNCHPADREPRESENTATION_H_ */
