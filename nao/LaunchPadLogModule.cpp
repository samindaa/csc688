/*
 * LaunchPadLogModule.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: sam
 */

#include "LaunchPadLogModule.h"

LaunchPadLogModule::LaunchPadLogModule()
{
}

LaunchPadLogModule::~LaunchPadLogModule()
{
  ofs.close();
}

void LaunchPadLogModule::init()
{
  ofs.open("forward.txt", std::ios::out | std::ios::app);
}

void LaunchPadLogModule::execute()
{
  if (!theRS232Representation->pfInputs.empty()) // This is test mode
  {
    if (ofs.is_open())
    {
#if defined(TARGET_NAO)
      ofs << theFrameInfo->time_ms << " ";
#endif
      for (std::vector<float>::const_iterator iter = theRS232Representation->pfInputs.begin();
          iter != theRS232Representation->pfInputs.end(); ++iter)
        ofs << *iter << " ";
      ofs << "\n";
      ofs.flush();
    }
  }
}

MAKE_MODULE(LaunchPadLogModule)

