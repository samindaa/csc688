/*
 * MPU9150Module.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef MPU9150MODULE_H_
#define MPU9150MODULE_H_

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "MPU9150Representation.h"

MODULE(MPU9150Module)
REQUIRES(LaunchPadRepresentation)
PROVIDES(MPU9150Representation)
END_MODULE
class MPU9150Module: public MPU9150ModuleBase
{
  public:
    void update(MPU9150Representation& theMPU9150Representation);
};

#endif /* MPU9150MODULE_H_ */
