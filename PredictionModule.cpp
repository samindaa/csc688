/*
 * PredictionModule.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: sam
 */

#include "PredictionModule.h"

PredictionModule::PredictionModule() :
    predictor(0)
{
}

PredictionModule::~PredictionModule()
{
  if (predictor)
    delete predictor;
}

void PredictionModule::init()
{
  predictor = new RLLib::SemiLinearIDBD<float>(1, 0.001);
}

void PredictionModule::execute()
{
}

MAKE_MODULE(PredictionModule)

