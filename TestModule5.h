/*
 * TestModule5.h
 *
 *  Created on: Aug 27, 2014
 *      Author: sam
 */

#ifndef TESTMODULE5_H_
#define TESTMODULE5_H_

#include "Template.h"

#if defined(ENERGIA)
//#include "lprf.h"
#endif

MODULE(TestModule5)
END_MODULE
class TestModule5: public TestModule5Base
{
  private:
    //testing
    uint8_t ledState;
    unsigned long prevTime;

  public:
    TestModule5();
    void init();
    void execute();
};

#endif /* TESTMODULE5_H_ */
