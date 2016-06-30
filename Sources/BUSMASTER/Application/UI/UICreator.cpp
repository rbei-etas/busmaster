#include "stdafx.h"
#include "UICreator.h"
#include "MenuCreator.h"
#include "IdGenerator.h"
#include "ToolBarCreator.h"

UICreator::~UICreator()
{
    if (nullptr != mMenucreator)
    {
        delete mMenucreator;
    }
    if (nullptr != mToolbarcreator)
    {
        delete mToolbarcreator;
    }
}

UICreator::UICreator(UIElements uielements, IIdGenerator* idgenerator)
{
    mUiElements.menu = uielements.menu;
    mUiElements.toolbarList = uielements.toolbarList;
    mMenucreator = new MenuCreator();
    mToolbarcreator = new ToolBarCreator();
    mIdgenerator = idgenerator;
}

int UICreator::Create(std::map<std::string, pluginMenuList> populatelist)
{
    mPluginMenuList = populatelist;
    mMenucreator->populateMenuList(mUiElements.menu, mPluginMenuList, mIdgenerator);
    mToolbarcreator->populateToolBar(mUiElements.toolbarList, mPluginMenuList, mIdgenerator);
    return 1;
}

int UICreator::getPluginMenuInfo(unsigned int id, std::string& pluginId, std::string& actualMenuId)
{
    mMenucreator->getPluginMenuInfo(id, pluginId, actualMenuId);
    return 0;
}
