/*
 * PredictionModule.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: sam
 */

#include "PredictionModule.h"

//MAKE_MODULE(PredictionModule)

PredictionModule::PredictionModule() :
    nbTrainingSample(0), nbMaxTrainingSamples(0), gridResolution(0), nbTilings(0), random(0), x(0), predictor(
        0), epsilon(0), problem(0), hashing(0), projector(0), toStateAction(0), e(0), alpha(0), gamma(
        0), lambda(0), sarsa(0), acting(0), control(0), agent(0), sim(0)
{

}

PredictionModule::~PredictionModule()
{
  if (random)
    delete random;
  if (x)
    delete x;
  if (predictor)
    delete predictor;
  if (problem)
    delete problem;
  if (hashing)
    delete hashing;
  if (projector)
    delete projector;
  if (toStateAction)
    delete toStateAction;
  if (e)
    delete e;
  if (sarsa)
    delete sarsa;
  if (acting)
    delete acting;
  if (control)
    delete control;
  if (agent)
    delete agent;
  if (sim)
    delete sim;
}

void PredictionModule::init()
{
  //
  nbTrainingSample = 0;
  nbMaxTrainingSamples = 1000;
  gridResolution = 10;
  nbTilings = 4;
  //
  random = new RLLib::Random<float>;
  hashing = new RLLib::MurmurHashing<float>(random, 32);
  problem = new RLLib::RLProblem<float>(random, 1, 1, 1); // Dummy
  projector = new RLLib::TileCoderHashing<float>(hashing, problem->dimension(), gridResolution, 4,
      true);
  //
  x = new RLLib::PVector<float>(problem->dimension());
  predictor = new RLLib::SemiLinearIDBD<float>(projector->dimension(), 0.01f);
  //
  toStateAction = new RLLib::StateActionTilings<float>(projector, problem->getDiscreteActions());
  e = new RLLib::RTrace<float>(projector->dimension());
  //
  alpha = 0.001f / projector->vectorNorm();
  gamma = 0.10f;
  lambda = 0.3f;
  epsilon = 0.01f;
  //
  sarsa = new RLLib::Sarsa<float>(alpha, gamma, lambda, e);
  acting = new RLLib::EpsilonGreedy<float>(random, problem->getDiscreteActions(), sarsa, epsilon);
  //
  control = new RLLib::SarsaControl<float>(acting, toStateAction, sarsa);
  agent = new RLLib::LearnerAgent<float>(control);
  sim = new RLLib::Simulator<float>(agent, problem, 1000, 1, 1);

}

void PredictionModule::execute()
{
  if (nbTrainingSample < nbMaxTrainingSamples)
  {
    float rnd = random->nextReal() - 1.0f;
    x->setEntry(0, (rnd + 1.0f) / 2.0f);
    predictor->learn(projector->project(x), 0.0f);

    rnd = random->nextReal();
    x->setEntry(0, (rnd + 1.0f) / 2.0f);
    predictor->learn(projector->project(x), 1.0f);

    ++nbTrainingSample;
  }
}

void PredictionModule::update(PredictionRepresentation& thePredictionRepresentation)
{
  if (random->nextReal() > 0.5f)
  {
    float rnd = random->nextReal() - 1.0f;
    x->setEntry(0, (rnd + 1.0f) / 2.0f);
    thePredictionRepresentation.target = 0.0f;
    thePredictionRepresentation.prediction = predictor->predict(projector->project(x));
  }
  else
  {
    float rnd = random->nextReal();
    x->setEntry(0, (rnd + 1.0f) / 2.0f);
    thePredictionRepresentation.target = 1.0f;
    thePredictionRepresentation.prediction = predictor->predict(projector->project(x));
  }
}
