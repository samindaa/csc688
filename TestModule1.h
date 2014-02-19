/*
 * TestModule1.h
 *
 *  Created on: Feb 10, 2014
 *      Author: sam
 */

#ifndef TESTMODULE1_H_
#define TESTMODULE1_H_

#include "Template.h"
#include "TestRepresentation1.h"

MODULE(TestModule1)
PROVIDES(TestRepresentation1)
END_MODULE

class TestModule1: public TestModule1Base
{
  private:
    uint8_t toggle;
public:
  void init();
  void execute();
  void update(TestRepresentation1& theTestRepresentation1);
};

#endif /* TESTMODULE1_H_ */
