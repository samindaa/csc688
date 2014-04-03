/*
 * Copyright 2014 Saminda Abeyruwan (saminda@cs.miami.edu)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Action.h
 *
 *  Created on: Aug 19, 2012
 *      Author: sam
 */

#ifndef ACTION_H_
#define ACTION_H_

#include <vector>

namespace RLLib
{

template<class T>
class Action
{
  private:
    int actionID;
    std::vector<T> values;

  public:
    explicit Action(const int& actionID) :
        actionID(actionID)
    {
    }

    ~Action()
    {
    }

    void push_back(const T& value)
    {
      values.push_back(value);
    }

    T at(const int& i = 0 /*default to a single action*/)
    {
#if defined(ENERGIA)
      return values[i];
#else
      return values.at(i);
#endif
    }

    const T at(const int& i = 0 /*default to a single action*/) const
    {
#if defined(ENERGIA)
      return values[i];
#else
      return values.at(i);
#endif
    }

    int dimension() const
    {
      return values.size();
    }

    void update(const int& i, const T& value)
    {
#if defined(ENERGIA)
      values[i] = value;
#else
      values.at(i) = value;
#endif
    }

    bool operator==(const Action<T>& that) const
    {
      return actionID == that.actionID;
    }

    bool operator!=(const Action<T>& that) const
    {
      return actionID != that.actionID;
    }

    // Id within an associated id group
    int id() const
    {
      return actionID;
    }

};

template<class T>
class Actions
{
  protected:
    std::vector<Action<T>*> actions;

  public:
    Actions()
    {
    }

    virtual ~Actions()
    {
    }

    virtual int dimension() const =0;
    virtual const Action<T>* operator[](const int& index) const =0;
    virtual const Action<T>* at(const int& index) const =0;
    virtual void push_back(const int& index, const T& value) =0;
    virtual void erase(const int& index) =0;
    virtual void update(const int& actionIndex, const int& vectorIndex, const T& value) =0;

    typedef typename std::vector<Action<T>*>::iterator iterator;
    typedef typename std::vector<Action<T>*>::const_iterator const_iterator;

    iterator begin()
    {
      return actions.begin();
    }

    const_iterator begin() const
    {
      return actions.begin();
    }

    iterator end()
    {
      return actions.end();
    }

    const_iterator end() const
    {
      return actions.end();
    }

};

template<class T>
class ActionArray: public Actions<T>
{
  private:
    typedef Actions<T> Base;
  public:
    ActionArray(const int& nbActions) :
        Actions<T>()
    {
      for (int i = 0; i < nbActions; i++)
        Base::actions.push_back(new Action<T>(i));
    }

    virtual ~ActionArray()
    {
      for (typename std::vector<Action<T>*>::iterator iter = Base::actions.begin();
          iter != Base::actions.end(); ++iter)
        delete *iter;
      Base::actions.clear();
    }

    const Action<T>* operator[](const int& index) const
    {
#if defined(ENERGIA)
      return Base::actions[index];
#else
      return Base::actions.at(index);
#endif
    }

    const Action<T>* at(const int& index) const
    {
#if defined(ENERGIA)
      return Base::actions[index];
#else
      return Base::actions.at(index);
#endif
    }

    void push_back(const int& index, const T& value)
    {
#if defined(ENERGIA)
      Base::actions[index]->push_back(value);
#else
      Base::actions.at(index)->push_back(value);
#endif
    }

    void erase(const int& index)
    {
      typename std::vector<Action<T>*>::iterator iter = Base::actions.begin();
      while (iter != Base::actions.end())
      {
        if ((*iter)->id() == index)
        {
          Base::actions.erase(iter);
          break;
        }
        else
          ++iter;
      }
    }

    void update(const int& actionIndex, const int& vectorIndex, const T& value)
    {
#if defined(ENERGIA)
      Base::actions[actionIndex]->update(vectorIndex, value);
#else
      Base::actions.at(actionIndex)->update(vectorIndex, value);
#endif
    }

    int dimension() const
    {
      return Base::actions.size();
    }
};

} // namespace RLLib

#endif /* ACTION_H_ */
