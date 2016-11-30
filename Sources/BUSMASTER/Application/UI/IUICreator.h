#pragma once
#include <map>
#include <list>
#include <string>
#include<iostream>
#include "Menus.h"

class IUICreator
{
public:
    virtual ~IUICreator(){};
	virtual int Create(std::map<std::string, std::list<RibbonButton>> populatelist) = 0;
    virtual int getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId)=0;
};

