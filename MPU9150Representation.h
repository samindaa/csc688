/*
 * MPU9150Representation.h
 *
 *  Created on: Feb 21, 2014
 *      Author: Saminda
 */

#ifndef MPU9150REPRESENTATION_H_
#define MPU9150REPRESENTATION_H_

#include "Template.h"

REPRESENTATION(MPU9150Representation)
class MPU9150Representation: public MPU9150RepresentationBase
{
  public:
    float fAccelX;
    float fAccelY;
    float fAccelZ;
    float fGyroX;
    float fGyroY;
    float fGyroZ;
    float fMagnetoX;
    float fMagnetoY;
    float fMagnetoZ;
    float fRoll;
    float fPitch;
    float fYaw;

    MPU9150Representation() :
        fAccelX(0), fAccelY(0), fAccelZ(0), fGyroX(0), fGyroY(0), fGyroZ(0), fMagnetoX(0), fMagnetoY(
            0), fMagnetoZ(0), fRoll(0), fPitch(0), fYaw(0)
    {
    }
};

#endif /* MPU9150REPRESENTATION_H_ */
