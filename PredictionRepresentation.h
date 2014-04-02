/*
 * PredictionRepresentation.h
 *
 *  Created on: Apr 2, 2014
 *      Author: sam
 */

#ifndef PREDICTIONREPRESENTATION_H_
#define PREDICTIONREPRESENTATION_H_

#include "Template.h"

REPRESENTATION(PredictionRepresentation)
class PredictionRepresentation: public PredictionRepresentationBase
{
  public:
    float target;
    float prediction;
    PredictionRepresentation() :
        target(0), prediction(0)
    {
    }
};

#endif /* PREDICTIONREPRESENTATION_H_ */
