/*
 * LightPredictionModule.cpp
 *
 *  Created on: Jun 12, 2014
 *      Author: sam
 */

#include "LightPredictionModule.h"

MAKE_MODULE(LightPredictionModule)

LightPredictionModule::LightPredictionModule() :
    gridResolution(8), nbTilings(4), nbInputs(3), memorySize(64), nbTrainingSample(0), nbMaxTrainingSamples(
        10000)
{
  random = new RLLib::Random<float>;
  input = new RLLib::PVector<float>(nbInputs);
  inputRange = new RLLib::Range<float>(-1.0f, 1.0f);
  hashing = new RLLib::MurmurHashing<float>(random, memorySize);
  projector = new RLLib::TileCoderHashing<float>(hashing, nbInputs, gridResolution, nbTilings);
  predictor = new RLLib::Autostep<float>(projector->dimension());
}

LightPredictionModule::~LightPredictionModule()
{
  delete random;
  delete input;
  delete inputRange;
  delete hashing;
  delete projector;
  delete predictor;
}

void LightPredictionModule::update(PredictionRepresentation& thePredictionRepresentation)
{
#if defined(ENERGIA)
  thePredictionRepresentation.target = theISL29023Representation->fAmbient;

  input->setEntry(0, inputRange->toUnit(cos(theMPU9150Representation->fRoll)));
  input->setEntry(1, inputRange->toUnit(cos(theMPU9150Representation->fPitch)));
  input->setEntry(2, inputRange->toUnit(cos(theMPU9150Representation->fYaw)));

  const RLLib::Vector<float>* featureVector = projector->project(input);
  if (nbTrainingSample++ < nbMaxTrainingSamples)
    predictor->learn(featureVector, thePredictionRepresentation.target);
  thePredictionRepresentation.prediction = predictor->predict(featureVector);
#endif
}
