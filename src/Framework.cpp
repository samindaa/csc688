/*
 * Graph.cpp
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#include "Framework.h"

#include <string.h>   //<< Not to be removed
#include <stdlib.h>  //<< TODO: remove
//#include <fstream>  //<< TODO: remove

Graph::Graph() :
    nodeCounter(0), error(false)
{
}

Graph::~Graph()
{
  for (Graph::ModuleVector::iterator iter = moduleVector.begin();
      iter != moduleVector.end(); ++iter)
    delete *iter;
  for (Graph::RepresentationVector::iterator iter =
      representationVector.begin(); iter != representationVector.end(); ++iter)
    delete *iter;
  for (Graph::ModuleRepresentationVector::iterator iter =
      moduleRepresentationRequiredVector.begin();
      iter != moduleRepresentationRequiredVector.end(); ++iter)
    delete *iter;
  for (Graph::ModuleRepresentationVector::iterator iter =
      moduleRepresentationUsedVector.begin();
      iter != moduleRepresentationUsedVector.end(); ++iter)
    delete *iter;
  for (Graph::GraphOutput::iterator iter = graphOutput.begin();
      iter != graphOutput.end(); ++iter)
    delete *iter;
}

Graph& Graph::getInstance()
{
  static Graph theInstance;
  return theInstance;
}

void Graph::addModule(Node* theInstance)
{
  // Check if a module type exits
  for (Graph::ModuleVector::const_iterator iter = moduleVector.begin();
      iter != moduleVector.end(); ++iter)
  {
    if (strcmp((*iter)->moduleNode->getName(), theInstance->getName()) == 0)
    {
      error = true;
      // TODO:
     /* std::cout << "ERROR! moduleByName=" << theInstance->getName()
          << " exists!" << std::endl; */
      exit(1);
    }
  }

  theInstance->setIndex(nodeCounter++);
  theInstance->setComputationNode(true);
  Graph::ModuleEntry* newModuleEntry = new Graph::ModuleEntry(theInstance);
  moduleVector.push_back(newModuleEntry);
  inDegreesMap.insert(std::make_pair(theInstance->getIndex(), 0));
}

void Graph::providedRepresentation(const char* moduleName, Node* theInstance,
    void (*updateRepresentation)(Node*, Node*))
{
  // Check if a representation type exits
  for (Graph::RepresentationVector::const_iterator iter =
      representationVector.begin(); iter != representationVector.end(); ++iter)
  {
    if (strcmp((*iter)->representationNode->getName(), theInstance->getName())
        == 0)
    {
      error = true;
      /*std::cout << "ERROR! representationByName=" << theInstance->getName()
          << " exists, and " << "providedModuleName="
          << (*iter)->providedModuleName << std::endl;*/
      exit(1);
    }
  }
  theInstance->setIndex(nodeCounter++);
  Graph::RepresentationEntry* newRepresentationEntry =
      new Graph::RepresentationEntry(moduleName, theInstance,
          updateRepresentation);
  representationVector.push_back(newRepresentationEntry);
  inDegreesMap.insert(std::make_pair(theInstance->getIndex(), 0));
}

void Graph::requiredRepresentation(const char* moduleName,
    const char* representationName)
{
  Graph::ModuleRepresentationEntry* newModuleRepresentationEntry =
      new Graph::ModuleRepresentationEntry(moduleName, representationName);
  moduleRepresentationRequiredVector.push_back(newModuleRepresentationEntry);
}

void Graph::usedRepresentation(const char* moduleName,
    const char* representationName)
{
  Graph::ModuleRepresentationEntry* newModuleRepresentationEntry =
      new Graph::ModuleRepresentationEntry(moduleName, representationName);
  moduleRepresentationUsedVector.push_back(newModuleRepresentationEntry);
}

Node* Graph::getRepresentation(const char* representationName)
{
  for (Graph::RepresentationVector::iterator iter =
      representationVector.begin(); iter != representationVector.end(); ++iter)
  {
    Graph::RepresentationEntry* representationEntry = *iter;
    if (strcmp(representationEntry->representationNode->getName(),
        representationName) == 0)
    {
      if (!representationEntry->representationNode->getInitialized())
      {
        error = true;
       /* std::cerr << " ERROR! " << std::endl;*/
        exit(1);
      }
      return representationEntry->representationNode;
    }
  }
  /** This is a double check and nothing should enter at this point */
  error = true;
  /*std::cerr << " ERROR! " << std::endl;*/
  exit(1);
  return 0;
}

