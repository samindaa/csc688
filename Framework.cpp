/*
 * Graph.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#include "Framework.h"

#if !defined(ENERGIA)
#include <fstream>
#endif

#include <string.h>

Graph* Graph::theInstance = 0;

Graph::Graph() :
    errorState(false), baudRate(115200)
{
}

Graph::~Graph()
{
  graphOutput.purge();
}

void Graph::addModule(Node* theInstance)
{
  // Check if a module type exits
  for (uint32_t iter = 0; iter < moduleVector.size(); iter++)
  {
    if (strcmp(moduleVector[iter]->moduleNode->getName(), theInstance->getName()) == 0)
    {
      errorMsg += (" moduleByName=");
      errorMsg += (theInstance->getName());
      errorMsg += ("exists!");
#if !defined(ENERGIA)
      std::cout << "ERROR!" << errorMsg << std::endl;
      exit(1);
#else
      errorState = true;
#endif
    }
  }

  theInstance->setComputationNode(true);
  Graph::ModuleEntry* newModuleEntry = new Graph::ModuleEntry(theInstance);
  moduleVector.push_back(newModuleEntry);
}

void Graph::providedRepresentation(const char* moduleName, Node* theInstance,
    void (*updateRepresentation)(Node*, Node*))
{
  // Check if a representation type exits
  for (uint32_t iter = 0; iter < representationVector.size(); iter++)
  {
    if (strcmp(representationVector[iter]->representationNode->getName(), theInstance->getName())
        == 0)
    {
      errorMsg += (" representationByName=");
      errorMsg += (theInstance->getName());
      errorMsg += (" exists, and  providedModuleName=");
      errorMsg += (representationVector[iter]->providedModuleName);
#if !defined(ENERGIA)
      std::cout << "ERROR!" << errorMsg << std::endl;
      exit(1);
#else
      errorState = true;
#endif
    }
  }
  Graph::RepresentationEntry* newRepresentationEntry = new Graph::RepresentationEntry(moduleName,
      theInstance, updateRepresentation);
  representationVector.push_back(newRepresentationEntry);
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

/**
 * This is called at most once per required representation per module.
 */
Node* Graph::getRepresentation(const char* representationName)
{
  for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
  {
    Node* node = graphOutput[iter];
    if (!node->isComputationNode() && strcmp(node->getName(), representationName) == 0)
    {
      if (!node->isInitialized())
      {
        errorMsg += (" missing representation. representationName=");
        errorMsg += (representationName);
#if !defined(ENERGIA)
        std::cerr << " ERROR!" << errorMsg << std::endl;
        exit(1);
#else
        errorState = true;
#endif
      }
      return node;
    }
  }
  errorMsg += (" this should not happen. representationName=");
  errorMsg += (representationName);
#if !defined(ENERGIA)
  // This is a double check and nothing should enter at this point
  std::cerr << "ERROR!" << errorMsg << std::endl;
  exit(1);
#else
  errorState = true;
#endif
  return 0;
}

