#pragma once
#include<iostream>
#include <list>
#include "Menus.h"
#include "IMenuCreator.h"
#include <string>
#include <map>
#include "IIdGenerator.h"
struct pluginIdtoMenuId
{
    std::string mPluginId;
    std::string mMenuId;
};

class MenuCreator : public IMenuCreator
{
    CMenu* mMainMenu;
    std::map < int, pluginIdtoMenuId> mMenuIdPluginInfo;
    std::map<std::string, pluginMenuList> mPluginList;
public:
    void populateMenuList(CMenu* mainMenu, std::map<std::string, pluginMenuList> menulist, IIdGenerator* idgenerator);
    void getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId);

private:
    void populatePluginMenu(std::string pluginId, PluginMenu menu, IIdGenerator* idgenerator);
    void populatePluginMenuList(std::string pluginId, pluginMenuList&, IIdGenerator* idgenerator);
    int getParentMenuIndex(std::string name);
    void populateMenu(std::string pluginId, CMenu* menu, Menu item, IIdGenerator* idgenerator);
};

