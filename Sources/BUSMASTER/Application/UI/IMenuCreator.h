#pragma once

#include <map>
#include <string>
#include<iostream>
#include "IIdGenerator.h"
#include "Menus.h"



typedef std::list<RibbonButton> pluginMenuList;

class IMenuCreator
{
public:
    virtual ~IMenuCreator() {};
	virtual void populateRibbonBar(CMFCRibbonBar* mainMenu, std::map<std::string, pluginMenuList> menulist, IIdGenerator* idgenerator) = 0;
    virtual void getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId) = 0;
};