void Graph::computeGraph()
{
  // 0) Loading errors
  errorHandler();
  // 1) Add modules
  for (uint32_t iter = 0; iter < moduleVector.size(); iter++)
  {
    Graph::ModuleEntry* moduleEntry = moduleVector[iter];
    graphStructureVector.push_back(moduleEntry->moduleNode);
  }

  // 2) Provides representations
  for (uint32_t iter = 0; iter < representationVector.size(); iter++)
  {
    Graph::RepresentationEntry* representationEntry = representationVector[iter];
    graphStructureVector.push_back(representationEntry->representationNode);

    for (uint32_t iter2 = 0; iter2 < moduleVector.size(); iter2++)
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
  for (uint32_t iter = 0; iter < moduleRepresentationRequiredVector.size(); iter++)
  {
    Graph::ModuleRepresentationEntry* moduleRepresentationEntry =
        moduleRepresentationRequiredVector[iter];

    Node *moduleNode = 0, *representationNode = 0;
    for (uint32_t iter2 = 0; iter2 < moduleVector.size(); iter2++)
    {
      Graph::ModuleEntry* moduleEntry = moduleVector[iter2];
      if (strcmp(moduleEntry->moduleNode->getName(), moduleRepresentationEntry->requiredModuleName)
          == 0)
        moduleNode = moduleEntry->moduleNode;
    }

    for (uint32_t iter2 = 0; iter2 < representationVector.size(); iter2++)
    {
      Graph::RepresentationEntry* representationEntry = representationVector[iter2];
      if (strcmp(representationEntry->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = representationEntry->representationNode;
    }

    if (moduleNode == 0)
    {
      errorMsg += (" requiredModuleName=");
      errorMsg += (moduleRepresentationEntry->requiredModuleName);
      errorMsg += (" is missing!");
#if !defined(ENERGIA)
      std::cout << "ERROR!" << errorMsg << std::endl;
#else
      errorState = true;
#endif
    }
    if (representationNode == 0)
    {
      errorMsg += (" requiredRepresentationName=");
      errorMsg += (moduleRepresentationEntry->requiredRepresentationName);
      errorMsg += (" is missing!");
#if !defined(ENERGIA)
      std::cout << "ERROR!" << errorMsg << std::endl;
#endif
    }
    if (!(moduleNode && representationNode))
    {
      errorMsg += (" moduleNode and representationNode are NULL!");
#if !defined(ENERGIA)
      std::cerr << "ERROR!" << errorMsg << std::endl;
      exit(1);
#else
      errorState = true;
#endif

    }
    errorHandler();
    representationNode->addNextNode(moduleNode);
  }

  // 4) Uses representation
  for (uint32_t iter = 0; iter < moduleRepresentationUsedVector.size(); iter++)
  {
    Graph::ModuleRepresentationEntry* moduleRepresentationEntry =
        moduleRepresentationUsedVector[iter];

    Node *moduleNode = 0, *representationNode = 0;
    for (uint32_t iter2 = 0; iter2 < moduleVector.size(); iter2++)
    {
      Graph::ModuleEntry* moduleEntry = moduleVector[iter2];
      if (strcmp(moduleEntry->moduleNode->getName(), moduleRepresentationEntry->requiredModuleName)
          == 0)
        moduleNode = moduleEntry->moduleNode;
    }

    for (uint32_t iter2 = 0; iter2 < representationVector.size(); iter2++)
    {
      Graph::RepresentationEntry* representationEntry = representationVector[iter2];
      if (strcmp(representationEntry->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = representationEntry->representationNode;
    }

    if (!(moduleNode && representationNode))
    {
      errorMsg += (" moduleNode and representationNode are NULL!");
#if !defined(ENERGIA)
      std::cerr << "ERROR!" << errorMsg << std::endl;
      exit(1);
#else
      errorState = true;
#endif
    }
    errorHandler();
    representationNode->addAuxiliaryNode(moduleNode);
  }
}

void Graph::topoSort()
{
  // Calculate in-degrees
  for (uint32_t i = 0; i < graphStructureVector.size(); i++)
  {
    Node* x = graphStructureVector[i];
    for (uint32_t j = 0; j < x->getNextNodes().size(); j++)
      ++(*x->getNextNodes()[j]);
  }

  // Initialize the loop
  for (uint32_t i = 0; i < graphStructureVector.size(); i++)
  {
    Node* x = graphStructureVector[i];
    if (x->getInDegrees() == 0)
      topoQueue.push_back(x);
  }

  // Main loop
  while (!topoQueue.empty())
  {
    Node* x = topoQueue.front();
    topoQueue.erase(0);
    if (x->isInitialized())
    {
      graphOutput.push_back(x);
      errorMsg += (" cycle detected!");
      errorMsg += ("\n");
      int tabCounter = 0;
      for (uint32_t j = 0; j < graphOutput.size(); j++)
      {
        for (int k = 0; k < tabCounter; k++)
          errorMsg += ("\t");
        const Node* y = graphOutput[j];
        errorMsg += (y->getName());
        errorMsg += ("\n");
        ++tabCounter;
      }
#if !defined(ENERGIA)
      std::cout << "ERROR!" << errorMsg << std::endl;
      exit(1);
#else
      errorState = true;
      errorHandler();
#endif
    }
    x->setInitialized(true);
    graphOutput.push_back(x);
    for (uint32_t j = 0; j < x->getNextNodes().size(); j++)
    {
      Node* y = x->getNextNodes()[j];
      --(*y);
      if (y->getInDegrees() == 0)
        topoQueue.push_back(y);
    }
  }

  if (graphOutput.size() != graphStructureVector.size())
  {
    errorMsg += (" cycle detected! ");
#if !defined(ENERGIA)
    std::cout << "ERROR!" << errorMsg << (int) graphOutput.size() << " "
    << (int) graphStructureVector.size() << std::endl;
    exit(1);
#else
    errorState = true;
    errorHandler();
#endif
  }

  if (graphOutput.size() != graphStructureVector.size())
  {
    errorMsg += ("graphOutput.size() != graphStructureVector.size()");
#if !defined(ENERGIA)
    std::cerr << "ERROR!" << errorMsg << std::endl;
    exit(1);
#else
    errorState = true;
    errorHandler();
#endif
  }

  // Purge entries
  purgeEntries();
}

void Graph::graphOutputInit()
{
  // 1) Allocate
  for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
  {
    // 1) Init()
    Node* node = graphOutput[iter];
    if (node->isComputationNode())
    {
      ((Module*) node)->init();
#if defined(ENERGIA)
      Serial.println(node->getName());
#endif
    }
  }
}

void Graph::graphOutputUpdate()
{
  // 2) Execute / Update
  for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
  {
    // 2.1) Execute() / 2.2) Update()
    Node* node = graphOutput[iter];
    if (node->isComputationNode())
      ((Module*) node)->execute();
    else
    {
      ((Representation*) node)->updateThis(node->getPreviousNode(), node);
      ((Representation*) node)->draw();
    }
  }

}

void Graph::setBaudRate(const unsigned long& baudRate)
{
  this->baudRate = baudRate;
}

unsigned long Graph::getBaudRate() const
{
  return baudRate;
}

void Graph::errorHandler()
{

#if defined(ENERGIA)
  if (errorState)
  {
    Serial.begin(baudRate);
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    uint8_t ledErrorState = HIGH;
    unsigned long prevTime1 = millis();
    unsigned long prevTime2 = prevTime1;
    for (;;)
    {
      if (millis() - prevTime1 > 3000)
      {
        prevTime1 = millis();
        Serial.println(errorMsg);
      }
      uint8_t ledErrorState = HIGH;
      if (millis() - prevTime2 > 250)
      {
        prevTime2 = millis();
        ledErrorState ^= HIGH;
      }
      digitalWrite(RED_LED, ledErrorState);
    }
  }
#endif

}

void Graph::purgeEntries()
{
  for (uint32_t iter = 0; iter < moduleVector.size(); iter++)
    delete moduleVector[iter];
  for (uint32_t iter = 0; iter < representationVector.size(); iter++)
    delete representationVector[iter];
  for (uint32_t iter = 0; iter < moduleRepresentationRequiredVector.size(); iter++)
    delete moduleRepresentationRequiredVector[iter];
  for (uint32_t iter = 0; iter < moduleRepresentationUsedVector.size(); iter++)
    delete moduleRepresentationUsedVector[iter];
  moduleVector.purge();
  representationVector.purge();
  moduleRepresentationRequiredVector.purge();
  moduleRepresentationUsedVector.purge();
  graphStructureVector.purge();
  topoQueue.purge();
}

void Graph::stream()
{
#if !defined(ENERGIA)
  std::cout << std::endl << std::endl;
  // This shows the raw graph
  std::cout << "moduleVector.size()=" << (int) moduleVector.size() << std::endl;
  for (uint32_t iter = 0; iter < moduleVector.size(); iter++)
  {
    const Graph::ModuleEntry* moduleEntry = moduleVector[iter];
    std::cout << moduleEntry->moduleNode->getName() << std::endl;
  }

  std::cout << std::endl;
  std::cout << "representationVector.size()=" << (int) representationVector.size() << std::endl;
  for (uint32_t iter = 0; iter < representationVector.size(); iter++)
  {
    const Graph::RepresentationEntry* representationEntry = representationVector[iter];
    std::cout << representationEntry->representationNode->getName() << " "
    << representationEntry->providedModuleName << std::endl;
  }

  std::cout << std::endl;
  std::cout << "graphStructureVector.size()=" << (int) graphStructureVector.size() << std::endl;
  for (uint32_t iter = 0; iter < graphStructureVector.size(); iter++)
  {
    Node* curr = graphStructureVector[iter];
    std::cout << "[" << curr->getName() << "] ";
    for (uint32_t iter2 = 0; iter2 != curr->getNextNodes().size(); iter2++)
    {
      Node* next = curr->getNextNodes()[iter2];
      std::cout << "[" << next->getName() << "] ";
    }
    std::cout << std::endl;
  }

  std::cout << "graphOutput.size()=" << (int) graphOutput.size() << std::endl;
  for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
  {
    const Node* x = graphOutput[iter];
    std::cout << x->getName() << std::endl;
  }

  // Graphviz output
  std::cout << std::endl << std::endl;
  std::cout.flush();
  std::ofstream graph("graph_structure.dot");
  if (graph.is_open())
  {
    graph << "digraph G {\n";
    graph << "\t node [shape=box, color=lightblue2, style=filled]; ";
    for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
    {
      const Node* x = graphOutput[iter];
      if (x->isComputationNode())
      graph << " " << x->getName() << "; ";
    }
    graph << "\n";
    graph << "\t node [shape=ellipse, color=lightpink, style=filled]; ";
    for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
    {
      Node* x = graphOutput[iter];
      if (!x->isComputationNode())
      graph << " " << x->getName() << "; ";
    }
    graph << "\n";
    for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
    {
      Node* x = graphOutput[iter];
      if (!x->isNextNodesEmpty())
      {
        for (uint32_t j = 0; j < x->getNextNodes().size(); j++)
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
    for (uint32_t iter = 0; iter < graphOutput.size(); iter++)
    {
      Node* x = graphOutput[iter];
      if (!x->auxiliaryNodesEmpty())
      {
        for (uint32_t j = 0; j < x->getAuxiliaryNodes().size(); j++)
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

void Graph::deleteInstance()
{
  if (theInstance)
    delete theInstance;
  theInstance = 0;
}

