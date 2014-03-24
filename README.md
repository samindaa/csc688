# CSC688

This is an open source project that provides a machine learning environment using:

1. [Tiva C Series TM4C123G LaunchPad microcontroller](http://www.ti.com/tool/ek-tm4c123gxl), is a low-cost evaluation platform for ARM Cortex-M4F-based microcontrollers from Texas Instruments;

2. [Sensor Hub BoosterPack](http://www.ti.com/tool/boostxl-senshub), is an add-on board designed to fit the Tiva C Series TM4C123G LaunchPad along with all of TI’s MCU LaunchPads. 

3. [Energia](http://energia.nu/), is an open-source electronics prototyping platform; and

4. [RLLib](http://web.cs.miami.edu/home/saminda/rllib.html), is a C++ Template Library to Learn Behaviors and Represent Learnable Knowledge using On/Off Policy Reinforcement Learning.


## Usage 

The project consists of three parts. It consists of modules and representations that runs on:

1. Microcontrollers;

2. Offline; 

3. [NAO](http://www.aldebaran.com/) robots; and

4. Visualizations. 

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

![](http://web.cs.miami.edu/home/saminda/tmp/graph_structure.png)

## Contact

Saminda Abeyruwan (saminda@cs.miami.edu)

## License

Apache License, Version 2.0
