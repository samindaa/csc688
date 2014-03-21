/*
 * RS232Module.h
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#ifndef RS232MODULE_H_
#define RS232MODULE_H_

#if defined(TARGET_NAO)
#include "framework/Module.h"
#else
#include "../Template.h"
#endif

#include "rs232.h"
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "RS232Representation.h"

MODULE(RS232Module)
PROVIDES(RS232Representation)
END_MODULE
class RS232Module: public RS232ModuleBase
{
  private:
    int cport_nr;/* /dev/ttyS0 (COM1 on windows) */
    int bdrate; /* 9600 baud */
    enum
    {
      BUF_SIZE = 512
    };

    unsigned char buf[BUF_SIZE];
    int cPointer;
    bool parserEOR;
    int burnoutPeriod;
  public:
    RS232Module();
    void init();
    void update(RS232Representation& theRS232Representation);
};

#endif /* RS232MODULE_H_ */
