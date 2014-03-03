/*
 * NaoModule.h
 *
 *  Created on: Feb 28, 2014
 *      Author: sam
 */

#ifndef NAOMODULE_H_
#define NAOMODULE_H_

#include "Template.h"
#include "MPU9150Representation.h"

#define STREAM_SRT \
  Serial.print("( ");
#define STREAM_END \
  Serial.print(")");

#if defined(ENERGIA)
#define STREAM(data) \
  Serial.print(data, 2); \
  Serial.print(" ");
#else
#define STREAM(data)
#endif

MODULE(NaoModule)
REQUIRES(MPU9150Representation)
END_MODULE
class NaoModule: public NaoModuleBase
{
  public:
    void execute();
};

#endif /* NAOMODULE_H_ */
