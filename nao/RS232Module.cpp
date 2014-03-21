/*
 * RS232Module.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#include "RS232Module.h"

RS232Module::RS232Module() :
    cport_nr(24 /* /dev/ttyS0 (COM1 on windows) */), bdrate(115200 /* 9600 baud */), cPointer(0), parserEOR(
        false), burnoutPeriod(0)
{
}

void RS232Module::init()
{
  if (RS232_OpenComport(cport_nr, bdrate))
    printf("Can not open comport\n");
}

void RS232Module::update(RS232Representation& theRS232Representation)
{
  theRS232Representation.pfInputs.clear();
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
  if (burnoutPeriod > 10)
  {
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> psInputs(begin, end);
    for (std::vector<std::string>::iterator iter = psInputs.begin(); iter != psInputs.end(); ++iter)
      theRS232Representation.pfInputs.push_back((float) std::atof((*iter).c_str()));

    //usleep(100000); /* sleep for 100 milliSeconds */
    // debug
    for (std::vector<float>::const_iterator iter = theRS232Representation.pfInputs.begin();
        iter != theRS232Representation.pfInputs.end(); ++iter)
      std::cout << *iter << " ";
    std::cout << std::endl;
  }
  else
    ++burnoutPeriod;
}

MAKE_MODULE(RS232Module)

