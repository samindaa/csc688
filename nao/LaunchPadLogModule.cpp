/*
 * LaunchPadLogModule.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: sam
 */

#include "LaunchPadLogModule.h"

void LaunchPadLogModule::execute()
{
  if (true/*Just to test*/ && !theRS232Representation->pfInputs.empty()) // This is test mode
  {
    for (std::vector<float>::const_iterator iter = theRS232Representation->pfInputs.begin();
        iter != theRS232Representation->pfInputs.end(); ++iter)
      std::cout << *iter << " ";
    std::cout << std::endl;
  }
}

MAKE_MODULE(LaunchPadLogModule)

