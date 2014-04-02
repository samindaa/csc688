/*
 * SendModule.h
 *
 *  Created on: Mar 22, 2014
 *      Author: sam
 */

#ifndef SENDMODULE_H_
#define SENDMODULE_H_

#include "Template.h"
#include "FloatDetails.h"

// Sending representations
#include "TestRepresentation2.h"
#include "MPU9150Representation.h"
#include "PredictionRepresentation.h"

MODULE(SendModule)
  REQUIRES(TestRepresentation2) //
  REQUIRES(MPU9150Representation) //
  REQUIRES(PredictionRepresentation)
END_MODULE
class SendModule: public SendModuleBase
{
  public:
    void execute();
  private:
    void floatToBytes(const float& value);
};

#endif /* SENDMODULE_H_ */
