#pragma once

#include <map>
#include <string>
#include<iostream>
#include "IIdGenerator.h"
#include "Menus.h"



typedef std::list<PluginMenu> pluginMenuList;

class IMenuCreator
{
public:
    virtual ~IMenuCreator() {};
    virtual void populateMenuList(CMenu* mainMenu, std::map<std::string, pluginMenuList> menulist, IIdGenerator* idgenerator) = 0;
    virtual void getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId) = 0;
};
