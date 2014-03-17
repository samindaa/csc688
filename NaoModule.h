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

/**
 * NaoModule stream the data to NAO robot. This streaming can be an raw values or computed
 * value from uC.
 *
 * bytes: a b c d | x y z 1 2 3 | ...
 *
 * A section of data is encapsulated between "|". Each byte is separated by space.
 */

#if defined(ENERGIA)
#define STREAM_DELIMITER \
  Serial.println("|");

#define STREAM(data) \
  Serial.print(data, 2); \
  Serial.print(" ");
#else
#define STREAM_DELIMITER
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
