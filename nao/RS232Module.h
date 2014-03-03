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
#include "Template.h"
#endif

#include "rs232.h"

MODULE(RS232Module)
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
  public:
    RS232Module();
    void init();
    void execute();
};

#endif /* RS232MODULE_H_ */
