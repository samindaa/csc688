/*
 * RS232Module.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#include "RS232Module.h"

RS232Module::RS232Module() :
    cport_nr(24 /* /dev/ttyS0 (COM1 on windows) */), bdrate(115200 /* 9600 baud */), cPointer(0), parserEOR(
    false), activeComport(false)
{
}

void RS232Module::init()
{
  activeComport = true;
  if (RS232_OpenComport(cport_nr, bdrate))
  {
    printf("Can not open comport\n");
    activeComport = false;
  }
}

#if defined(TARGET_NAO)
void RS232Module::update(RS232Representation* theRS232Representation)
{
  if (activeComport)
    update(theRS232Representation->pfInputs);
}
#else
void RS232Module::update(RS232Representation& theRS232Representation)
{
  if (activeComport)
    update(theRS232Representation.pfInputs);
}
#endif

void RS232Module::update(std::vector<float>& pfInputs)
{
  pfInputs.clear();
  cPointer = 0;
  parserEOR = false;
  std::stringstream ss;
  while (!parserEOR)
  {
    int n = RS232_PollComport(cport_nr, buf, BUF_SIZE - 1);
    if (n > 0)
    {
      buf[n] = 0; /* always put a "null" at the end of a string! */
      for (int i = 0; i < n; i++)
      {
        if ((buf[i] >= 0x20) && (buf[i] < 0x7F)) /* replace unreadable control-codes by dots */
        {
          if (buf[i] == '|')
          {
            parserEOR = true;
            break;
          }
          else
            ss << buf[i];
        }
      }
      //printf("received %i bytes: %s\n", n, (char *) buf);
    }
  }

  std::istream_iterator<std::string> begin(ss);
  std::istream_iterator<std::string> end;
  std::vector<std::string> psInputs(begin, end);
  if (psInputs.size() % 2 == 0)
  {
    for (size_t i = 0; i < psInputs.size(); i += 2)
    {
      int32_t mantissa = (int32_t) std::atoi(psInputs[i + 0].c_str());
      int16_t exp = (int16_t) std::atoi(psInputs[i + 1].c_str());
      pfInputs.push_back(FloatDetails(mantissa, exp));
    }
  }
}

MAKE_MODULE(RS232Module)

