/*
 * RS232Module.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#include "RS232Module.h"

RS232Module::RS232Module() :
    cport_nr(24 /* /dev/ttyS0 (COM1 on windows) */), bdrate(115200 /* 9600 baud */)
{
}

void RS232Module::init()
{
  if (RS232_OpenComport(cport_nr, bdrate))
    printf("Can not open comport\n");
}

void RS232Module::execute()
{
  int n = RS232_PollComport(cport_nr, buf, BUF_SIZE - 1);

  if (n > 0)
  {
    buf[n] = 0; /* always put a "null" at the end of a string! */

    for (int i = 0; i < n; i++)
    {
      if ((buf[i] < 32) && (buf[i] > 127)) /* replace unreadable control-codes by dots */
        buf[i] = '';
    }
    printf("received %i bytes: %s\n", n, (char *) buf);
  }

  //usleep(100000); /* sleep for 100 milliSeconds */
}

MAKE_MODULE(RS232Module)

