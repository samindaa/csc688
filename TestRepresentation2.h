/*
 * TestRepresentation2.h
 *
 *  Created on: Feb 19, 2014
 *      Author: sam
 */

#ifndef TESTREPRESENTATION2_H_
#define TESTREPRESENTATION2_H_

#include "Template.h"

REPRESENTATION(TestRepresentation2)
class TestRepresentation2: public TestRepresentation2Base
{
  public:
    bool collectData;
    TestRepresentation2() :
      collectData(0)
    {
    }
};

#endif /* TESTREPRESENTATION2_H_ */
