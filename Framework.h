/*
 * Graph.h
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#if defined(ENERGIA)
#include "Energia.h"
#include "pins_energia.h"
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#endif

/**
 * This framework creates a topologically sorted graph from computational and representational units
 * created by the user. Since, the framework needs to be run on a MCU, the size (in bytes) of the
 * framework is minimized. At most 255 nodes can be allocated within the framework.
 */

/**
 * This is a very simple vector class
 */
template<typename T>
class Vector
{
private:
  uint8_t theSize;
  uint8_t theCapacity;
  T* objects;

public:
  explicit Vector() :
      theSize(0), theCapacity(0), objects(0)
  {
  }

  Vector(const Vector& that) :
      objects(0)
  {
    operator=(that);
  }

  ~Vector()
  {
    if (objects)
      free(objects);
  }

  const Vector & operator=(const Vector& that)
  {
    if (this != &that)
    {
      if (objects)
        free(objects);
      objects = 0;
      theSize = that.size();
      theCapacity = that.theCapacity;

      objects = (T*)malloc(capacity() * sizeof(T));
      for (uint8_t k = 0; k < size(); k++)
        objects[k] = that.objects[k];

    }

    return *this;
  }

  void resize(uint8_t newSize)
  {
    if (newSize > theCapacity)
      reserve(newSize * 2 + 1);
    theSize = newSize;
  }

  void reserve(uint8_t newCapacity)
  {
    if (newCapacity < theSize)
      return;

    T *oldArray = objects;

    objects =  (T*)malloc(newCapacity * sizeof(T));
    for (uint8_t k = 0; k < theSize; k++)
      objects[k] = oldArray[k];

    theCapacity = newCapacity;

    if (oldArray)
     free(oldArray);
  }

  T& operator[](uint8_t index) { return objects[index]; }
  const T& operator[](uint8_t index) const { return objects[index]; }
  bool empty() const { return size() == 0;  }
  uint8_t size() const { return theSize; }
  uint8_t capacity() const { return theCapacity; }

  void push_back(T x)
  {
    if (theSize == theCapacity)
      reserve(2 * theCapacity + 1);
    objects[theSize++] = x;
  }

  const T& front() const { return objects[0]; }
  T& front() { return objects[0]; }

  void erase(uint8_t index)
  {
    for (uint8_t i = index; i < size() - 1; ++i)    // for each item that follows 'index'
      objects[i] = objects[i + 1];            // shift the item down one slot in memory
    --theSize;
  }

    void purge()
    {
      theSize = 0;
      theCapacity = 0;
      if (objects)
        delete[] objects;
      objects = 0;
    }

};

// ADT's for building the graph.
/**
 * Every object in the graph is an instance of a Node class. But this node
 * behaves differently according to its responsibilities.
 */
class Node
{
  public:
    typedef Vector<Node*> NodeVector;
  private:
    NodeVector nextNodes;
    NodeVector auxiliaryNodes;
    Node* previousNode;
    uint8_t inDegrees;
    bool initialized;
    bool computationNode;

  public:
    explicit Node() : previousNode(0), inDegrees(0), initialized(false), computationNode(false) {}
    virtual ~Node() {}

    bool isInitialized()                                const { return initialized; }
    void setInitialized(const bool initialized)               { if (!this->initialized) this->initialized = initialized; }
    bool isComputationNode()                            const { return computationNode; }
    void setComputationNode(const bool computationNode)       { if (!initialized) this->computationNode = computationNode; }
    void addAuxiliaryNode(Node* that)                         { if (!initialized) this->auxiliaryNodes.push_back(that);  }
    void setPreviousNode(Node* that)                          { if (!initialized) this->previousNode = that;  }
    void addNextNode(Node* that)                              { if (!initialized) this->nextNodes.push_back(that);  }

    bool isPreviousNodeEmpty()                          const { return previousNode == 0; }
    bool isNextNodesEmpty()                             const { return nextNodes.empty(); }
    bool auxiliaryNodesEmpty()                          const { return auxiliaryNodes.empty(); }
    NodeVector& getNextNodes()                                { return nextNodes; }
    Node* getPreviousNode()                                   { return previousNode; }
    NodeVector& getAuxiliaryNodes()                           { return auxiliaryNodes; }
    void operator++()                                         { inDegrees++;  }
    void operator--()                                         { inDegrees--; }
    uint8_t getInDegrees()                              const { return inDegrees; }
    virtual const char* getName() const =0;
};

class Module : public Node
{
  public: Module() : Node()       {}
  public: virtual ~Module()       {}
  public: virtual void init()     {}
  public: virtual void execute()  {}
};

