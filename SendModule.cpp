/*
 * SendModule.cpp
 *
 *  Created on: Mar 22, 2014
 *      Author: sam
 */

#include "SendModule.h"

void SendModule::execute()
{
#if defined(ENERGIA)
  if (theTestRepresentation2->collectData)
  {
    Serial.print("{"); //<< SOF
    // Write the
    /*
     floatToBytes(theMPU9150Representation->fAccelX);
     floatToBytes(theMPU9150Representation->fAccelY);
     floatToBytes(theMPU9150Representation->fAccelZ);
     floatToBytes(theMPU9150Representation->fGyroX);
     floatToBytes(theMPU9150Representation->fGyroY);
     floatToBytes(theMPU9150Representation->fGyroZ);
     floatToBytes(theMPU9150Representation->fMagnetoX);
     floatToBytes(theMPU9150Representation->fMagnetoY);
     floatToBytes(theMPU9150Representation->fMagnetoZ);
     floatToBytes(theMPU9150Representation->fRoll);
     floatToBytes(theMPU9150Representation->fPitch);
     floatToBytes(theMPU9150Representation->fYaw);
     */
    floatToBytes(thePredictionRepresentation->target);
    floatToBytes(thePredictionRepresentation->prediction);
    Serial.println("}");    //<< EOF
  }
#endif
}

void SendModule::floatToBytes(const float& value)
{
#if defined(ENERGIA)
  FloatDetails floatDetails(value);
  const int32_t& mantissa = floatDetails.getMantissa();
  const int16_t& exponent = floatDetails.getExponent();
  Serial.print((int32_t) mantissa);
  Serial.print(" ");
  Serial.print((int32_t) exponent);
  Serial.print(" ");
#endif
}

MAKE_MODULE(SendModule)

