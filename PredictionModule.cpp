/*
 * PredictionModule.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: sam
 */

#include "PredictionModule.h"

PredictionModule::PredictionModule()
{
  RLLib::Probabilistic<float>::srand(0);
  predictor = new RLLib::SemiLinearIDBD<float>(1, 0.001);
  problem = new RLLib::RLProblem<float>(2, 1, 1); // Dummy
  hashing = new RLLib::MurmurHashing(32);
  projector = new RLLib::TileCoderHashing<float>(hashing, problem->dimension(), 10, 10,
  true);
  toStateAction = new RLLib::StateActionTilings<float>(projector, problem->getDiscreteActions());
  e = new RLLib::RTrace<float>(projector->dimension());
  alpha = 0.15 / projector->vectorNorm();
  gamma = 0.99;
  lambda = 0.3;
  epsilon = 0.01;
  sarsa = new RLLib::Sarsa<float>(alpha, gamma, lambda, e);
  acting = new RLLib::EpsilonGreedy<float>(sarsa, problem->getDiscreteActions(), epsilon);
  
  control = new RLLib::SarsaControl<float>(acting, toStateAction, sarsa);
  agent = new RLLib::LearnerAgent<float>(control);
  sim = new RLLib::Simulator<float>(agent, problem, 5000, 300, 1);
}

PredictionModule::~PredictionModule()
{
  delete predictor;
  delete problem;
  delete hashing;
  delete projector;
  delete toStateAction;
  delete e;
  delete sarsa;
  delete acting;
  delete control;
  delete agent;
  delete sim;
}

void PredictionModule::init()
{
  //predictor->initialize();
  //control->persist("foo.dat");
}

void PredictionModule::execute()
{
  //predictor->learn(0, 1.0f);
  //sim->step();
}

MAKE_MODULE(PredictionModule)

