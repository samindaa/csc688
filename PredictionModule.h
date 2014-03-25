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
#include "ControlAlgorithm.h"
#include "RL.h"
#include "Projector.h"

MODULE(PredictionModule)
END_MODULE
class PredictionModule: public PredictionModuleBase
{
  private:
    // Supervised Learning
    RLLib::SemiLinearIDBD<float>* predictor;

    // Reinforcement Learning
    float epsilon;
    RLLib::RLProblem<float>* problem;
    RLLib::Hashing* hashing;
    RLLib::Projector<float>* projector;
    RLLib::StateToStateAction<float>* toStateAction;
    RLLib::Trace<float>* e;
    float alpha;
    float gamma;
    float lambda;
    RLLib::Sarsa<float>* sarsa;
    RLLib::Policy<float>* acting;
    RLLib::OnPolicyControlLearner<float>* control;

    RLLib::RLAgent<float>* agent;
    RLLib::Simulator<float>* sim;

  public:
    PredictionModule();
    ~PredictionModule();
    void init();
    void execute();
};

#endif /* PREDICTIONMODULE_H_ */
