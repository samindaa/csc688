/*
 * Graph.h
 *
 *  Created on: Jul 12, 2013
 *      Author: sam
 */

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#include <vector>
#include <map>


// ADT's for building the graph.

/**
 * Every object in the graph is an instance of a Node class. But this node
 * behaves differently according to its responsibilities.
 */
class Node
{
  private:
    std::vector<Node*> auxiliaryNodes;
    std::vector<Node*> previousNodes;
    std::vector<Node*> nextNodes;
    unsigned int index;
    bool initialized;
    bool computationNode;

  public:
    explicit Node() : index(0), initialized(false), computationNode(false) {}
    virtual ~Node() {}

    unsigned int getIndex() const { return index; }
    bool getInitialized()   const { return initialized; }
    void setIndex(const unsigned int index) { if (!initialized) this->index = index;}
    void setInitialized(const bool initialized) { if (!this->initialized) this->initialized = initialized; }
    bool getComputationNode() const { return computationNode; }
    void setComputationNode(const bool computationNode) { if (!initialized) this->computationNode = computationNode; }
    void addAuxiliaryNode(Node* that) { if (!initialized) this->auxiliaryNodes.push_back(that);  }
    void addPreviousNode(Node* that) { if (!initialized) this->previousNodes.push_back(that);  }
    void addNextNode(Node* that) { if (!initialized) this->nextNodes.push_back(that);  }

    typedef std::vector<Node*>::iterator iterator;
    typedef std::vector<Node*>::const_iterator const_iterator;
    iterator auxiliaryNodesBegin() { return auxiliaryNodes.begin(); }
    iterator auxiliaryNodesEnd()   { return auxiliaryNodes.end();   }
    iterator nextNodesBegin()      { return nextNodes.begin(); }
    iterator nextNodesEnd()        { return nextNodes.end();   }
    iterator previousNodesBegin()  { return previousNodes.begin(); }
    iterator previousNodesEnd()    { return previousNodes.end();   }
    const_iterator auxiliaryNodesBegin() const { return auxiliaryNodes.begin(); }
    const_iterator auxiliaryNodesEnd()   const { return auxiliaryNodes.end();   }
    const_iterator nextNodesBegin()      const { return nextNodes.begin(); }
    const_iterator nextNodesEnd()        const { return nextNodes.end();   }
    const_iterator previousNodesBegin()  const { return previousNodes.begin(); }
    const_iterator previousNodesEnd()    const { return previousNodes.end();   }
    bool previousNodesEmpty()  const { return previousNodes.empty(); }
    bool nextNodesEmpty()      const { return nextNodes.empty(); }
    bool auxiliaryNodesEmpty() const { return auxiliaryNodes.empty(); }

    virtual const char* getName() const =0;
};

class Module : public Node
{
  public: Module() : Node() {}
  public: virtual ~Module() {}
  public: virtual void init() {}
  public: virtual void execute() {}
};

class Representation: public Node
{
  public: void (*updateThis)(Node* , Node* );
  public: Representation() : Node(), updateThis(0) {}
  public: virtual ~Representation() {}
};

class TopoNode
{
  public:
    virtual ~TopoNode() {}
    virtual void init() =0;
    virtual void update() =0;
    virtual const Node* getNode() const =0;
};

class TopoModule : public TopoNode
{
  public:
    Module* module;
    TopoModule(Module*  module) : module(module) {}
    virtual ~TopoModule() {}
    void init() {module->init();}
    void update() { module->execute();}
    const Node* getNode() const { return module; }
};

class TopoRepresentation: public TopoNode
{
  public:
    Module* module;
    Representation* representation;
    TopoRepresentation(Module* module, Representation* representation) : module(module), representation(representation) {}
    virtual ~TopoRepresentation() {}
    void init() {/** For later use */}
    void update() { representation->updateThis(module, representation); }
    const Node* getNode() const { return representation; }
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

    typedef std::vector<ModuleEntry*> ModuleVector;
    typedef std::vector<RepresentationEntry*> RepresentationVector;
    typedef std::vector<ModuleRepresentationEntry*> ModuleRepresentationVector;
    typedef std::map<unsigned int, int> InDegreesMap;
    typedef std::map<unsigned int, Node*> GraphStructure;
    ModuleVector moduleVector;
    RepresentationVector representationVector;
    ModuleRepresentationVector moduleRepresentationRequiredVector;
    ModuleRepresentationVector moduleRepresentationUsedVector;
    InDegreesMap inDegreesMap;
    GraphStructure graphStructure;
    unsigned int nodeCounter;
    bool error;

    // For topological sort
    typedef std::vector<Node*> TopoQueue;
    typedef std::vector<TopoNode*> GraphOutput;
    TopoQueue topoQueue;
    GraphOutput graphOutput;

    static Graph& getInstance();
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

  protected:
    Graph();
    ~Graph();
    Graph(Graph const&);
    Graph& operator=(Graph const&);

  public: /** verbose */
    //void stream();
};

// All the computational units are loaded into an instance of this class.
template <class T>
class ModuleLoader
{
  public:
    T* theInstance;
    ModuleLoader() : theInstance(new T) { Graph::getInstance().addModule(theInstance); }
    ~ModuleLoader() { delete theInstance; }
};

template<const char* (*getModuleName)(), void (*updateRepresentation)(Node*, Node*), class T>
class RepresentationProvider
{
  public:
    T* theInstance;
    RepresentationProvider() : theInstance(new T) { Graph::getInstance().providedRepresentation(getModuleName(), theInstance, updateRepresentation); }
    ~RepresentationProvider() { delete theInstance; }
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
    const bool isNull()   const { return (getRepresentation() == 0); }

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
    const bool isNull()   const { return (getRepresentation() == 0); }

};


#endif /* FRAMEWORK_H_ */
