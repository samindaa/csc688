/*
 * TestModule2.h
 *
 *  Created on: Feb 11, 2014
 *      Author: sam
 */

#ifndef TESTMODULE2_H_
#define TESTMODULE2_H_

#include "Template.h"
#include "TestRepresentation1.h"
#include "ISL29023Representation.h"

MODULE(TestModule2)
  REQUIRES(TestRepresentation1) //
  REQUIRES(ISL29023Representation)
END_MODULE

class TestModule2: public TestModule2Base
{
  private:
    uint8_t ledState;
    unsigned long prevTime;
  public:
    TestModule2();
    void execute();

private:
  void blinkfast();
  void blinkslow();
};

#endif /* TESTMODULE2_H_ */
