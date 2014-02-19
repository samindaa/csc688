/*
 * TestModule3.h
 *
 *  Created on: Feb 18, 2014
 *      Author: sam
 */

#ifndef TESTMODULE3_H_
#define TESTMODULE3_H_

#include "Template.h"

MODULE(TestModule3)
END_MODULE

class TestModule3: public TestModule3Base
{
public:
  void execute();
};

#endif /* TESTMODULE3_H_ */
