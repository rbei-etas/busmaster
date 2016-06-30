#pragma once
#include "Menus.h"
#include <map>
#include "IIdGenerator.h"

static std::map < std::string, int > menuidlist;
class IdGenerator :public IIdGenerator
{
public:
    int generateId(std::string, int& id);
    //std::map < std::string, int > menuidlist;
    bool removeid(std::string);
    int setStartId(int mid);
    std::string GetMenuItem(UINT id, std::map<std::string, std::list<PluginMenu>>);

};

