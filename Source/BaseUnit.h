#ifndef _BASEUNIT_H_
#define _BASEUNIT_H_

#include <string>
#include <unordered_map>

#include "Geist/Config.h"
#include "Geist/Object.h"

class UnitConfigMap
{
public:
    std::unordered_map<std::string, ConfigInfo>::iterator find(const std::string& key)
    {
        return m_Data.find(key);
    }

    std::unordered_map<std::string, ConfigInfo>::const_iterator find(const std::string& key) const
    {
        return m_Data.find(key);
    }

    std::unordered_map<std::string, ConfigInfo>::iterator end()
    {
        return m_Data.end();
    }

    std::unordered_map<std::string, ConfigInfo>::const_iterator end() const
    {
        return m_Data.end();
    }

    ConfigInfo& operator[](const std::string& key)
    {
        return m_Data[key];
    }

    const ConfigInfo& operator[](const std::string& key) const
    {
        static ConfigInfo s_Empty;
        auto it = m_Data.find(key);
        if (it != m_Data.end())
            return it->second;
        return s_Empty;
    }

    void clear() { m_Data.clear(); }

private:
    std::unordered_map<std::string, ConfigInfo> m_Data;
};

void LoadUnitConfigFile(UnitConfigMap& configMap, const std::string& filename);

class BaseUnit : public Object
{
public:
    BaseUnit() = default;
    virtual ~BaseUnit() = default;

    virtual void Init(const std::string& configfile);
    virtual void Shutdown();
    virtual void Update();
    virtual void Draw();

    virtual bool IsDead() { return m_IsDead; }

    bool m_IsDead = false;
    UnitConfigMap m_UnitConfig;
};

#endif