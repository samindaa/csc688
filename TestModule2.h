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
#include "TestRepresentation2.h"

MODULE(TestModule2)
REQUIRES(TestRepresentation1)
REQUIRES(TestRepresentation2)
END_MODULE

class TestModule2: public TestModule2Base
{
public:
  void init();
  void execute();

private:
  void blinkfast();
  void blinkslow();
};

#endif /* TESTMODULE2_H_ */
