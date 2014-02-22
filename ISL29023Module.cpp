/*
 * ISL29023Module.cpp
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#include "ISL29023Module.h"

ISL29023Module::ISL29023Module() :
    ui8Range(ISL29023_CMD_II_RANGE_1K >> ISL29023_CMD_II_RANGE_S), ui8Resolution(
        ISL29023_CMD_II_RANGE_1K >> ISL29023_CMD_II_RANGE_S)
{
}

void ISL29023Module::update(ISL29023Representation& theISL29023Representation)
{
  // TODO:
}

MAKE_MODULE(ISL29023Module)

