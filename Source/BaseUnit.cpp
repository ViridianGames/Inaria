#include "BaseUnit.h"

#include <fstream>
#include <sstream>

using namespace std;

void LoadUnitConfigFile(UnitConfigMap& configMap, const std::string& filename)
{
    ifstream instream(filename);
    if (instream.fail())
        return;

    configMap.clear();

    string line;
    while (getline(instream, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        const auto idx = line.find('=');
        if (idx == string::npos)
            continue;

        string leftside = line.substr(0, idx);
        string rightside = line.substr(idx + 1);

        while (!leftside.empty() && (leftside.back() == ' ' || leftside.back() == '\t'))
            leftside.pop_back();
        while (!rightside.empty() && (rightside.front() == ' ' || rightside.front() == '\t'))
            rightside.erase(0, 1);

        ConfigInfo temp;
        istringstream numbertest(rightside);
        float numtest = 0.0f;
        numbertest >> numtest;
        if (!numbertest || rightside.find_first_not_of("0123456789-.") != string::npos)
        {
            temp.datatype = Config::DATA_STRING;
            temp.stringdata = rightside;
        }
        else
        {
            temp.datatype = Config::DATA_NUMBER;
            temp.numdata = numtest;
        }

        configMap[leftside] = temp;
    }
}

void BaseUnit::Init(const std::string& configfile)
{
    m_IsDead = false;
    if (!configfile.empty())
        LoadUnitConfigFile(m_UnitConfig, configfile);
}

void BaseUnit::Shutdown()
{
    m_IsDead = true;
    m_UnitConfig.clear();
}

void BaseUnit::Update()
{
}

void BaseUnit::Draw()
{
}