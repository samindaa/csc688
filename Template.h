/*
 * Template.h
 *
 *  Created on: Jul 29, 2013
 *      Author: sam
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#include "Framework.h"
// Macros to create the computational units as well as the representations that they provide.
#define MODULE(NAME)                                                        \
class NAME;                                                                 \
class NAME##Base : public Module                                       \
{                                                                           \
  private: typedef NAME##Base _This;                                        \
  public: NAME##Base() : Module() {}                                   \
  public: virtual ~NAME##Base() {}                                          \
  public: const char* getName() const { return #NAME ;}                     \
  public: static const char* getNameStatic() { return #NAME ; }             \

#define REQUIRES(REPRESENTATION)                                            \
  private: static const char* getRepresentation##REPRESENTATION() { return #REPRESENTATION ; }       \
  protected: RepresentationRequierer<&_This::getNameStatic, &_This::getRepresentation##REPRESENTATION, REPRESENTATION> the##REPRESENTATION;      \

#define PROVIDES(REPRESENTATION)                                            \
  protected: virtual void update(REPRESENTATION& the##REPRESENTATION) =0;     \
  protected: static void _update##REPRESENTATION(Node* moduleNode, Node* representationNode)         \
          { (((_This*) moduleNode)->update(*((REPRESENTATION*) representationNode))); } \
  protected: RepresentationProvider<&_This::getNameStatic, &_This::_update##REPRESENTATION, REPRESENTATION> _the##REPRESENTATION##Provides;      \

#define USES(REPRESENTATION)                                               \
  private: static const char* getRepresentation##REPRESENTATION() { return #REPRESENTATION ; }       \
  protected: RepresentationUser<&_This::getNameStatic, &_This::getRepresentation##REPRESENTATION, REPRESENTATION> the##REPRESENTATION;                                  \

#define END_MODULE };                                                      \

#define MAKE_MODULE(NAME) ModuleLoader<NAME> _the##NAME##Module;

#define REPRESENTATION(NAME)                                                \
class NAME;                                                                 \
class NAME##Base : public Representation                               \
{                                                                           \
  private: typedef NAME##Base _This;                                        \
  public: NAME##Base() : Representation() {}                           \
  public: virtual ~NAME##Base() {}                                          \
  public: const char* getName() const { return #NAME ;}                     \
};                                                                          \

#endif /* TEMPLATE_H_ */
