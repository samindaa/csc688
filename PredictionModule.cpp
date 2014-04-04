/*
 * PredictionModule.cpp
 *
 *  Created on: Mar 20, 2014
 *      Author: sam
 */

#include "PredictionModule.h"

PredictionModule::PredictionModule() :
    nbTrainingSample(0), nbMaxTrainingSamples(0), x(0), predictor(0), epsilon(0), problem(0), hashing(
        0), projector(0), toStateAction(0), e(0), alpha(0), gamma(0), lambda(0), sarsa(0), acting(
        0), control(0), agent(0), sim(0)
{

}

PredictionModule::~PredictionModule()
{
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
  //predictor->initialize();
  //control->persist("foo.dat");
  RLLib::Probabilistic<float>::srand(0);

  nbTrainingSample = 0;
  nbMaxTrainingSamples = 100000;
  x = new RLLib::PVector<float>(3);
  x->setEntry(0, 1); // bias
  predictor = new RLLib::SemiLinearIDBD<float>(x->dimension(), 1e-4 / x->dimension());

  // NOT USE NOW
  /*
   hashing = new RLLib::MurmurHashing(32);
   problem = new RLLib::RLProblem<float>(2, 1, 1); // Dummy
   projector = new RLLib::TileCoderHashing<float>(hashing, problem->dimension(), 10, 10, true);
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
   */
}

void PredictionModule::execute()
{
  //sim->step();
  if (nbTrainingSample < nbMaxTrainingSamples)
  {
    float x0 = RLLib::Probabilistic<float>::nextGaussian(0.25, 0.2);
    x->setEntry(1, x0); // @@>> input noise?
    x->setEntry(2, x0 * x0);
    float y0 = 0; // @@>> output noise?
    predictor->learn(x, y0);

    float x1 = RLLib::Probabilistic<float>::nextGaussian(0.75, 0.2);
    x->setEntry(1, x1); // @@>> input noise?
    x->setEntry(2, x1 * x1);
    float y1 = 1; // @@>> output noise?
    predictor->learn(x, y1);
    ++nbTrainingSample;
  }
}

void PredictionModule::update(PredictionRepresentation& thePredictionRepresentation)
{
  if (RLLib::Probabilistic<float>::nextReal() > 0.5)
  {
    float x0 = RLLib::Probabilistic<float>::nextGaussian(0.25, 0.2);
    x->setEntry(1, x0); // @@>> input noise?
    x->setEntry(2, x0 * x0);
    thePredictionRepresentation.target = 0;
    thePredictionRepresentation.prediction = predictor->predict(x);
  }
  else
  {
    float x1 = RLLib::Probabilistic<float>::nextGaussian(0.75, 0.2);
    x->setEntry(1, x1); // @@>> input noise?
    x->setEntry(2, x1 * x1);
    thePredictionRepresentation.target = 1;
    thePredictionRepresentation.prediction = predictor->predict(x);
  }
}

MAKE_MODULE(PredictionModule)

