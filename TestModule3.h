/*
 * TestModule3.h
 *
 *  Created on: Feb 18, 2014
 *      Author: sam
 */

#ifndef TESTMODULE3_H_
#define TESTMODULE3_H_

#include "Template.h"
#include "TestRepresentation2.h"

MODULE(TestModule3)
PROVIDES(TestRepresentation2)
END_MODULE

class TestModule3: public TestModule3Base
{
    // http://forum.43oh.com/topic/3075-reading-lm4f120-internal-temp-sensor/
  private:
    uint32_t ulADC0Value[4];
    volatile uint32_t ulTempAvg;
    volatile uint32_t ulTempValueC;
    volatile uint32_t ulTempValueF;

  public:
    void init();
    void execute();

    void update(TestRepresentation2& theTestRepresentation2);
};

#endif /* TESTMODULE3_H_ */
