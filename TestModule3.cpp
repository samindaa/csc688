/*
 * TestModule3.cpp
 *
 *  Created on: Feb 18, 2014
 *      Author: sam
 */

#include "TestModule3.h"

#ifndef OFFLINE
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#endif

void TestModule3::init()
{
#ifndef OFFLINE
  Serial.begin(9600);
  pinMode(BLUE_LED, OUTPUT);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
  SysCtlADCSpeedSet(SYSCTL_ADCSPEED_125KSPS); // 250
  ADCSequenceDisable(ADC0_BASE, 1);
  ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 1, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 2, ADC_CTL_TS);
  ADCSequenceStepConfigure(ADC0_BASE, 1, 3, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
  ADCSequenceEnable(ADC0_BASE, 1);
#endif
}

void TestModule3::execute()
{
#ifndef OFFLINE
  ADCIntClear(ADC0_BASE, 1);
  ADCProcessorTrigger(ADC0_BASE, 1);

  while (!ADCIntStatus(ADC0_BASE, 1, false))
  {
    digitalWrite(BLUE_LED, HIGH);
  }

  ADCSequenceDataGet(ADC0_BASE, 1, ulADC0Value);
  ulTempAvg = (ulADC0Value[0] + ulADC0Value[1] + ulADC0Value[2] + ulADC0Value[3] + 2) / 4;
  ulTempValueC = (1475 - ((2475 * ulTempAvg)) / 4096) / 10;
  ulTempValueF = ((ulTempValueC * 9) + 160) / 5;

  if (Serial.available())
    Serial.println(ulTempValueC);
#endif
}

void TestModule3::update(TestRepresentation2& theTestRepresentation2)
{
  theTestRepresentation2.ulTempValueC = ulTempValueC;
}

MAKE_MODULE(TestModule3)
