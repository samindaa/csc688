/*
 * Config.h
 *
 *  Created on: Jul 16, 2013
 *      Author: sam
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#if defined(ENERGIA)
#define EMBEDDED_MODE
#endif

#if !defined(EMBEDDED_MODE)
#ifdef WIN32
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include <iostream>
#include <sstream>

class Config
{
  private:
    typedef std::tr1::unordered_map<std::string, std::string> HashMap;
    HashMap* values;
    std::string name;
    std::string path;
    bool modified;
    bool persistenceMode;

  public:
    Config();
    Config(const std::string name, const std::string path);
    ~Config();

  private:
    template<class T>
    T getCfgValue(const std::string name, T value)
    {
      HashMap::iterator iter = values->find(name);
      std::string cfgValue;
      if (iter != values->end())
      {
        cfgValue = iter->second;
        std::stringstream ss(cfgValue);
        ss >> value;
        return value;
      }
      else
      {
        // New value has been found
        modified = true;
        std::stringstream ss;
        ss << value;
        values->insert(std::make_pair((std::string) name, ss.str()));
      }
      return value;
    }

    template<class T>
    void setCfgValue(const std::string name, T value)
    {
      HashMap::iterator iter = values->find(name);
      if (iter != values->end())
      {
        values->erase(iter);
      }
      modified = true;
      std::stringstream ss;
      ss << value;
      values->insert(std::make_pair((std::string) name, ss.str()));
    }

  public:
    bool getValue(const std::string name, bool defaultValue)
    {
      return getCfgValue(name, defaultValue);
    }

    int getValue(const std::string name, int defaultValue)
    {
      return getCfgValue(name, defaultValue);
    }

    double getValue(const std::string name, double defaultValue)
    {
      return getCfgValue(name, defaultValue);
    }

    std::string getValue(const std::string name, std::string defaultValue)
    {
      return getCfgValue(name, defaultValue);
    }

    void setValue(const std::string name, bool value)
    {
      setCfgValue(name, value);
    }

    void setValue(const std::string name, int value)
    {
      setCfgValue(name, value);
    }

    void setValue(const std::string name, long value)
    {
      setCfgValue(name, value);
    }

    void setValue(const std::string name, float value)
    {
      setCfgValue(name, value);
    }

    void setValue(const std::string name, double value)
    {
      setCfgValue(name, value);
    }

    void setValue(const std::string name, std::string value)
    {
      setCfgValue(name, value);
    }

    void setName(const std::string name);
    void setPath(const std::string path);
    void persist();
    void resurrect();
    void setPersist(const bool persistenceMode);

};
#endif
#endif /* CONFIG_H_ */
