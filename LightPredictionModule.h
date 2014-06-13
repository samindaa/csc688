/*
 * LightPredictionModule.h
 *
 *  Created on: Jun 12, 2014
 *      Author: sam
 */

#ifndef LIGHTPREDICTIONMODULE_H_
#define LIGHTPREDICTIONMODULE_H_

#include "Template.h"

#include "MPU9150Representation.h"
#include "ISL29023Representation.h"
#include "PredictionRepresentation.h"
#include "SupervisedAlgorithm.h"
#include "Projector.h"
#include "Math.h"
#include <cmath>

#if defined(ENERGIA)
#include "Energia.h"
#endif

MODULE(LightPredictionModule)
  REQUIRES(MPU9150Representation) //
  REQUIRES(ISL29023Representation) //
  PROVIDES(PredictionRepresentation) //
END_MODULE
class LightPredictionModule: public LightPredictionModuleBase
{
  private:
    float gridResolution;
    int nbTilings;
    int nbInputs;
    int memorySize;
    int nbTrainingSample;
    int nbMaxTrainingSamples;
    //
    RLLib::Random<float>* random;
    RLLib::Vector<float>* input;
    RLLib::Range<float>* inputRange;
    RLLib::Hashing<float>* hashing;
    RLLib::Projector<float>* projector;
    RLLib::Autostep<float>* predictor;

  public:
    LightPredictionModule();
    ~LightPredictionModule();

    void update(PredictionRepresentation& thePredictionRepresentation);

};

#endif /* LIGHTPREDICTIONMODULE_H_ */
