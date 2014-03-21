/*
 * PredictionModule.h
 *
 *  Created on: Mar 20, 2014
 *      Author: sam
 */

#ifndef PREDICTIONMODULE_H_
#define PREDICTIONMODULE_H_

#include "Template.h"
#include "ISL29023Representation.h"
#include "SupervisedAlgorithm.h"

MODULE(PredictionModule)
END_MODULE
class PredictionModule: public PredictionModuleBase
{
  private:
    RLLib::SemiLinearIDBD<float>* predictor;
  public:
    PredictionModule();
    ~PredictionModule();
    void init();
    void execute();
};

#endif /* PREDICTIONMODULE_H_ */
