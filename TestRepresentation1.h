/*
 * TestRepresentation1.h
 *
 *  Created on: Feb 10, 2014
 *      Author: sam
 */

#ifndef TESTREPRESENTATION1_H_
#define TESTREPRESENTATION1_H_

#include "Template.h"

REPRESENTATION(TestRepresentation1)
class TestRepresentation1: public TestRepresentation1Base
{
public:
  bool blinkfast, blinkslow;
  TestRepresentation1() :
    blinkfast(false), blinkslow(false)
  {
  }
};

#endif /* TESTREPRESENTATION1_H_ */