void Graph::computeGraph()
{
  // 1) Add modules
  for (Graph::ModuleVector::iterator iter = moduleVector.begin();
      iter != moduleVector.end(); ++iter)
  {
    Graph::ModuleEntry* moduleEntry = *iter;
    graphStructure.insert(
        std::make_pair(moduleEntry->moduleNode->getIndex(),
            moduleEntry->moduleNode));
  }

  // 2) Provides representations
  for (Graph::RepresentationVector::iterator iter =
      representationVector.begin(); iter != representationVector.end(); ++iter)
  {
    Graph::RepresentationEntry* representationEntry = *iter;
    graphStructure.insert(
        std::make_pair(representationEntry->representationNode->getIndex(),
            representationEntry->representationNode));

    for (Graph::ModuleVector::iterator iter2 = moduleVector.begin();
        iter2 != moduleVector.end(); ++iter2)
    {
      if (strcmp((*iter2)->moduleNode->getName(),
          representationEntry->providedModuleName) == 0)
      {
        representationEntry->representationNode->addPreviousNode(
            (*iter2)->moduleNode);
        ((Representation*) representationEntry->representationNode)->updateThis =
            representationEntry->update;
        (*iter2)->moduleNode->addNextNode(
            representationEntry->representationNode);
      }
    }
  }

  // 3) Requires representations
  for (Graph::ModuleRepresentationVector::iterator iter =
      moduleRepresentationRequiredVector.begin();
      iter != moduleRepresentationRequiredVector.end(); ++iter)
  {
    Graph::ModuleRepresentationEntry* moduleRepresentationEntry = *iter;

    Node *moduleNode = 0, *representationNode = 0;
    for (Graph::ModuleVector::iterator iter2 = moduleVector.begin();
        iter2 != moduleVector.end(); ++iter2)
    {
      if (strcmp((*iter2)->moduleNode->getName(),
          moduleRepresentationEntry->requiredModuleName) == 0)
        moduleNode = (*iter2)->moduleNode;
    }

    for (Graph::RepresentationVector::iterator iter2 =
        representationVector.begin(); iter2 != representationVector.end();
        ++iter2)
    {
      if (strcmp((*iter2)->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
      {
        representationNode = (*iter2)->representationNode;
      }
    }

    if (moduleNode == 0)
    {
     /* std::cout << "requiredModuleName="
          << moduleRepresentationEntry->requiredModuleName << " is missing!"
          << std::endl;*/
      error = true;
    }
    if (representationNode == 0)
    {
      error = true;
      /*std::cout << "requiredRepresentationName="
          << moduleRepresentationEntry->requiredRepresentationName
          << " is missing!" << std::endl;*/
    }
    if (!(moduleNode && representationNode))
    {
      error = true;
      /*std::cerr << "ERROR " << std::endl;*/
      exit(1);
    }
    representationNode->addNextNode(moduleNode);

  }

  // 4) Uses representation
  for (Graph::ModuleRepresentationVector::iterator iter =
      moduleRepresentationUsedVector.begin();
      iter != moduleRepresentationUsedVector.end(); ++iter)
  {
    Graph::ModuleRepresentationEntry* moduleRepresentationEntry = *iter;

    Node *moduleNode = 0, *representationNode = 0;
    for (Graph::ModuleVector::iterator iter2 = moduleVector.begin();
        iter2 != moduleVector.end(); ++iter2)
    {
      if (strcmp((*iter2)->moduleNode->getName(),
          moduleRepresentationEntry->requiredModuleName) == 0)
        moduleNode = (*iter2)->moduleNode;
    }

    for (Graph::RepresentationVector::iterator iter2 =
        representationVector.begin(); iter2 != representationVector.end();
        ++iter2)
    {
      if (strcmp((*iter2)->representationNode->getName(),
          moduleRepresentationEntry->requiredRepresentationName) == 0)
        representationNode = (*iter2)->representationNode;
    }

    if (!(moduleNode && representationNode))
    {
      error = true;
      /*std::cerr << "ERROR!" << std::endl;*/
      exit(1);
    }

    representationNode->addAuxiliaryNode(moduleNode);

  }

}

void Graph::topoSort()
{
  // Calculate in-degrees
  for (Graph::GraphStructure::iterator i = graphStructure.begin();
      i != graphStructure.end(); ++i)
  {
    Node* x = i->second;
    for (Node::iterator j = x->nextNodesBegin(); j != x->nextNodesEnd(); ++j)
      ++inDegreesMap[(*j)->getIndex()];
  }

  // Initialize the loop
  for (Graph::InDegreesMap::iterator i = inDegreesMap.begin();
      i != inDegreesMap.end(); ++i)
  {
    if (i->second == 0)
      topoQueue.push_back(graphStructure[i->first]);
  }

  // Main loop
  while (!topoQueue.empty())
  {
    Node* x = topoQueue.front();
    topoQueue.erase(topoQueue.begin());

    TopoNode* topoNode = 0;
    if (x->getComputationNode())
      topoNode = new TopoModule((Module*) x);
    else
      topoNode = new TopoRepresentation(((Module*) *(x->previousNodesBegin())),
          (Representation*) x);

    if (x->getInitialized())
    {
      graphOutput.push_back(topoNode);
      error  = true;
      /*std::cout << "ERROR! Cycle detected!" << std::endl;
      int tabCounter = 0;
      for (Graph::GraphOutput::const_iterator j = graphOutput.begin();
          j != graphOutput.end(); ++j)
      {
        for (int k = 0; k < tabCounter; k++)
          std::cout << "\t";
        const Node* y = (*j)->getNode();
        std::cout << y->getName() << std::endl;
        ++tabCounter;
      }*/
      exit(1);
    }
    x->setInitialized(true);
    graphOutput.push_back(topoNode);
    for (Node::iterator j = x->nextNodesBegin(); j != x->nextNodesEnd(); ++j)
    {
      Node* y = *j;
      --inDegreesMap[y->getIndex()];
      if (inDegreesMap[y->getIndex()] == 0)
        topoQueue.push_back(y);
    }
  }

  if (graphOutput.size() != graphStructure.size())
  {
    error = true;
    /*std::cout << "ERROR! cycle detected!" << std::endl;*/
    exit(1);
  }

  if (graphOutput.size() != graphStructure.size())
  {
    error = true;
    /*std::cerr << "ERROR!" << std::endl;*/
    exit(1);
  }
}

void Graph::graphOutputInit()
{
  // 1) Allocate
  for (Graph::GraphOutput::iterator iter = graphOutput.begin();
      iter != graphOutput.end(); ++iter)
  {
    // 1) Init()
    (*iter)->init();
    // 2.1) Execute() / 2.2) Update()
    // (*iter)->update();
  }
}

void Graph::graphOutputUpdate()
{
  // 2) Execute / Update
  for (Graph::GraphOutput::iterator iter = graphOutput.begin();
      iter != graphOutput.end(); ++iter)
  {
    //startTimer((*iter)->getNode()->getName());
    // 2.1) Execute() / 2.2) Update()
    (*iter)->update();
    //stopTimer((*iter)->getNode()->getName());
  }

  //displayTimers();
}

/*void Graph::stream()
{
  std::cout << std::endl << std::endl;
  // This shows the raw graph
  for (Graph::ModuleVector::const_iterator iter = moduleVector.begin();
      iter != moduleVector.end(); ++iter)
  {
    const Graph::ModuleEntry* moduleEntry = *iter;
    std::cout << moduleEntry->moduleNode->getName() << " "
        << moduleEntry->moduleNode->getIndex() << std::endl;
  }

  std::cout << std::endl;
  for (Graph::RepresentationVector::const_iterator iter =
      representationVector.begin(); iter != representationVector.end(); ++iter)
  {
    const Graph::RepresentationEntry* representationEntry = *iter;
    std::cout << representationEntry->representationNode->getName() << " "
        << representationEntry->representationNode->getIndex() << " "
        << representationEntry->providedModuleName << std::endl;
  }

  std::cout << std::endl;

  for (Graph::GraphStructure::const_iterator iter = graphStructure.begin();
      iter != graphStructure.end(); ++iter)
  {
    std::cout << "[" << iter->first << ":" << iter->second->getName() << "] ";
    for (Node::const_iterator iter2 = iter->second->nextNodesBegin();
        iter2 != iter->second->nextNodesEnd(); ++iter2)
    {
      Node* next = *iter2;
      std::cout << "[" << next->getIndex() << ":" << next->getName() << "] ";
    }
    std::cout << std::endl;
  }

  for (Graph::GraphOutput::const_iterator iter = graphOutput.begin();
      iter != graphOutput.end(); ++iter)
  {
    const Node* x = (*iter)->getNode();
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
    for (Graph::GraphOutput::const_iterator iter = graphOutput.begin();
        iter != graphOutput.end(); ++iter)
    {
      const Node* x = (*iter)->getNode();
      if (x->getComputationNode())
        graph << " " << x->getName() << "; ";
    }
    graph << "\n";
    graph << "\t node [shape=ellipse, color=lightpink, style=filled]; ";
    for (Graph::GraphOutput::const_iterator iter = graphOutput.begin();
        iter != graphOutput.end(); ++iter)
    {
      const Node* x = (*iter)->getNode();
      if (!x->getComputationNode())
        graph << " " << x->getName() << "; ";
    }
    graph << "\n";
    for (Graph::GraphOutput::const_iterator iter = graphOutput.begin();
        iter != graphOutput.end(); ++iter)
    {
      const Node* x = (*iter)->getNode();
      if (!x->nextNodesEmpty())
      {
        for (Node::const_iterator j = x->nextNodesBegin();
            j != x->nextNodesEnd(); ++j)
        {
          Node* y = *j;
          if (y->getComputationNode())
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
    for (Graph::GraphOutput::const_iterator iter = graphOutput.begin();
        iter != graphOutput.end(); ++iter)
    {
      const Node* x = (*iter)->getNode();
      if (!x->auxiliaryNodesEmpty())
      {
        for (Node::const_iterator j = x->auxiliaryNodesBegin();
            j != x->auxiliaryNodesEnd(); ++j)
        {
          Node* y = *j;
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
    std::cerr << "ERROR! unable to open the graph_structure.dot file"
        << std::endl;
  }
}*/

