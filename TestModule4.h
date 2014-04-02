/*
 * TestModule4.h
 *
 *  Created on: Apr 1, 2014
 *      Author: sam
 */

#ifndef TESTMODULE4_H_
#define TESTMODULE4_H_

#include "Template.h"
#include "MPU9150Representation.h"

MODULE(TestModule4)
  REQUIRES(MPU9150Representation) //
END_MODULE
class TestModule4: public TestModule4Base
{
  public:
    void init();
    void execute();
};

#endif /* TESTMODULE4_H_ */
