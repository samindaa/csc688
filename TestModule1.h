/*
 * TestModule1.h
 *
 *  Created on: Feb 10, 2014
 *      Author: sam
 */

#ifndef TESTMODULE1_H_
#define TESTMODULE1_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "TestRepresentation1.h"

MODULE(TestModule1)
  REQUIRES(LaunchPadRepresentation) //
  PROVIDES(TestRepresentation1)
END_MODULE

class TestModule1: public TestModule1Base
{
public:
  void update(TestRepresentation1& theTestRepresentation1);
};

#endif /* TESTMODULE1_H_ */
