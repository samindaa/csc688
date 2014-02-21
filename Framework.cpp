/*
 * Graph.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#include "Framework.h"

#if !defined(ENERGIA)
#include <iostream>
#include <cstdlib>
#include <fstream>
#endif

#include <string.h>

Graph* Graph::theInstance = 0;

Graph::Graph() :
    errorValue(0)
{
}

Graph::~Graph()
{
  for (int iter = 0; iter < moduleVector.size(); iter++)
    delete moduleVector[iter];
  for (int iter = 0; iter < representationVector.size(); iter++)
    delete representationVector[iter];
  for (int iter = 0; iter < moduleRepresentationRequiredVector.size(); iter++)
    delete moduleRepresentationRequiredVector[iter];
  for (int iter = 0; iter < moduleRepresentationUsedVector.size(); iter++)
    delete moduleRepresentationUsedVector[iter];
  for (int iter = 0; iter < graphOutput.size(); iter++)
    delete graphOutput[iter];
}

void Graph::addModule(Node* theInstance)
{
  // Check if a module type exits
  for (int iter = 0; iter < moduleVector.size(); iter++)
  {
    if (strcmp(moduleVector[iter]->moduleNode->getName(), theInstance->getName()) == 0)
    {
#if !defined(ENERGIA)
      std::cout << "ERROR! moduleByName=" << theInstance->getName() << " exists!" << std::endl;
      exit(1);
#else
      errorValue = 1;
#endif
    }
  }

  theInstance->setIndex(inDegreesVector.size());
  theInstance->setComputationNode(true);
  Graph::ModuleEntry* newModuleEntry = new Graph::ModuleEntry(theInstance);
  moduleVector.push_back(newModuleEntry);
  inDegreesVector.push_back(0);
}

void Graph::providedRepresentation(const char* moduleName, Node* theInstance,
    void (*updateRepresentation)(Node*, Node*))
{
  // Check if a representation type exits
  for (int iter = 0; iter < representationVector.size(); iter++)
  {
    if (strcmp(representationVector[iter]->representationNode->getName(), theInstance->getName())
        == 0)
    {
#if !defined(ENERGIA)
      std::cout << "ERROR! representationByName=" << theInstance->getName() << " exists, and "
          << "providedModuleName=" << representationVector[iter]->providedModuleName << std::endl;
      exit(1);
#else
      errorValue = 2;
#endif
    }
  }
  theInstance->setIndex(inDegreesVector.size());
  Graph::RepresentationEntry* newRepresentationEntry = new Graph::RepresentationEntry(moduleName,
      theInstance, updateRepresentation);
  representationVector.push_back(newRepresentationEntry);
  inDegreesVector.push_back(0);
}

void Graph::requiredRepresentation(const char* moduleName, const char* representationName)
{
  Graph::ModuleRepresentationEntry* newModuleRepresentationEntry =
      new Graph::ModuleRepresentationEntry(moduleName, representationName);
  moduleRepresentationRequiredVector.push_back(newModuleRepresentationEntry);
}

void Graph::usedRepresentation(const char* moduleName, const char* representationName)
{
  Graph::ModuleRepresentationEntry* newModuleRepresentationEntry =
      new Graph::ModuleRepresentationEntry(moduleName, representationName);
  moduleRepresentationUsedVector.push_back(newModuleRepresentationEntry);
}

Node* Graph::getRepresentation(const char* representationName)
{
  for (int iter = 0; iter < representationVector.size(); iter++)
  {
    Graph::RepresentationEntry* representationEntry = representationVector[iter];
    if (strcmp(representationEntry->representationNode->getName(), representationName) == 0)
    {
      if (!representationEntry->representationNode->isInitialized())
      {
#if !defined(ENERGIA)
        std::cerr << " ERROR! " << std::endl;
        exit(1);
#else
        errorValue = 3;
#endif
      }
      return representationEntry->representationNode;
    }
  }
#if !defined(ENERGIA)
  // This is a double check and nothing should enter at this point
  std::cerr << " ERROR! " << std::endl;
  exit(1);
#else
  errorValue = 4;
#endif
  return 0;
}

void Graph::computeGraph()
{
  // 0) Loading errors
  errorHandler();
  // 1) Add modules
  for (int iter = 0; iter < moduleVector.size(); iter++)
  {
    Graph::ModuleEntry* moduleEntry = moduleVector[iter];
    graphStructureVector.push_back(moduleEntry->moduleNode);
  }

  // 2) Provides representations
  for (int iter = 0; iter < representationVector.size(); iter++)
  {
    Graph::RepresentationEntry* representationEntry = representationVector[iter];
    graphStructureVector.push_back(representationEntry->representationNode);

    for (int iter2 = 0; iter2 < moduleVector.size(); iter2++)
    {
      Graph::ModuleEntry* moduleEntry = moduleVector[iter2];
      if (strcmp(moduleEntry->moduleNode->getName(), representationEntry->providedModuleName) == 0)
      {
        representationEntry->representationNode->setPreviousNode(moduleEntry->moduleNode);
        ((Representation*) representationEntry->representationNode)->updateThis =
            representationEntry->update;
        moduleEntry->moduleNode->addNextNode(representationEntry->representationNode);
      }
    }
  }

  // 3) Requires representations
  for (int iter = 0; iter < moduleRepresentationRequiredVector.size(); iter++)
  {
    Graph::ModuleRepresentationEntry* moduleRepresentationEntry =
        moduleRepresentationRequiredVector[iter];

    Node *moduleNode = 0, *representationNode = 0;
    for (int iter2 = 0; iter2 < moduleVector.size(); iter2++)
    {
      Graph::ModuleEntry* moduleEntry = moduleVector[iter2];
      if (strcmp(moduleEntry->moduleNode->getName(), moduleRepresentationEntry->requiredModuleName)
          == 0)
        moduleNode = moduleEntry->moduleNode;
    }

    for (int iter2 = 0; iter2 < representationVector.size(); iter2++)
    {
      Graph::RepresentationEntry* representationEntry = representationVector[iter2];
      if (strcmp(representationEntry->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = representationEntry->representationNode;
    }

    if (moduleNode == 0)
    {
#if !defined(ENERGIA)
      std::cout << "requiredModuleName=" << moduleRepresentationEntry->requiredModuleName
          << " is missing!" << std::endl;
#else
      errorValue = 5;
#endif
    }
    if (representationNode == 0)
    {
#if !defined(ENERGIA)
      std::cout << "requiredRepresentationName="
          << moduleRepresentationEntry->requiredRepresentationName << " is missing!" << std::endl;
#endif
    }
    if (!(moduleNode && representationNode))
    {
#if !defined(ENERGIA)
      std::cerr << "ERROR " << std::endl;
      exit(1);
#else
      errorValue = 6;
#endif

    }
    errorHandler();
    representationNode->addNextNode(moduleNode);
  }

  // 4) Uses representation
  for (int iter = 0; iter < moduleRepresentationUsedVector.size(); iter++)
  {
    Graph::ModuleRepresentationEntry* moduleRepresentationEntry =
        moduleRepresentationUsedVector[iter];

    Node *moduleNode = 0, *representationNode = 0;
    for (int iter2 = 0; iter2 < moduleVector.size(); iter2++)
    {
      Graph::ModuleEntry* moduleEntry = moduleVector[iter2];
      if (strcmp(moduleEntry->moduleNode->getName(), moduleRepresentationEntry->requiredModuleName)
          == 0)
        moduleNode = moduleEntry->moduleNode;
    }

    for (int iter2 = 0; iter2 < representationVector.size(); iter2++)
    {
      Graph::RepresentationEntry* representationEntry = representationVector[iter2];
      if (strcmp(representationEntry->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = representationEntry->representationNode;
    }

    if (!(moduleNode && representationNode))
    {
#if !defined(ENERGIA)
      std::cerr << "ERROR!" << std::endl;
      exit(1);
#else
      errorValue = 7;
#endif
    }
    errorHandler();
    representationNode->addAuxiliaryNode(moduleNode);
  }

}

void Graph::topoSort()
{
  // Calculate in-degrees
  for (int i = 0; i < graphStructureVector.size(); i++)
  {
    Node* x = graphStructureVector[i];
    for (int j = 0; j < x->getNextNodes().size(); j++)
      ++inDegreesVector[x->getNextNodes()[j]->getIndex()];
  }

  // Initialize the loop
  for (int i = 0; i < inDegreesVector.size(); i++)
  {
    if (inDegreesVector[i] == 0)
    {
      for (int j = 0; j < graphStructureVector.size(); ++j)
      {
        Node* x = graphStructureVector[j];
        if (x->getIndex() == i)
          topoQueue.push_back(x);
      }
    }
  }

  // Main loop
  while (!topoQueue.empty())
  {
    Node* x = topoQueue.front();
    //topoQueue.erase(topoQueue.begin());
    topoQueue.erase(0);

    TopoNode* topoNode = 0;
    if (x->isComputationNode())
      topoNode = new TopoModule((Module*) x);
    else
      topoNode = new TopoRepresentation(((Module*) (x->getPreviousNode())),
          (Representation*) x);

    if (x->isInitialized())
    {
      graphOutput.push_back(topoNode);
#if !defined(ENERGIA)
      std::cout << "ERROR! Cycle detected!" << std::endl;
      int tabCounter = 0;
      for (int j = 0; j < graphOutput.size(); j++)
      {
        for (int k = 0; k < tabCounter; k++)
          std::cout << "\t";
        const Node* y = graphOutput[j]->getNode();
        std::cout << y->getName() << std::endl;
        ++tabCounter;
      }
      exit(1);
#else
      errorValue = 8;
      errorHandler();
#endif
    }
    x->setInitialized(true);
    graphOutput.push_back(topoNode);
    for (int j = 0; j < x->getNextNodes().size(); j++)
    {
      Node* y = x->getNextNodes()[j];
      --inDegreesVector[y->getIndex()];
      if (inDegreesVector[y->getIndex()] == 0)
        topoQueue.push_back(y);
    }
  }

  if (graphOutput.size() != graphStructureVector.size())
  {
#if !defined(ENERGIA)
    std::cout << "ERROR! cycle detected! " << graphOutput.size() << " "
        << graphStructureVector.size() << std::endl;
    exit(1);
#else
    errorValue = 9;
    errorHandler();
#endif
  }

  if (graphOutput.size() != graphStructureVector.size())
  {
#if !defined(ENERGIA)
    std::cerr << "ERROR!" << std::endl;
    exit(1);
#else
    errorValue = 10;
    errorHandler();
#endif
  }
}

void Graph::graphOutputInit()
{
  // 1) Allocate
  for (int iter = 0; iter < graphOutput.size(); iter++)
  {
    // 1) Init()
    graphOutput[iter]->init();
    // 2.1) Execute() / 2.2) Update()
    // (*iter)->update();
  }
}

void Graph::graphOutputUpdate()
{
  // 2) Execute / Update
  for (int iter = 0; iter < graphOutput.size(); iter++)
  {
    //startTimer((*iter)->getNode()->getName());
    // 2.1) Execute() / 2.2) Update()
    graphOutput[iter]->update();
    //stopTimer((*iter)->getNode()->getName());
  }

}

void Graph::errorHandler()
{

#if defined(ENERGIA)
  if (errorValue > 0)
  {
    Serial.begin(9600);
    pinMode(RED_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    for (;;)
    {
      digitalWrite(RED_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
      Serial.print("errorValue: ");
      Serial.println(errorValue);
      uint8_t ledErrorState = HIGH;
      for (int i = 0; i < errorValue; i++)
      {
        digitalWrite(RED_LED, ledErrorState);
        delay(1000);
        ledErrorState ^= HIGH; // toggle
      }
      digitalWrite(BLUE_LED, HIGH);
      delay(5000); // pause
    }
  }
#endif

}

void Graph::stream()
{
#if !defined(ENERGIA)
  std::cout << std::endl << std::endl;
  // This shows the raw graph
  for (int iter = 0; iter < moduleVector.size(); iter++)
  {
    const Graph::ModuleEntry* moduleEntry = moduleVector[iter];
    std::cout << moduleEntry->moduleNode->getName() << " " << moduleEntry->moduleNode->getIndex()
        << std::endl;
  }

  std::cout << std::endl;
  for (int iter = 0; iter < representationVector.size(); iter++)
  {
    const Graph::RepresentationEntry* representationEntry = representationVector[iter];
    std::cout << representationEntry->representationNode->getName() << " "
        << representationEntry->representationNode->getIndex() << " "
        << representationEntry->providedModuleName << std::endl;
  }

  std::cout << std::endl;

  for (int iter = 0; iter < graphStructureVector.size(); iter++)
  {
    Node* curr = graphStructureVector[iter];
    std::cout << "[" << curr->getIndex() << ":" << curr->getName() << "] ";
    for (int iter2 = 0; iter2 != curr->getNextNodes().size(); iter2++)
    {
      Node* next = curr->getNextNodes()[iter2];
      std::cout << "[" << next->getIndex() << ":" << next->getName() << "] ";
    }
    std::cout << std::endl;
  }

  for (int iter = 0; iter < graphOutput.size(); iter++)
  {
    const Node* x = graphOutput[iter]->getNode();
    std::cout << x->getIndex() << ":" << x->getName() << std::endl;
  }

  // Graphviz output
  std::cout << std::endl << std::endl;
  std::cout.flush();
  std::ofstream graph("graph_structure.dot");
  if (graph.is_open())
  {
    graph << "digraph G {\n";
    graph << "\t node [shape=box, color=lightblue2, style=filled]; ";
    for (int iter = 0; iter < graphOutput.size(); iter++)
    {
      const Node* x = graphOutput[iter]->getNode();
      if (x->isComputationNode())
        graph << " " << x->getName() << "; ";
    }
    graph << "\n";
    graph << "\t node [shape=ellipse, color=lightpink, style=filled]; ";
    for (int iter = 0; iter < graphOutput.size(); iter++)
    {
      Node* x = graphOutput[iter]->getNode();
      if (!x->isComputationNode())
        graph << " " << x->getName() << "; ";
    }
    graph << "\n";
    for (int iter = 0; iter < graphOutput.size(); iter++)
    {
      Node* x = graphOutput[iter]->getNode();
      if (!x->isNextNodesEmpty())
      {
        for (int j = 0; j < x->getNextNodes().size(); j++)
        {
          Node* y = x->getNextNodes()[j];
          if (y->isComputationNode())
            graph << "edge [color=green]; \n";
          else
            graph << "edge [color=blue]; \n";
          graph << "\t" << x->getName() << " -> " << y->getName() << "; \n";
        }
      }
      else
      {
        graph << "\t" << x->getName() << "; \n";
      }
    }
    graph << "edge [color=red]; \n";
    for (int iter = 0; iter < graphOutput.size(); iter++)
    {
      Node* x = graphOutput[iter]->getNode();
      if (!x->auxiliaryNodesEmpty())
      {
        for (int j = 0; j < x->getAuxiliaryNodes().size(); j++)
        {
          Node* y = x->getAuxiliaryNodes()[j];
          graph << "\t" << x->getName() << " -> " << y->getName() << "; \n";
        }
      }
    }

    graph << "\t fontsize=20; \n";
    graph << "} \n";
    graph.close();
  }
  else
  {
    std::cerr << "ERROR! unable to open the graph_structure.dot file" << std::endl;
  }
#endif
}

Graph& Graph::getInstance()
{
  if (!theInstance)
    theInstance = new Graph;
  return *theInstance;
}

