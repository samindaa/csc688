#include <Wire.h>
#include "Framework.h"

void setup()
{
  // Reference to the graph.
  Graph& graph = Graph::getInstance();
  // Default baud rate is set to 115200.
  // If this value needs to be changed, uncomment following line:
  // graph.setBaudRate(9600);
  // Create the graph using modules and representations.
  graph.computeGraph();
  // Topologically sort the graph.
  graph.topoSort();
  // Intialize the modules once. 
  graph.graphOutputInit();
  // If there are run-time errors in the graph, such as loop etc,
  // an error will be written to the serial port, while RED LED
  // blinking periodically. 
}

void loop()
{
  // unsigned long startTime = millis(); // DEBUG
  // Execute the duty-cycle as fast as possible. 
  Graph::getInstance().graphOutputUpdate();
  // unsigned long elapsedTime = millis() - startTime; // DEBUG
  //DEBUG
  //Serial.print("ElapsedTime=");
  //Serial.println(elapsedTime);

  //delay(2000); //<< SLOW DOWN to DEBUG 
}