class Representation: public Node
{
  public: void (*updateThis)(Node* , Node* );
  public: Representation() : Node(), updateThis(0)  {}
  public: virtual ~Representation()                 {}
  public: virtual void draw() const                 {}
};

class Graph
{
  public:
    class ModuleEntry
    {
      public:
        Node* moduleNode;
        ModuleEntry(Node* moduleNode) : moduleNode(moduleNode) {}
    };

    class RepresentationEntry
    {
      public:
        const char* providedModuleName;
        Node* representationNode;
        void (*update)(Node*, Node*);

        RepresentationEntry(const char* providedModuleName, Node* representationNode, void (*update)(Node*, Node*)) :
            providedModuleName(providedModuleName), representationNode(representationNode), update(update) {}
    };

    class ModuleRepresentationEntry
    {
      public:
        const char* requiredModuleName;
        const char* requiredRepresentationName;

        ModuleRepresentationEntry(const char*  requiredModuleName, const char* requiredRepresentationName):
          requiredModuleName(requiredModuleName), requiredRepresentationName(requiredRepresentationName) {}
    };

    typedef Vector<ModuleEntry*> ModuleVector;
    typedef Vector<RepresentationEntry*> RepresentationVector;
    typedef Vector<ModuleRepresentationEntry*> ModuleRepresentationVector;
    //typedef Vector<uint8_t> InDegreesVector;
    typedef Vector<Node*> GraphStructureVector;
    ModuleVector moduleVector;
    RepresentationVector representationVector;
    ModuleRepresentationVector moduleRepresentationRequiredVector;
    ModuleRepresentationVector moduleRepresentationUsedVector;
    GraphStructureVector graphStructureVector;

    // For topological sort
    typedef Vector<Node*> TopoQueue;
    typedef Vector<Node*> GraphOutput;
    TopoQueue topoQueue;
    GraphOutput graphOutput;
    bool errorState;
#if defined(ENERGIA)
    typedef String ErrorMsg;
#else
    typedef std::string ErrorMsg;
#endif
    ErrorMsg errorMsg;
    unsigned long baudRate;

    static Graph& getInstance();
    static void deleteInstance();
    void addModule(Node* theInstance);
    void providedRepresentation(const char* moduleName, Node* theInstance, void (*updateRepresentation)(Node* , Node* ));
    void requiredRepresentation(const char* moduleName, const char* representationName);
    void usedRepresentation(const char* moduleName, const char* representationName);
    Node* getRepresentation(const char* representationName);

    /** Computational resources */
    void computeGraph();
    void topoSort();
    void graphOutputInit();
    void graphOutputUpdate();
    void setBaudRate(const unsigned long& baudRate);
    unsigned long getBaudRate() const;

  private:
    void errorHandler();
    void purgeEntries();

  protected:
    Graph();
    ~Graph();
    Graph(Graph const&);
    Graph& operator=(Graph const&);
    static Graph* theInstance;

  public: /** verbose */
    void stream();
};

// All the computational units are loaded into an instance of this class.
template <class T>
class ModuleLoader
{
  public:
    T theInstance;
    ModuleLoader() { Graph::getInstance().addModule(&theInstance); }
    ~ModuleLoader() { }
};

template<const char* (*getModuleName)(), void (*updateRepresentation)(Node*, Node*), class T>
class RepresentationProvider
{
  public:
    T theInstance;
    RepresentationProvider() { Graph::getInstance().providedRepresentation(getModuleName(), &theInstance, updateRepresentation); }
    ~RepresentationProvider() { }
};

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
class RepresentationRequierer
{
  public:
    RepresentationRequierer() { Graph::getInstance().requiredRepresentation(getModuleName(), getRepresentationName()); }

  protected:
    T* getRepresentation() const
    {
      static T* theInstance = 0;
      if (theInstance == 0)
        theInstance = (T*) Graph::getInstance().getRepresentation(getRepresentationName());
      return theInstance;
    };
  public:
    const T* operator->() const { return getRepresentation(); }
    const T& operator*()  const { return *(getRepresentation()); }
    operator const T*()   const { return getRepresentation(); }
    bool isNull()         const { return (getRepresentation() == 0); }

};

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
class RepresentationUser
{
  public:
    RepresentationUser() { Graph::getInstance().usedRepresentation(getModuleName(), getRepresentationName()); }

  protected:
    T* getRepresentation() const
    {
      static T* theInstance = 0;
      if (theInstance == 0)
        theInstance = (T*) Graph::getInstance().getRepresentation(getRepresentationName());
      return theInstance;
    };
  public:
    const T* operator->() const { return getRepresentation(); }
    const T& operator*()  const { return *(getRepresentation()); }
    operator const T*()   const { return getRepresentation(); }
    bool isNull()         const { return (getRepresentation() == 0); }

};


#endif /* FRAMEWORK_H_ */
