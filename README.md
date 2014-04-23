# CSC688 Energia
### A Software Development Platform for MSP-EXP430G2 LaunchPad, Tiva C Series EK-TM4C123GXL LaunchPad, and Tiva C Series TM4C129 Connected LaunchPad

This is an open source project that provides a software development and a machine learning environment using:

1. [Tiva C Series TM4C123G LaunchPad microcontroller](http://www.ti.com/tool/ek-tm4c123gxl), is a low-cost evaluation platform for ARM Cortex-M4F-based microcontrollers from Texas Instruments;

2. [Tiva C Series TM4C129 Connected LaunchPad](http://embeddedcomputing.weebly.com/connected-launchpad-tiva-c-series-tm4c129.html),  is a new development platform from Texas Instruments based on the powerful TM4C129;

3. [Sensor Hub BoosterPack](http://www.ti.com/tool/boostxl-senshub), is an add-on board designed to fit the Tiva C Series TM4C123G LaunchPad along with all of TI’s MCU LaunchPads; 

4. [Energia](http://energia.nu/), is an open-source electronics prototyping platform; and

5. [RLLib](http://web.cs.miami.edu/home/saminda/rllib.html), is a C++ Template Library to Predict, Control, Learn Behaviors, and Represent Learnable Knowledge using On/Off Policy Reinforcement Learning, and Supervised Learning. 


## Usage 

The project consists of four parts. It consists of modules and representations that runs on:

1. Microcontrollers (incremental);

2. Offline (batch mode); 

3. [NAO](http://www.aldebaran.com/) robots; and

4. Visualizations. 

## Software Architecture

The software development framework uses a notion of `modules` and `representations` to perform computations. The modules implements functions, while representations exchange information from one module to another. The following figure shows an example of modules are representations currently available in the distribution. 

![](http://web.cs.miami.edu/home/saminda/tmp/csc688_energia.png)

The green boxes represent modules, while the yellow ellipses represent the representations. As an example, module `MPU9150Module` contains logic to read/write  from MPU-9150 Nine-Axis (Gyro + Accelerometer + Compass) MEMS MotionTracking™ device on the sensor hub booster pack. `MPU9150Representation` representation contains all the values that module MPU9150Module would like to share with other modules. In this graph TestModule4 requests values from MPU9150Representation to implement its logic. A module can provide multiple representations as shown in the module `TestModule2`. The yellow arrows shows the provided representations, the black arrows show the requested representations, and the red arrows show the used representations.  

There are two graphs in the figure; the "offline graph" will be executed offline, while the rest of the graph, "online graph", will be executed on the devices. We have shown only two graphs in this figure; in reality, one can keep up to N number of graphs. The uses of the software only requires to write the modules and representations, while the framework will compute the topologically sorted graph out of the nodes. This will be computed once online/offline, and the nodes in the queue will be executed one after the other. If there were to be cycles in the graph, the framework will detect them and indicate them to the users.  

The module `PredictionModule` uses RLLib to learn on-line about a supervised learning problem. It provides the representation `PredictionRepresentation`. We have the `SendModule` to send values of the represenations to an offline module. This is useful during the debug phases of the project. The module `RS232Module` reads the data values that is send from SendModule. 

To write modules and representations, one needs the following (this example shows the implementation of  TestModule1 and TestRepresentation1 in the figure):


```cpp

#pragma once

#include "Template.h"
#include "LaunchPadRepresentation.h"
#include "TestRepresentation1.h"

MODULE(TestModule1)
  REQUIRES(LaunchPadRepresentation) //
  PROVIDES(TestRepresentation1) //
END_MODULE

class TestModule1: public TestModule1Base
{
  public:
    void update(TestRepresentation1& theTestRepresentation1);
};

```

```cpp

#include "TestModule1.h"

void TestModule1::update(TestRepresentation1& theTestRepresentation1)
{
  theTestRepresentation1.leftButton = theTestRepresentation1.rightButton = false;
#if defined(ENERGIA)
  if (digitalRead(PUSH1) == LOW)
    theTestRepresentation1.leftButton = true;
  if (digitalRead(PUSH2) == LOW)
    theTestRepresentation1.rightButton = true;
#endif
}

MAKE_MODULE(TestModule1)


```

```cpp

#pragma once

#include "Template.h"

REPRESENTATION(TestRepresentation1)
class TestRepresentation1: public TestRepresentation1Base
{
public:
  bool rightButton, leftButton;
  TestRepresentation1() :
    rightButton(false), leftButton(false)
  {
  }
};


```

You need to include the header file `Template.h`  to access the framework functionality. 


## Compilation

In order to compile the project:

1. Microcontrollers:
    1. Connect the microcontroller.
    2. Download the latest [Energia distribution](http://energia.nu/download/) that matches to the host machine. 
    3. Open _csc688.ino_ file.
    4. Compile and upload the csc688.elf file to the microcontroller.

2. Offline:
    1. Run *configure_offline* script to generate make file. 
    2.  *make*
    3. *./build/offline_csc688*

3. NAO robos (TODO):

4. Visualizations:
    1. Visualizations use QT4 library. Download the library to your host system.
    2. Change directory to *viz*.
    3. *qmake viz.pro*
    4. *make*
    5. Change directory to *build*.
    4. Execute the binary *viz*.

## Platform

Following figure shows an example of modules and representations available as at (03/24/2014):



## Contact

Saminda Abeyruwan (saminda@cs.miami.edu)

## License

Apache License, Version 2.0
