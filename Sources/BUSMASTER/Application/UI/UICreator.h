#pragma once
#include <map>
#include <string>
#include<iostream>
#include "IUICreator.h"
#include "IMenuCreator.h"
#include "ToolBarCreator.h"
#include "IIdgenerator.h"

struct UIElements
{
    CMenu* menu;
    std::map <std::string, CToolBar*>toolbarList;
};
/*class IIdGenerator;
class IMenuCreator;
class ToolBarCreator;*/

class UICreator:public IUICreator
{
private :
    UIElements mUiElements;
    IMenuCreator* mMenucreator = nullptr;
    ToolBarCreator* mToolbarcreator = nullptr;
    IIdGenerator* mIdgenerator;
    std::map<std::string, std::list<PluginMenu>> mPluginMenuList;
public:

    UICreator(UICreator::UIElements, IIdGenerator*);
    ~UICreator();
    int Create(std::map<std::string, std::list<PluginMenu>> populatelist);
    int getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId);
    int CreateToolbar(CImageList* imagelist, std::list<ToolButton> path);
};

