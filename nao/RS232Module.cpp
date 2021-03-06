/*
 * RS232Module.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#include "RS232Module.h"

RS232Module::RS232Module() :
    cport_nr(24 /* /dev/ttyS0 (COM1 on windows) */), bdrate(115200), pSOF(false), active(false)
{
}

void RS232Module::init()
{
  active = true;
  if (RS232_OpenComport(cport_nr, bdrate))
  {
    printf("Can not open comport\n");
    active = false;
  }
}

#if defined(TARGET_NAO)
void RS232Module::update(RS232Representation* theRS232Representation)
{
  if (active)
  update(theRS232Representation->pfInputs);
}
#else
void RS232Module::update(RS232Representation& theRS232Representation)
{
  if (active)
    update(theRS232Representation.pfInputs);
}
#endif

void RS232Module::update(RS232Representation::Inputs& pfInputs)
{
  pfInputs.clear();
  int n = RS232_PollComport(cport_nr, buf, BUF_SIZE - 1);
  if (n > 0)
  {
    buf[n] = 0;
    for (int i = 0; i < n; i++)
    {
      if ((buf[i] >= 0x20) && (buf[i] < 0x7F)) /* TEXT */
      {
        if (buf[i] == '{')
        {
          // Start of the stream
          pSOF = true;
          ss.clear();
        }
        else if (buf[i] == '}')
        {
          // Potential end has reached
          if (pSOF)
          {
            pSOF = false;
            pfInputs.clear(); //<< most recent reading
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> psInputs(begin, end);
            std::vector<float> fInputs;
            if (!psInputs.empty())
            {
              for (size_t i = 0; i < psInputs.size(); i += 2)
              {
                int32_t mantissa = (int32_t) std::atoi(psInputs[i + 0].c_str());
                int16_t exp = (int16_t) std::atoi(psInputs[i + 1].c_str());
                fInputs.push_back(FloatDetails(mantissa, exp));
              }
              pfInputs.insert(std::make_pair(pfInputs.size(), fInputs));
            }
          }
        }
        else
        {
          // Collect data
          if (pSOF)
            ss << buf[i];
        }

      }
    }
    printf("received %i bytes: %s\n", n, (char *) buf);
  }
  // debug
  //myDebug(pfInputs);
}

void RS232Module::myDebug(RS232Representation::Inputs& pfInputs)
{
  for (RS232Representation::Inputs::iterator iter = pfInputs.begin(); iter != pfInputs.end();
      ++iter)
  {
    for (std::vector<float>::iterator iter2 = iter->second.begin(); iter2 != iter->second.end();
        ++iter2)
      std::cout << *iter2 << " ";
  }
  if (!pfInputs.empty())
    std::cout << std::endl;
}

MAKE_MODULE(RS232Module)

