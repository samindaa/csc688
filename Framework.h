/*
 * Graph.h
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#if defined(ENERGIA)
#define EMBEDDED_MODE
#endif

#if defined(EMBEDDED_MODE)
#include "Energia.h"
#include "pins_energia.h"
#else
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#endif
//
#if !defined(EMBEDDED_MODE)
#include "Config.h"
#endif

/**
 * This framework creates a topologically sorted graph from computational and representational units
 * created by the user. Since, the framework needs to be run on a MCU, the size (in bytes) of the
 * framework is minimized. At most INT_MAX nodes can be allocated within the framework.
 */

/**
 * This is a very simple vector class
 */
template<typename T>
class Vector
{
  private:
    int theSize;
    int theCapacity;
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
        for (int k = 0; k < size(); k++)
          objects[k] = that.objects[k];

      }

      return *this;
    }

    void resize(int newSize)
    {
      if (newSize > theCapacity)
        reserve(newSize * 2 + 1);
      theSize = newSize;
    }

    void reserve(int newCapacity)
    {
      if (newCapacity < theSize)
        return;

      T *oldArray = objects;

      objects =  (T*)malloc(newCapacity * sizeof(T));
      for (int k = 0; k < theSize; k++)
        objects[k] = oldArray[k];

      theCapacity = newCapacity;

      if (oldArray)
       free(oldArray);
    }

    T& operator[](int index) { return objects[index]; }
    const T& operator[](int index) const { return objects[index]; }
    bool empty() const { return size() == 0;  }
    int size() const { return theSize; }
    int capacity() const { return theCapacity; }

    void push_back(T x)
    {
      if (theSize == theCapacity)
        reserve(2 * theCapacity + 1);
      objects[theSize++] = x;
    }

    const T& front() const { return objects[0]; }
    T& front() { return objects[0]; }

    void erase(int index)
    {
      for (int i = index; i < size() - 1; ++i)    // for each item that follows 'index'
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
    int inDegrees;
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
    int getInDegrees()                              const { return inDegrees; }
    virtual const char* getName() const =0;
};

class Module : public Node
{
#if !defined(EMBEDDED_MODE)
  public: Config config;
#endif
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
#if defined(EMBEDDED_MODE)
    typedef String ErrorMsg;
#else
    typedef std::string ErrorMsg;
#endif
    ErrorMsg errorMsg;

#if defined(EMBEDDED_MODE)
    unsigned long baudRate;
#endif

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

#if defined(EMBEDDED_MODE)
    void setBaudRate(const unsigned long& baudRate);
    unsigned long getBaudRate() const;
#endif

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
  private:
    T theInstance;
  public:
    ModuleLoader() { Graph::getInstance().addModule(&theInstance); }
    ~ModuleLoader() { }
};

template<const char* (*getModuleName)(), void (*updateRepresentation)(Node*, Node*), class T>
class RepresentationProvider
{
  private :
    T theInstance;
  public:
    RepresentationProvider() { Graph::getInstance().providedRepresentation(getModuleName(), &theInstance, updateRepresentation); }
    virtual ~RepresentationProvider() { }
};

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
class RepresentationPointer
{
  protected:
    T* theInstance;

  public:
    RepresentationPointer() : theInstance(0) {  }
    virtual ~RepresentationPointer() { }

  protected:
    inline T* getRepresentation()
    {
      if (!theInstance)
        theInstance = (T*) Graph::getInstance().getRepresentation(getRepresentationName());
      return theInstance;
    }

    inline T* getRepresentation() const
    {
      if (!theInstance)
        return theInstance;
      return (T*) Graph::getInstance().getRepresentation(getRepresentationName()); //<< SLOW for CONST ACCESS
    }

  public:
    virtual bool isNull()                   { return (this->getRepresentation() == 0); }
    virtual bool operator==(const T* other) { return this->getRepresentation() == other; }
    virtual bool operator!=(const T* other) { return !(this->getRepresentation() == other); }

};

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
class RepresentationRequierer : public RepresentationPointer<getModuleName, getRepresentationName, T>
{
  public:
    RepresentationRequierer() { Graph::getInstance().requiredRepresentation(getModuleName(), getRepresentationName()); }
    virtual ~RepresentationRequierer() { }

  public:
    const T* operator->()        { return this->getRepresentation(); }
    const T* operator->() const  { return this->getRepresentation(); }
    const T& operator *()        { return *(this->getRepresentation()); }
    const T& operator *() const  { return *(this->getRepresentation()); }
    operator const   T*()        { return this->getRepresentation(); }
    operator const   T*() const  { return this->getRepresentation(); }

};

template<const char* (*getModuleName)(), const char* (*getRepresentationName)(), class T>
class RepresentationUser : public RepresentationPointer<getModuleName, getRepresentationName, T>
{
  public:
    RepresentationUser() { Graph::getInstance().usedRepresentation(getModuleName(), getRepresentationName()); }
    virtual ~RepresentationUser() { }

  public:
    const T* operator->()        { return this->getRepresentation(); }
    const T* operator->() const  { return this->getRepresentation(); }
    const T& operator *()        { return *(this->getRepresentation()); }
    const T& operator *() const  { return *(this->getRepresentation()); }
    operator const   T*()        { return this->getRepresentation(); }
    operator const   T*() const  { return this->getRepresentation(); }

};


#endif /* FRAMEWORK_H_ */
